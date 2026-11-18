#pragma once

#include "SoundCue.h"
#include "UfoSirenPhase.h"

#include <array>
#include <cstddef>

#include <raylib.h>

// Owns the audio device, the one-shot sounds and the UFO siren loop.
// GameUI translates GameEvents into calls on this class; it holds no game rules.
class GameAudio final
{
public:
    GameAudio() = default;
    ~GameAudio();

    void Initialize();
    void Update(float deltaTime);

    void Play(SoundCue cue);

    void StartUfoApproach();
    void OnUfoSpawned();
    void StartUfoLeaving();
    void FadeOutUfoSiren();
    void StopUfoSiren();

    void SetUfoDistanceToExit(float distance) noexcept;
    void SetUfoSirenPaused(bool paused);

private:
    bool StartUfoSiren();

private:
    GameAudio(const GameAudio&) = delete;
    GameAudio& operator=(const GameAudio&) = delete;

    GameAudio(GameAudio&&) = delete;
    GameAudio& operator=(GameAudio&&) = delete;

private:
    static constexpr size_t kSoundCount = 6;

    std::array<Sound, kSoundCount> m_sounds{};
    Music m_ufoSiren{};

    UfoSirenPhase m_ufoPhase = UfoSirenPhase::SILENT;
    float m_ufoVolume{};
    float m_ufoDistanceToExit{};
    bool m_isUfoSirenPaused{};
    bool m_isInitialized{};
};
