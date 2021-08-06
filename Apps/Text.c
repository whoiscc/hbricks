#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "Game.h"
#include "Views/Text.h"

int main(void) {
  uint8_t view_mem[SIZEOF_TextViewState];
  struct ViewState *view = InitTextView(view_mem);

  struct GameConfig config = {
      .screen_width = 800,
      .screen_height = 600,
      .bracket_width = 80,
      .bracket_height = 10,
      .brick_width = 20,
      .brick_height = 20,
      .nb_brick_col = 40,
      .ball_radius = 8,
      .speed = 4,
  };
  // todo interactive edit config

  uint8_t game_mem[SIZEOF_GameState];
  struct GameState *game = InitGame(game_mem, &config);

  while (1) {
    Game.Draw(game, view, &TextView);
#define LINE_LENGTH_MAX 80
    char line[LINE_LENGTH_MAX + 2];  // 1 for newline, 1 for zero
    do {
      printf("> ");
      char *s = fgets(line, LINE_LENGTH_MAX + 2, stdin);
      assert(s);
    } while (strlen(line) == 1);  // for newline

#define RESPOND(msg, ...) \
  printf("[Game] " msg "\n", ##__VA_ARGS__); \
  continue;

    if (strlen(line) == LINE_LENGTH_MAX + 1) {
      // clear input buffer
      int c;
      while ((c = getchar()) != '\n' && c != EOF);

      RESPOND("input line too long");
    }

    size_t offset = 0, len = strlen(line);
    // trim newline
    line[len - 1] = '\0';
    len -= 1;

    char command[LINE_LENGTH_MAX + 1];
    int s = sscanf_s(line, "%s", command, LINE_LENGTH_MAX + 1);
    if (s == EOF) {
      RESPOND("cannot parse input line");
    }
    offset += strlen(command);

    if (!strcmp(command, "?")) {
      printf(
          "q\tquit\n"
          "n <i>\tforward i tick(s), default {i = 1}\n"
          "b <x>\tmove bracket to x\n"
          "--- end ---\n"
      );
      continue;
    }

    if (!strcmp(command, "q"))
      break;

    if (!strcmp(command, "b")) {
      double bracket_x;
      if (offset >= len || sscanf_s(line + offset, " %lf", &bracket_x) == EOF) {
        RESPOND("cannot parse <x> for {command = b}");
      }
      Game.SetBracketX(game, bracket_x);
      continue;
    }

    if (!strcmp(command, "n")) {
      unsigned nb_tick;
      if (offset >= len) {
        nb_tick = 1;
      } else if (sscanf_s(line + offset, " %u", &nb_tick) == EOF) {
        RESPOND("cannot parse <i> for {command = n}");
      }
      for (int i = 0; i < nb_tick && !Game.IsOver(game); i += 1)
        Game.NextTick(game);
      if (Game.IsOver(game)) {
        Game.Draw(game, view, &TextView);
        break;
      }
      continue;
    }

    RESPOND("unknown command: %s", command);
  }

  if (Game.IsOver(game))
    printf("Game Over\n");

  DropGame(game);
  return 0;
}