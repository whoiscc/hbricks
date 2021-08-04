#include <gtest/gtest.h>
#include <cstdint>
extern "C" {
#include "Game.h"
#include "GameInteral.h"
}

class GameTest : public testing::Test {
protected:
  std::uint8_t state_mem[SIZEOF_GameState]{};
  struct GameState *state{};
  struct GameConfig config{};

  void SetUp() override {
    state = reinterpret_cast<struct GameState *>(state_mem);
    config.screen_width = 800;
    config.screen_height = 600;
    config.bracket_width = 80;
    config.bracket_height = 10;
    config.brick_width = 20;
    config.bracket_height = 20;
    config.nb_brick_col = 30;
    config.ball_radius = 7;
    config.speed = 4;
    Game.Init(state, &config);
  }

  void TearDown() override {
    Game.Drop(state);
  }
};

TEST_F(GameTest, CanInit) {  // NOLINT
}

TEST_F(GameTest, NotGameOverFromStart) {  // NOLINT
  ASSERT_FALSE(Game.IsOver(state));
}

// initially ball should head up right at 45 degree
TEST_F(GameTest, InitBallProperty) {  // NOLINT
  ASSERT_DOUBLE_EQ(state->ball_x, state->bracket_x);
  ASSERT_DOUBLE_EQ(state->ball_y + state->config->ball_radius,
                   state->config->screen_height - state->config->bracket_height);
  ASSERT_GT(state->ball_vx, 0);
  ASSERT_LT(state->ball_vy, 0);
  ASSERT_DOUBLE_EQ(state->ball_vx, -state->ball_vy);
}

TEST_F(GameTest, OneTick) {  // NOLINT
  double old_x = state->ball_x;
  double old_y = state->ball_y;
  Game.NextTick(state);
  ASSERT_DOUBLE_EQ(state->ball_x, old_x + state->ball_vx);
  ASSERT_DOUBLE_EQ(state->ball_y, old_y + state->ball_vy);
}

TEST_F(GameTest, BouceWall) {  // NOLINT
  state->ball_x = state->config->screen_width - state->config->ball_radius - 0.001;
  double old_vx = state->ball_vx;
  double old_vy = state->ball_vy;
  Game.NextTick(state);
  Game.NextTick(state);
  ASSERT_DOUBLE_EQ(state->ball_vx, -old_vx);
  ASSERT_DOUBLE_EQ(state->ball_vy, old_vy);

  state->ball_x = state->config->ball_radius + 0.001;
  old_vx = state->ball_vx;
  old_vy = state->ball_vy;
  Game.NextTick(state);
  Game.NextTick(state);
  ASSERT_DOUBLE_EQ(state->ball_vx, -old_vx);
  ASSERT_DOUBLE_EQ(state->ball_vy, old_vy);

  // initially top wall is probably covered by bricks, so not tested
}

TEST_F(GameTest, BouceWallAndGetSlow) {  // NOLINT
  state->ball_x = state->config->screen_width - state->config->ball_radius - 0.001;
  double old_vx = state->ball_vx;
  double old_vy = state->ball_vy;
  Game.NextTick(state);
  state->ball_vx *= 0.1;
  state->ball_vy *= 0.1;
  config.speed *= 0.1;  // not necessary for now, just for consistency
  Game.NextTick(state);
  ASSERT_DOUBLE_EQ(state->ball_vx, -old_vx * 0.1);
  ASSERT_DOUBLE_EQ(state->ball_vy, old_vy * 0.1);
  old_vx = state->ball_vx;
  old_vy = state->ball_vy;
  Game.NextTick(state);
  ASSERT_DOUBLE_EQ(state->ball_vx, old_vx);
  ASSERT_DOUBLE_EQ(state->ball_vy, old_vy);
}
