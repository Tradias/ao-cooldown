/**
 * Copyright (c) 2024 Dennis Hezel
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Input {
  float attack;
  float recharge;
  int32_t cycle;
};

typedef float (*formula)(struct Input, int32_t);

float as_cooldown(struct Input input, int32_t skill) {
  const float as_skill_multiplier = 3.f / 100.f;
  return (input.recharge * 40.f) - (skill * as_skill_multiplier) + input.attack;
}

float burst_cooldown(struct Input input, int32_t skill) {
  return (input.recharge + (input.cycle / 2000.f) - (skill / 500.f)) * 20.f +
         input.attack;
}

float fast_attack_cooldown(struct Input input, int32_t skill) {
  return (input.attack * 16.f) - (skill / 100.f);
}

float fling_shot_cooldown(struct Input input, int32_t skill) {
  return (input.attack * 16.f) - (skill / 100.f);
}

float full_auto_cooldown(struct Input input, int32_t skill) {
  return (input.recharge * 40.f) + (input.cycle / 100.f) - (skill / 25.f) +
         input.attack;
}

void print_cooldowns(struct Input input, int32_t cap, formula formula) {
  float previous_cd = 0;
  float current_cd = 0;
  int32_t skill = 0;
  do {
    ++skill;
    current_cd = formula(input, skill);
    if (floor(previous_cd) > floor(current_cd) && current_cd > cap) {
      printf("%2is | %i\n", (int)current_cd, skill);
    }
    previous_cd = current_cd;
  } while (previous_cd > cap);
}

int main(int argc, const char **argv) {
  if (argc < 4) {
    printf(
        "usage: as|burst|fast|fling|fa attack recharge [burst cycle|fa cycle]");
    return 1;
  }
  struct Input input = {
      .attack = (float)atof(argv[2]),
      .recharge = (float)atof(argv[3]),
      .cycle = argc > 4 ? atoi(argv[4]) : 1000,
  };
  struct Option {
    const char *name;
    int32_t cap;
    formula formula;
  };
  struct Option options[] = {{"as", 10, as_cooldown},
                             {"burst", 8, burst_cooldown},
                             {"fast", 5, fast_attack_cooldown},
                             {"fling", 5, fling_shot_cooldown},
                             {"fa", 10, full_auto_cooldown}};
  for (size_t i = 0; i != sizeof(options) / sizeof(options[0]); ++i) {
    struct Option option = options[i];
    if (strcmp(argv[1], option.name) == 0) {
      const int32_t cap = (int32_t)floor(option.cap + input.attack);
      print_cooldowns(input, cap, option.formula);
    }
  }
}
