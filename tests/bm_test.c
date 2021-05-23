#include "osup/osup_beatmap.h"

int main() {
  osup_bm map = {0};
  return !osup_beatmap_load(&map, "res/test.osu");
}

