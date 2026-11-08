#include "StarField.h"

#include <array>
#include <cmath>

#include <raylib.h>

namespace
{
    struct LayerStyle
    {
        float minSpeed;
        float maxSpeed;
        int size; // square side in pixels
        Color color;
    };

    // Three depth layers give a cheap parallax effect.
    constexpr std::array<LayerStyle, 3> kLayers{ {
        { 6.0f, 10.0f, 1, Color{ 25, 95, 25, 255 } },   // far
        { 14.0f, 20.0f, 2, Color{ 30, 150, 30, 255 } }, // mid
        { 26.0f, 36.0f, 2, Color{ 80, 235, 80, 255 } }, // near
    } };

    constexpr float kMinTwinkleSpeed = 1.0f;
    constexpr float kMaxTwinkleSpeed = 3.0f;
    constexpr float kMinBrightness = 0.3f; // alpha factor at the dimmest point of a twinkle
    constexpr float kTimeWrap = 10000.0f;  // keeps m_time small enough for float precision

    float RandomFloat(float min, float max)
    {
        constexpr int resolution = 10000;
        const float t = static_cast<float>(GetRandomValue(0, resolution)) / static_cast<float>(resolution);
        return min + (max - min) * t;
    }

    // 60% far, 30% mid, 10% near
    std::size_t PickLayer()
    {
        const int roll = GetRandomValue(0, 99);
        if (roll < 60)
        {
            return 0;
        }
        return roll < 90 ? 1 : 2;
    }
}

void StarField::Initialize(int width, int height, int starCount)
{
    m_width = width;
    m_height = height;
    m_time = 0.0f;

    m_stars.assign(static_cast<std::size_t>(starCount), Star{});
    for (Star& star : m_stars)
    {
        ResetStar(star, true);
    }
}

void StarField::ResetStar(Star& star, bool anywhereOnScreen) const
{
    star.layer = PickLayer();
    const LayerStyle& style = kLayers[star.layer];

    star.x = RandomFloat(0.0f, static_cast<float>(m_width));
    star.y = anywhereOnScreen ? RandomFloat(0.0f, static_cast<float>(m_height))
                              : -static_cast<float>(style.size);
    star.speed = RandomFloat(style.minSpeed, style.maxSpeed);
    star.twinklePhase = RandomFloat(0.0f, 2.0f * PI);
    star.twinkleSpeed = RandomFloat(kMinTwinkleSpeed, kMaxTwinkleSpeed);
}

void StarField::Update(float deltaTime)
{
    m_time += deltaTime;
    if (m_time > kTimeWrap)
    {
        m_time -= kTimeWrap;
    }

    for (Star& star : m_stars)
    {
        star.y += star.speed * deltaTime;
        if (star.y > static_cast<float>(m_height))
        {
            ResetStar(star, false);
        }
    }
}

void StarField::Draw() const
{
    for (const Star& star : m_stars)
    {
        const LayerStyle& style = kLayers[star.layer];

        // sin in [-1, 1] -> brightness in [kMinBrightness, 1]
        const float wave = 0.5f + 0.5f * std::sin(m_time * star.twinkleSpeed + star.twinklePhase);
        const float brightness = kMinBrightness + (1.0f - kMinBrightness) * wave;

        Color color = style.color;
        color.a = static_cast<unsigned char>(255.0f * brightness);

        DrawRectangle(static_cast<int>(star.x), static_cast<int>(star.y), style.size, style.size, color);
    }
}
