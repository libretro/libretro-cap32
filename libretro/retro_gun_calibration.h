#ifndef RETRO_GUN_CALIBRATION_H
#define RETRO_GUN_CALIBRATION_H
#include <stdbool.h>
#include <libretro.h>
void gun_calibration_options(struct retro_core_option_v2_definition *defs, struct retro_variable *vars);
void gun_calibration_sync_options(void);
void gun_calibration_read_options(void);
void gun_calibration_load(const char *content);
void gun_calibration_unload(void);
void gun_calibration_option(const char *value);
bool gun_calibration_active(void);
void gun_calibration_frame(void);
void gun_calibration_restore(void);
bool gun_calibration_apply(unsigned port, int *x, int *y);
#endif
