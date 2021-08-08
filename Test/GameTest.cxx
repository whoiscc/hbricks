#include <cstdint>

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
    explicit CollectedView(struct ViewState *view) {
      for (int i = 0; i < view->n; i += 1) {
        switch (view->log[i].kind) {
        case ViewState::Log::Kind::ADD_BALL: {
          ball_x = view->log[i].add_ball.x;
          ball_y = view->log[i].add_ball.y;
          break;
        }
        case ViewState::Log::Kind::ADD_BRACKET: {
          bracket_x = view->log[i].add_bracket.x;
          bracket_y = view->log[i].add_bracket.y;
          break;
        }
        default: {
          break;
        }
        }
      }
    }
  };
//  CollectedView record;

};

TEST_F(GameTest, CanInit) {  // NOLINT
}

TEST_F(GameTest, NotGameOverFromStart) {  // NOLINT
  ASSERT_FALSE(Game.IsOver(state));
}

TEST_F(GameTest, DrawInit) {  // NOLINT
  std::uint8_t view_mem[SIZEOF_LogViewState];
  ViewState *view = InitLogView(view_mem);
  Game.Draw(state, view, &LogView);
  ASSERT_EQ(view->n, 2);
  CollectedView record(view);
  ASSERT_DOUBLE_EQ(record.ball_x, config.screen_width / 2.);
  ASSERT_NEAR(record.ball_y, config.screen_height - config.bracket_height - config.ball_radius, 0.1);
  ASSERT_DOUBLE_EQ(record.bracket_x, config.screen_width / 2.);
  ASSERT_DOUBLE_EQ(record.bracket_y, config.screen_height - config.bracket_height / 2.);
}