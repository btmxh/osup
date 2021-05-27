#ifndef OSUP_BEATMAP_H
#define OSUP_BEATMAP_H

/*********
 * USAGE *
 *********/
#if 0

int main() {
  /* remember to initialize everything to 0, the program will crash if you gave
   * it garbage data */
  osup_bm map{};
  osup_beatmap_load(&map, "/path/to/beatmap");
  printf(map.metadata.title);
  osup_beatmap_free(&map);
  return 0;
}

#endif

#define OSUP_API

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "osup_common.h"

#define OSUP_FLAG(x) (1 << (x))
#define OSUP_IS_HITCIRCLE(type) (type & OSUP_FLAG(0))
#define OSUP_IS_SLIDER(type) (type & OSUP_FLAG(1))
#define OSUP_IS_SPINNER(type) (type & OSUP_FLAG(3))
#define OSUP_IS_MANIA_HOLD(type) (type & OSUP_FLAG(7))

#define OSUP_PARSE_GENERAL OSUP_FLAG(0)
#define OSUP_PARSE_EDITOR OSUP_FLAG(1)
#define OSUP_PARSE_METADATA OSUP_FLAG(2)
#define OSUP_PARSE_DIFFICULTY OSUP_FLAG(3)
#define OSUP_PARSE_TIMING_POINTS OSUP_FLAG(4)
#define OSUP_PARSE_EVENTS OSUP_FLAG(5)
#define OSUP_PARSE_COLORS OSUP_FLAG(6)
#define OSUP_PARSE_HIT_OBJECTS OSUP_FLAG(7)
#define OSUP_PARSE_ALL                                                    \
  (OSUP_PARSE_GENERAL | OSUP_PARSE_EDITOR | OSUP_PARSE_METADATA |         \
   OSUP_PARSE_DIFFICULTY | OSUP_PARSE_TIMING_POINTS | OSUP_PARSE_EVENTS | \
   OSUP_PARSE_COLORS | OSUP_PARSE_HIT_OBJECTS)

typedef enum {
  OSUP_SAMPLESET_DEFAULT = 0,
  OSUP_SAMPLESET_NORMAL = 1,
  OSUP_SAMPLESET_SOFT = 2,
  OSUP_SAMPLESET_DRUM = 3,
} osup_sampleset;

typedef enum {
  OSUP_OVERLAYPOS_NOCHANGE,
  OSUP_OVERLAYPOS_BELOW,
  OSUP_OVERLAYPOS_ABOVE
} osup_overlaypos;

typedef enum {
  OSUP_COUNTDOWN_SPEED_NONE,
  OSUP_COUNTDOWN_SPEED_NORMAL,
  OSUP_COUNTDOWN_SPEED_HALF,
  OSUP_COUNTDOWN_SPEED_DOUBLE
} osup_countdown_speed;

typedef struct {
  char* audioFilename;
  osup_int audioLeadIn;
  char* audioHash;
  osup_int previewTime;
  osup_countdown_speed countdown;
  osup_sampleset sampleSet;
  osup_decimal stackLeniency;
  osup_gamemode mode;
  osup_bool letterboxInBreaks;
  osup_bool storyFireInFront;
  osup_bool useSkinSprites;
  osup_bool alwaysShowPlayfield;
  osup_overlaypos overlayPosition;
  char* skinPreference;
  osup_bool epilepsyWarning;
  osup_int countdownOffset;
  osup_bool specialStyle;
  osup_bool widescreenStoryboard;
  osup_bool samplesMatchPlaybackRate;
} osup_bm_general;

typedef struct {
  struct {
    osup_int* elements;
    size_t count;
  } bookmarks;
  osup_decimal distanceSpacing;
  osup_decimal beatDivisor;
  osup_int gridSize;
  osup_decimal timelineZoom;
} osup_bm_editor;

typedef struct {
  char* title;
  char* titleUnicode;
  char* artist;
  char* artistUnicode;
  char* creator;
  char* version;
  char* source;
  struct {
    char** elements;
    size_t count;
  } tags;
  osup_int beatmapID;
  osup_int beatmapSetID;
} osup_bm_metadata;

typedef struct {
  osup_decimal hpDrainRate;
  osup_decimal circleSize;
  osup_decimal overallDifficulty;
  osup_decimal approachRate;
  osup_decimal sliderMultiplier;
  osup_decimal sliderTickRate;
} osup_bm_difficulty;

