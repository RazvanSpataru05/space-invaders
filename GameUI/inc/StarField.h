#pragma once

#include "Star.h"

#include <cstddef>
#include <vector>

class StarField
{
public:
    void Initialize(int width, int height, size_t starCount);
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
