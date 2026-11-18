#include "GameAudio.h"

#include "Utils/Constants.h"

#include <algorithm>

namespace
{
	constexpr float kMasterVolume = 0.8f;

	constexpr const char* kUfoSirenPath = "Assets/Audio/ufo_siren_loop.wav";

	// The siren is continuous, so it sits below the one-shots in the mix.
	constexpr float kUfoSirenMaxVolume = 0.55f;
	// Share of full volume reached at the end of the warning window; the spawn then ramps to 1.
	constexpr float kUfoApproachPeak = 0.6f;
	constexpr float kUfoSpawnRampTime = 0.3f;
	constexpr float kUfoCancelFadeTime = 0.6f;
	// Pitch follows the volume envelope: rises as the UFO approaches, drops as it leaves.
	constexpr float kUfoMinPitch = 0.75f;

	constexpr std::array<SoundCue, 6> kAllSoundCues{
		SoundCue::MENU_MOVE,
		SoundCue::MENU_CONFIRM,
		SoundCue::PLAYER_SHOT,
		SoundCue::PLAYER_HIT,
		SoundCue::ENEMY_HIT,
		SoundCue::ENEMY_DESTROYED };

	const char* SoundPath(SoundCue cue)
	{
		switch (cue)
		{
		case SoundCue::MENU_MOVE:			return "Assets/Audio/menu_move.wav";
		case SoundCue::MENU_CONFIRM:		return "Assets/Audio/menu_confirm.wav";
		case SoundCue::PLAYER_SHOT:			return "Assets/Audio/player_shot.wav";
		case SoundCue::PLAYER_HIT:			return "Assets/Audio/player_hit.wav";
		case SoundCue::ENEMY_HIT:			return "Assets/Audio/enemy_hit.wav";
		case SoundCue::ENEMY_DESTROYED:		return "Assets/Audio/enemy_destroyed.wav";
		}
		return "";
	}
}

GameAudio::~GameAudio()
{
	if (!m_isInitialized) return;

	for (Sound& sound : m_sounds)
	{
		if (IsSoundValid(sound)) UnloadSound(sound);
	}

	if (IsMusicValid(m_ufoSiren)) UnloadMusicStream(m_ufoSiren);

	CloseAudioDevice();
}

void GameAudio::Initialize()
{
	static_assert(kAllSoundCues.size() == kSoundCount, "every SoundCue needs a slot in m_sounds");

	if (m_isInitialized) return;

	InitAudioDevice();
	if (!IsAudioDeviceReady()) return;	// no audio output: every call stays a no-op

	SetMasterVolume(kMasterVolume);

	for (SoundCue cue : kAllSoundCues)
	{
		m_sounds[static_cast<size_t>(cue)] = LoadSound(SoundPath(cue));
	}

	m_ufoSiren = LoadMusicStream(kUfoSirenPath);
	m_ufoSiren.looping = true;

	m_isInitialized = true;
}

void GameAudio::Update(float deltaTime)
{
	if (!m_isInitialized || m_ufoPhase == UfoSirenPhase::SILENT) return;

	UpdateMusicStream(m_ufoSiren);
	if (m_isUfoSirenPaused) return;

	switch (m_ufoPhase)
	{
	case UfoSirenPhase::SILENT:
		return;
	case UfoSirenPhase::APPROACHING:
		m_ufoVolume = std::min(m_ufoVolume + deltaTime * kUfoApproachPeak / GameLogic::Utils::Constants::UFO_WARNING_TIME,
			kUfoApproachPeak);
		break;
	case UfoSirenPhase::ACTIVE:
		m_ufoVolume = std::min(m_ufoVolume + deltaTime / kUfoSpawnRampTime, 1.0f);
		break;
	case UfoSirenPhase::LEAVING:
		m_ufoVolume = std::min(m_ufoVolume,
			std::clamp(m_ufoDistanceToExit / GameLogic::Utils::Constants::UFO_LEAVING_DISTANCE, 0.0f, 1.0f));
		break;
	case UfoSirenPhase::FADING_OUT:
		m_ufoVolume -= deltaTime / kUfoCancelFadeTime;
		if (m_ufoVolume <= 0.0f)
		{
			StopUfoSiren();
			return;
		}
		break;
	}

	SetMusicVolume(m_ufoSiren, m_ufoVolume * kUfoSirenMaxVolume);
	SetMusicPitch(m_ufoSiren, kUfoMinPitch + (1.0f - kUfoMinPitch) * m_ufoVolume);
}

void GameAudio::Play(SoundCue cue)
{
	if (!m_isInitialized) return;

	const Sound& sound = m_sounds[static_cast<size_t>(cue)];
	if (IsSoundValid(sound)) PlaySound(sound);
}

void GameAudio::StartUfoApproach()
{
	if (StartUfoSiren()) m_ufoPhase = UfoSirenPhase::APPROACHING;
}

void GameAudio::OnUfoSpawned()
{
	// Normally already playing from the approach; start it here if that event was missed.
	if (m_ufoPhase == UfoSirenPhase::SILENT && !StartUfoSiren()) return;
	m_ufoPhase = UfoSirenPhase::ACTIVE;
}

void GameAudio::StartUfoLeaving()
{
	if (m_ufoPhase == UfoSirenPhase::SILENT) return;
	m_ufoPhase = UfoSirenPhase::LEAVING;
}

void GameAudio::FadeOutUfoSiren()
{
	if (m_ufoPhase == UfoSirenPhase::SILENT) return;
	m_ufoPhase = UfoSirenPhase::FADING_OUT;
}

void GameAudio::StopUfoSiren()
{
	if (m_isInitialized && IsMusicValid(m_ufoSiren)) StopMusicStream(m_ufoSiren);

	m_ufoPhase = UfoSirenPhase::SILENT;
	m_ufoVolume = 0.0f;
}

void GameAudio::SetUfoDistanceToExit(float distance) noexcept
{
	m_ufoDistanceToExit = distance;
}

void GameAudio::SetUfoSirenPaused(bool paused)
{
	if (paused == m_isUfoSirenPaused) return;
	m_isUfoSirenPaused = paused;

	if (!m_isInitialized || m_ufoPhase == UfoSirenPhase::SILENT) return;

	if (paused) PauseMusicStream(m_ufoSiren);
	else        ResumeMusicStream(m_ufoSiren);
}

bool GameAudio::StartUfoSiren()
{
	if (!m_isInitialized || !IsMusicValid(m_ufoSiren)) return false;

	m_ufoVolume = 0.0f;
	SetMusicVolume(m_ufoSiren, 0.0f);
	SetMusicPitch(m_ufoSiren, kUfoMinPitch);

	StopMusicStream(m_ufoSiren);
	PlayMusicStream(m_ufoSiren);

	if (m_isUfoSirenPaused) PauseMusicStream(m_ufoSiren);
	return true;
}
