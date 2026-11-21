#include "pch.h"
#include "Structs/UFOState.h"
#include "Utils/Constants.h"

#include <array>

namespace Constants = GameLogic::Utils::Constants;

class UFOStateTests : public ::testing::Test
{
protected:
	const GameLogic::UFOState ufo{ -1.0f, Constants::UFO_RIGHT_SPAWN_POSITION };
};

TEST_F(UFOStateTests, ConstructorSetsDirectionAndPosition)
{
	EXPECT_FLOAT_EQ(ufo.direction, -1.0f);
	EXPECT_FLOAT_EQ(ufo.position.x, Constants::UFO_RIGHT_SPAWN_POSITION.x);
	EXPECT_FLOAT_EQ(ufo.position.y, Constants::UFO_RIGHT_SPAWN_POSITION.y);
}

TEST_F(UFOStateTests, ScoreTableMatchesGameRules)
{
	const std::array<uint16_t, 16> expected
	{
		100, 50, 50, 100,
		150, 100, 100, 50,
		300, 100, 100, 100,
		50, 150, 100, 50
	};

	EXPECT_EQ(ufo.scores, expected);
}

TEST_F(UFOStateTests, ScoreTableContainsOnlyAllowedValues)
{
	for (size_t i = 0; i < ufo.scores.size(); ++i)
	{
		const uint16_t score = ufo.scores[i];

		EXPECT_TRUE(score == 50u || score == 100u || score == 150u || score == 300u)
			<< "Unexpected score " << score << " at index " << i;
	}
}