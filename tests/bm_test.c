#include "osup/osup_beatmap.h"

int main() {
  osup_bm map = {0};
  osup_beatmap_load(&map, "res/magma.osu");

  return 0;
}

