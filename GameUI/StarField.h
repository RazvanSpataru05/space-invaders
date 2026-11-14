#pragma once

// Background of small green dots ("stars") that drift slowly downwards and twinkle.
// Pure UI decoration: no game logic, no raylib types in this header.

#include <cstddef>
#include <vector>

class StarField
{
public:
    // Fills the given area with starCount stars at random positions.
    void Initialize(int width, int height, int starCount);
    // Moves the stars and advances the twinkle animation.
    void Update(float deltaTime);
    void Draw() const;

private:
    struct Star
    {
        float x = 0.0f;
        float y = 0.0f;
        float speed = 0.0f;        // pixels per second, downwards
        float twinklePhase = 0.0f; // radians
        float twinkleSpeed = 0.0f; // radians per second
        std::size_t layer = 0;     // 0 = far (dim, slow) ... 2 = near (bright, fast)
    };

    // Re-rolls a star. anywhereOnScreen = false places it just above the top edge.
    void ResetStar(Star& star, bool anywhereOnScreen) const;

    std::vector<Star> m_stars;
    int m_width = 0;
    int m_height = 0;
    float m_time = 0.0f;
};
