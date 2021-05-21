#ifndef OSUP_BEATMAP_H
#define OSUP_BEATMAP_H

#define OSUP_API

#include <stdio.h>

#include "osup_types.h"

typedef enum {
  OSUP_SAMPLESET_DEFAULT = 0,
  OSUP_SAMPLESET_NORMAL = 1,
  OSUP_SAMPLESET_SOFT = 2,
  OSUP_SAMPLESET_DRUM = 3,
  OSUP_SAMPLESET_INVALID = 4
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
  char* AudioFilename;
  osup_int AudioLeadIn;
  char* AudioHash;
  osup_int PreviewTime;
  osup_countdown_speed Countdown;
  osup_sampleset SampleSet;
  osup_decimal StackLeniency;
  osup_gamemode Mode;
  osup_bool LetterboxInBreaks;
  osup_bool StoryFireInFront;
  osup_bool UseSkinSprites;
  osup_bool AlwaysShowPlayfield;
  osup_overlaypos OverlayPosition;
  char* SkinPreference;
  osup_bool EpilepsyWarning;
  osup_int CountdownOffset;
  osup_bool SpecialStyle;
  osup_bool WidescreenStoryboard;
  osup_bool SamplesMatchPlaybackRate;
} osup_bm_general;

typedef struct {
  struct {
    osup_int* elements;
    size_t count;
  } Bookmarks;
  osup_decimal DistanceSpacing;
  osup_decimal BeatDivisor;
  osup_int GridSize;
  osup_decimal TimelineZoom;
} osup_bm_editor;

typedef struct {
  char* Title;
  char* TitleUnicode;
  char* Artist;
  char* ArtistUnicode;
  char* Creator;
  char* Version;
  char* Source;
  struct {
    char** elements;
    size_t count;
  } Tags;
  osup_int BeatmapID;
  osup_int BeatmapSetID;
} osup_bm_metadata;

typedef struct {
  osup_decimal HPDrainRate;
  osup_decimal CircleSize;
  osup_decimal OverallDifficulty;
  osup_decimal ApproachRate;
  osup_decimal SliderMultiplier;
  osup_decimal SliderTickRate;
} osup_bm_difficulty;

typedef enum {
  OSUP_EVENT_TYPE_BACKGROUND,
  OSUP_EVENT_TYPE_VIDEO,
  OSUP_EVENT_TYPE_BREAK
} osup_event_type;

typedef struct {
  const char* filename;
  osup_int xOffset;
  osup_int yOffset;
} osup_bg_event_params;

typedef struct {
  const char* filename;
  osup_int xOffset;
  osup_int yOffset;
} osup_video_event_params;

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
  struct {
    osup_rgb* elements;
    size_t count;
  } Combo;
  osup_rgb SliderTrackOverride;
  osup_rgb SliderBorder;
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
    osup_vec2 elements;
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
  };
} osup_hitobject;

typedef struct {
  osup_hitobject* elements;
  size_t count;
} osup_bm_hitobjects;

typedef struct {
  osup_bm_general General;
  osup_bm_editor Editor;
  osup_bm_metadata Metadata;
  osup_bm_difficulty Difficulty;
  osup_bm_events Events;
  osup_bm_timingpoints TimingPoints;
  union {
    osup_bm_colours Colours;
    osup_bm_colors Colors;
  };
  osup_bm_hitobjects HitObjects;
} osup_bm;

typedef const char* (*osup_bm_callback)(void*);

OSUP_API void osup_beatmap_init(osup_bm* map);

OSUP_API void osup_beatmap_load(osup_bm* map, const char* file);
OSUP_API void osup_beatmap_load_string(osup_bm* map, const char* string);
OSUP_API void osup_beatmap_load_callbacks(osup_bm* map,
                                          osup_bm_callback callback, void* ptr);
OSUP_API void osup_beatmap_load_stream(osup_bm* map, FILE* stream);

#endif

