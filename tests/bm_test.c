#include "osup/osup_beatmap.h"

int main() {
  osup_bm map = {0};
#ifndef OSUP_NO_LOGGING
  osup_set_default_error_callback();
#endif
  osup_bool ret = osup_beatmap_load(&map, "res/unshakable.osu", OSUP_PARSE_ALL);
  osup_beatmap_free(&map);
  return !ret;
}

