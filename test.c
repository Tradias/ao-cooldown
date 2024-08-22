/**
 * Copyright (c) 2024 Dennis Hezel
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define test_assert(x)                                                         \
  do {                                                                         \
    if (!(x)) {                                                                \
      printf("assertion failed: %s", #x);                                      \
      exit(1);                                                                 \
    }                                                                          \
    break;                                                                     \
  } while (1)

typedef int32_t (*test_action)(int32_t, int32_t);

void *copy_n(const void *source, size_t count, void *dest) {
  memcpy(dest, source, count);
  return (char*)dest + count;
}

char *build_command(const char *args) {
  size_t arg_length = strlen(args);
  char *command = malloc(sizeof(EXECUTABLE_PATH) + arg_length + 1);
  char *c = command;
  c = copy_n(EXECUTABLE_PATH, sizeof(EXECUTABLE_PATH) - 1, c);
  *c++ = ' ';
  c = copy_n(args, arg_length, c);
  *c = '\0';
  return command;
}

int32_t run(test_action action, const char *args) {
  int32_t result = 0;

  char *command = build_command(args);
  FILE *fp = _popen(command, "r");
  free(command);

  test_assert(fp);

  char buf[128];
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    int32_t cooldown = atoi(buf + (buf[0] == ' ' ? 1 : 0));
    int32_t skill = atoi(buf + 6);
    if (cooldown != 0 && skill != 0) {
      result += action(cooldown, skill);
    }
  }

  test_assert(_pclose(fp) == 0);

  return result;
}

int32_t test_tigress_as(int32_t cooldown, int32_t skill) {
  if (cooldown == 12 && skill == 1481) {
    return 1;
  }
  if (cooldown == 11 && skill == 1514) {
    return 1;
  }
  return 0;
}

int32_t test_envoy_to_chaos_burst(int32_t cooldown, int32_t skill) {
  if (cooldown == 9 && skill == 1381) {
    return 1;
  }
  return 0;
}

int32_t test_bigburger_fa(int32_t cooldown, int32_t skill) {
  if (cooldown == 11 && skill == 1583) {
    return 1;
  }
  return 0;
}

int32_t test_hellfury_fa(int32_t cooldown, int32_t skill) {
  if (cooldown == 11 && skill == 976) {
    return 1;
  }
  return 0;
}

int main(int argc, const char **argv) {
  test_assert(2 == run(test_tigress_as, "as 1.4 1.4"));
  test_assert(1 == run(test_envoy_to_chaos_burst, "burst 1.2 1.2 4000"));
  test_assert(1 == run(test_bigburger_fa, "fa 1.3 1 3400"));
  test_assert(1 == run(test_hellfury_fa, "fa 1 1"));
}
