#ifndef __JEWELBOT_TYPES_H__
#define __JEWELBOT_TYPES_H__

#include "ble_gap.h"
#include <stdbool.h>
#include <stdint.h>

typedef uint8_t jb_guid_t[8];
typedef uint8_t jwb_color_t; // color index
typedef int32_t time_t;

typedef struct {
  uint8_t magic[6]; // JWLB01
  jb_guid_t user_device;
  uint8_t num_friends;
  time_t last_sync;
} jwb_persist_store_t;

typedef struct {
  jb_guid_t guid;
  jwb_color_t color;
  time_t timestamp;
} friend_t;
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_t;

typedef union color_rgb_t {
  uint32_t word;
  uint8_t byte[3];
  rgb_t color;
} color_rgb_t;

typedef struct {
  uint8_t led;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  time_t timestamp;
} led_cmd_t;

typedef struct {
  uint8_t duration;
  uint8_t amplitude;
  uint8_t waveform;
  time_t timestamp;
} haptic_cmd_t;

typedef struct {
  uint8_t effect_id;
  uint8_t duration;
} effect_cmd_t;

typedef struct {
  ble_gap_addr_t address;
  uint8_t color; // index of color
} color_friends_t;

#define MAX_NUM_OF_FRIENDS 16

#pragma pack(1)
typedef struct {
  color_friends_t friends[MAX_NUM_OF_FRIENDS];
  uint8_t num_of_friends;
  uint8_t __padding[3];
} friends_list_t;
#pragma pack()

#endif