typedef enum {
  OSUP_EVENT_TYPE_BACKGROUND,
  OSUP_EVENT_TYPE_VIDEO,
  OSUP_EVENT_TYPE_BREAK
} osup_event_type;

typedef struct {
  char* filename;
  osup_int xOffset;
  osup_int yOffset;
} osup_bg_event_params;

/* same structure */
typedef osup_bg_event_params osup_video_event_params;

typedef struct {
  osup_int endTime;
} osup_break_event_params;

typedef struct {
  osup_event_type eventType;
  osup_int startTime;
  union {
    osup_bg_event_params bg;
    osup_video_event_params video;
    osup_break_event_params brk;
  };
} osup_event;

typedef struct {
  osup_event* elements;
  size_t count;
} osup_bm_events;

typedef struct {
  osup_int time;
  osup_decimal beatLength;
  osup_int meter;
  osup_sampleset sampleSet;
  osup_int sampleIndex;
  osup_int volume;
  osup_bool uninherited;
  osup_bitfield8 effects;
} osup_timingpoint;

typedef struct {
  osup_timingpoint* elements;
  size_t count;
} osup_bm_timingpoints;

typedef struct {
  /* i'm not sure if there's a limit for the number of combo colors but it's
   * probably limited to 8 or sth, also this is 0-indexed, so Combo1 will be in
   * combos[0] */
  osup_rgb combos[8];
  /* also 0-indexed */
  size_t maxCombo;
  osup_rgb sliderTrackOverride;
  osup_rgb sliderBorder;
} osup_bm_colors;

typedef osup_bm_colors osup_bm_colours;

typedef struct {
  osup_sampleset normalSet;
  osup_sampleset additionSet;
  osup_int index;
  osup_int volume;
  char* filename;
} osup_hitsample;

typedef enum {
  OSUP_CURVE_BEZIER,
  OSUP_CURVE_CENTRIPETAL_CATMULL_ROM,
  OSUP_CURVE_LINEAR,
  OSUP_CURVE_PERFECT_CIRCLE
} osup_slider_curve;

typedef struct {
  osup_slider_curve curveType;
  struct {
    osup_vec2* elements;
    size_t count;
  } curvePoints;
  osup_int slides;
  osup_decimal length;
  struct {
    osup_int* elements;
    size_t count;
  } edgeSounds;

  struct {
    struct {
      osup_int normalSet;
      osup_int additionSet;
    } * elements;
    size_t count;
  } edgeSets;
} osup_slider_params;

typedef struct {
  osup_int endTime;
} osup_spinner_params;

/* same structure */
typedef osup_spinner_params osup_mania_hold_params;

typedef struct {
  osup_int x;
  osup_int y;
  osup_int time;
  osup_bitfield8 type;
  osup_bitfield8 hitSound;
  osup_hitsample hitSample;
  union {
    osup_slider_params slider;
    osup_spinner_params spinner;
    osup_mania_hold_params maniaHold;
  };
} osup_hitobject;

typedef struct {
  osup_hitobject* elements;
  size_t count;
} osup_bm_hitobjects;

typedef struct {
  osup_bm_general general;
  osup_bm_editor editor;
  osup_bm_metadata metadata;
  osup_bm_difficulty difficulty;
  osup_bm_events events;
  osup_bm_timingpoints timingPoints;
  union {
    osup_bm_colours colours;
    osup_bm_colors colors;
  };
  osup_bm_hitobjects hitObjects;
} osup_bm;

typedef const char* (*osup_bm_callback)(void*);

OSUP_API osup_bool osup_beatmap_load(osup_bm* map, const char* file,
                                     osup_bitfield32 flags);
OSUP_API osup_bool osup_beatmap_load_string(osup_bm* map, const char* string,
                                            osup_bitfield32 flags);
OSUP_API osup_bool osup_beatmap_load_callbacks(osup_bm* map,
                                               osup_bm_callback callback,
                                               void* ptr, osup_bitfield32 flags);
OSUP_API osup_bool osup_beatmap_load_stream(osup_bm* map, FILE* stream,
                                            osup_bitfield32 flags);

OSUP_API void osup_hitobject_free(osup_hitobject* obj);
OSUP_API void osup_event_free(osup_event* event);
OSUP_API void osup_beatmap_free(osup_bm* map);

#ifdef __cplusplus
}
#endif

#endif

