#pragma once

#include <cstddef>
#include <vector>

struct Star
{
    float x = 0.0f;
    float y = 0.0f;
    float speed = 0.0f;
    float twinklePhase = 0.0f;
    float twinkleSpeed = 0.0f;
    size_t layer = 0;
};

class StarField
{
public:
    void Initialize(int width, int height, int starCount);
    void Update(float deltaTime);
    void Draw() const;

private:
    void ResetStar(Star& star, bool anywhereOnScreen) const;

private:

    std::vector<Star> m_stars;
    int m_width{};
    int m_height{};
    float m_time{};
};
