#include <cstdint>
#define _USE_MATH_DEFINES
#include <cmath>

#include <gtest/gtest.h>

extern "C" {
#include "Game/Normal.h"
#include "LogView.h"
}

class GameTest : public testing::Test {
protected:
  std::uint8_t state_mem[SIZEOF_GameState]{};
  struct GameState *state{};
  struct GameConfig config = default_game_config;

  void SetUp() override {
    state = InitGame(state_mem, &config);
  }

  void TearDown() override {
    DropGame(state);
  }

  struct CollectedView {
    double ball_x = -1, ball_y = -1;
    double bracket_x = -1, bracket_y = -1;
    CollectedView() = default;

    static CollectedView Collect(struct ViewState *view) {
      CollectedView collect;
      for (int i = 0; i < view->n; i += 1) {
        switch (view->log[i].kind) {
        case ViewState::Log::Kind::ADD_BALL: {
          collect.ball_x = view->log[i].add_ball.x;
          collect.ball_y = view->log[i].add_ball.y;
          break;
        }
        case ViewState::Log::Kind::ADD_BRACKET: {
          collect.bracket_x = view->log[i].add_bracket.x;
          collect.bracket_y = view->log[i].add_bracket.y;
          break;
        }
        default: {
          break;
        }
        }
      }
      return collect;
    }

    explicit CollectedView(struct GameState *state) {
      uint8_t view_mem[SIZEOF_LogViewState];
      struct ViewState *view = InitLogView(view_mem);
      Game.Draw(state, view, &LogView);
      *this = Collect(view);
    }
  };
  CollectedView sample;

  void Sample() {
    sample = CollectedView(state);
  }

  struct Interval {
    double dist = -1;
    double angle = -1;
  };
  [[nodiscard]] Interval SampleDifference() const {
    CollectedView next_sample(state);
    Interval interval;
    interval.dist = std::hypot(next_sample.ball_x - sample.ball_x, next_sample.ball_y - sample.ball_y);
    interval.angle = std::atan2(next_sample.ball_y - sample.ball_y, next_sample.ball_x - sample.ball_x);
    return interval;
  }
};

TEST_F(GameTest, CanInit) {  // NOLINT
}

TEST_F(GameTest, NotGameOverFromStart) {  // NOLINT
  ASSERT_FALSE(Game.IsOver(state));
}

TEST_F(GameTest, InitialState) {  // NOLINT
  Sample();
  ASSERT_DOUBLE_EQ(sample.ball_x, config.screen_width / 2.);
  ASSERT_NEAR(sample.ball_y, config.screen_height - config.bracket_height - config.ball_radius, 0.1);
  ASSERT_DOUBLE_EQ(sample.bracket_x, config.screen_width / 2.);
  ASSERT_DOUBLE_EQ(sample.bracket_y, config.screen_height - config.bracket_height / 2.);
}

TEST_F(GameTest, OneTick) {  // NOLINT
  Sample();
  Game.NextTick(state);
  Interval interval = SampleDifference();
  ASSERT_NEAR(interval.dist, config.speed, 1e-6);  // why
  ASSERT_DOUBLE_EQ(interval.angle, -M_PI / 4);
}

// todo bounce test when emitter is added
