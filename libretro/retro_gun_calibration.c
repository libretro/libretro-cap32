#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libretro.h>
#include <libretro-core.h>
#include <streams/file_stream.h>
#include <file/file_path.h>
#include "cap32.h"
#include "gfx/software.h"
#include "gfx/video.h"
#include "retro_gun.h"
#include "retro_gun_calibration.h"

extern retro_environment_t environ_cb;
extern retro_input_state_t input_state_cb;
extern const char *retro_save_directory;
extern uint32_t *video_buffer;

/* One shader transform is shared by both frontend lightgun ports. */
typedef struct {
   int valid, width, height;
   double m[6];
} gun_calibration;
static gun_calibration profile = {1, 0, 0, {1, 0, 0, 0, 1, 0}}, candidate;
extern void gun_calibration_refresh_options(void);
static const double targets[4][2] = {
   {-.70, -.70}, {.70, -.70}, {-.70, .70}, {.70, .70}
};
static double samples[3][2];
static int active_port = -1, step, settle;
static bool loaded, trigger_down, middle_down, right_down, exiting, frame_saved;
static int session_width, session_height, session_depth;
static char storage_path[4096], last_option[32];
static const char *notice;
static void *saved_frame;
static size_t saved_bytes;

static bool valid_matrix(const gun_calibration *c)
{
   unsigned i;
   double det = c->m[0]*c->m[4] - c->m[1]*c->m[3];
   for (i=0; i<6; i++)
      if (!isfinite(c->m[i]) || fabs(c->m[i]) > 10.0)
         return false;
   return c->m[0] > 0 && c->m[4] > 0 && fabs(det) >= .009999 && fabs(det) <= 100.0;
}

/* Fit each axis independently: output = scale * input + offset. */
static bool solve(void)
{
   unsigned axis, point;
   memset(&candidate, 0, sizeof(candidate));
   for (axis=0; axis<2; axis++) {
      double sum = 0, target_sum = 0, square_sum = 0, product_sum = 0;
      double denominator, scale;
      for (point=0; point<3; point++) {
         double input = samples[point][axis], target = targets[point][axis];
         sum += input;
         target_sum += target;
         square_sum += input*input;
         product_sum += input*target;
      }
      denominator = 3*square_sum-sum*sum;
      if (!isfinite(denominator) || denominator < .05) return false;
      scale = (3*product_sum-sum*target_sum)/denominator;
      if (scale <= 0) return false;
      candidate.m[axis ? 4 : 0] = scale;
      candidate.m[axis ? 5 : 2] = (target_sum-scale*sum)/3;
   }
   candidate.width = retro_video.screen_render_width;
   candidate.height = retro_video.screen_render_height;
   candidate.valid = 1;
   return valid_matrix(&candidate);
}

static void identity(void)
{
   memset(&profile, 0, sizeof(profile));
   profile.valid = 1;
   profile.m[0] = profile.m[4] = 1.0;
}

static bool save_profiles(void)
{
   char text[512];
   int n = snprintf(text, sizeof(text),
      "CAP32_GUN_CAL_2\n1 %d %d %.17g %.17g %.17g %.17g %.17g %.17g\n",
      profile.width, profile.height, profile.m[0], profile.m[1], profile.m[2],
      profile.m[3], profile.m[4], profile.m[5]);
   return storage_path[0] && n > 0 && (size_t)n < sizeof(text) &&
      filestream_write_file(storage_path, text, n);
}

/* Keep the measured value as an exact selectable entry alongside the manual
 * steps. SET_VARIABLE alone cannot select values absent from the definition. */
static const unsigned coefficient[4] = {0, 2, 4, 5};
static const char *value_keys[4] = {
   "cap32_lightgun_scale_x", "cap32_lightgun_offset_x",
   "cap32_lightgun_scale_y", "cap32_lightgun_offset_y"
};
static char values[4][128][32], labels[4][128][32], legacy[4][4096];
static char displayed[4][32];
static double frontend_values[4];
static bool frontend_known[4];

void gun_calibration_options(struct retro_core_option_v2_definition *defs,
      struct retro_variable *vars)
{
   unsigned i, j, k, n;
   for (i=0; i<4; i++) {
      struct retro_core_option_v2_definition *def = defs;
      struct retro_variable *var = vars;
      bool scale = i == 0 || i == 2, offset = i == 1 || i == 3;
      double choices[127], current = profile.m[coefficient[i]];
      n = 0;
      /* Fixed ranges fit the libretro limit of 127 selectable values, with
       * one additional entry reserved for the exact calibration result. */
      for (j=0; j <= (scale ? 124 : 100); j++)
         choices[n++] = scale ? (4+(int)j)/40.0 : ((int)j-50)/100.0;
      for (j=0; j<n && fabs(choices[j]-current) > 1e-9; j++);
      if (j == n) choices[n++] = current;
      else choices[j] = current;
      /* Insert the non-rounded calibration result in numerical order. */
      for (j=1; j<n; j++) {
         double v = choices[j];
         for (k=j; k && choices[k-1] > v; k--) choices[k] = choices[k-1];
         choices[k] = v;
      }
      while (def->key && strcmp(def->key, value_keys[i])) def++;
      while (var->key && strcmp(var->key, value_keys[i])) var++;
      if (!def->key || !var->key) continue;
      memset(def->values, 0, sizeof(def->values));
      snprintf(displayed[i], sizeof(displayed[i]), "%.9g", current);
      snprintf(legacy[i], sizeof(legacy[i]), "Light Gun > %s; %s", def->desc, displayed[i]);
      for (j=0; j<n; j++) {
         snprintf(values[i][j], sizeof(values[i][j]), "%.9g", choices[j]);
         snprintf(labels[i][j], sizeof(labels[i][j]), offset ? "%.3f%%" : "%.6g",
               choices[j] * (offset ? 50.0 : 1.0));
         def->values[j].value = values[i][j];
         def->values[j].label = labels[i][j];
         if (strcmp(values[i][j], displayed[i])) {
            size_t used = strlen(legacy[i]);
            snprintf(legacy[i]+used, sizeof(legacy[i])-used, "|%s", values[i][j]);
         }
      }
      def->default_value = displayed[i];
      var->value = legacy[i];
   }
}

void gun_calibration_sync_options(void)
{
   unsigned i;
   for (i=0; i<4; i++) {
      struct retro_variable var = {value_keys[i], displayed[i]};
      environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, &var);
      var.value = NULL;
      frontend_known[i] = environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) && var.value;
      if (frontend_known[i]) frontend_values[i] = strtod(var.value, NULL);
   }
}

void gun_calibration_read_options(void)
{
   gun_calibration next = profile;
   unsigned i;
   bool changed = false;
   if (!loaded) return;
   for (i=0; i<4; i++) {
      struct retro_variable var = {value_keys[i], NULL};
      char *end;
      double value;
      if (!environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE, &var) || !var.value) continue;
      value = strtod(var.value, &end);
      if (*end || !isfinite(value)) continue;
      if (frontend_known[i] && value != frontend_values[i]) {
         next.m[coefficient[i]] = value;
         changed = true;
      }
      frontend_values[i] = value;
      frontend_known[i] = true;
   }
   if (!changed) return;
   if (valid_matrix(&next)) {
      profile = next;
      active_port = -1;
      if (!save_profiles()) retro_message("Gun settings active for this session; save failed");
   } else retro_message("Invalid gun transform; previous values restored");
   gun_calibration_refresh_options();
   gun_calibration_sync_options();
}

static void finish(void)
{
   struct retro_variable variable = {"cap32_lightgun_calibration", "off"};
   active_port = -1;
   if (environ_cb(RETRO_ENVIRONMENT_SET_VARIABLE, &variable))
      strcpy(last_option, "off");
}

void gun_calibration_unload(void)
{
   active_port = -1;
   frame_saved = exiting = false;
   loaded = false;
   storage_path[0] = 0;
   identity();
   free(saved_frame);
   saved_frame = NULL;
   saved_bytes = 0;
}

void gun_calibration_load(const char *content)
{
   char text[1024] = {0};
   const char *name = content ? path_basename(content) : "";
   uint32_t hash = 2166136261u;
   RFILE *file;
   size_t i;
   gun_calibration_unload();
   loaded = true;
   /* Match the content name, including a playlist name, across disk swaps
    * and directory moves. No writable content directory is required. */
   for (i=0; name[i]; i++) hash = (hash ^ (unsigned char)name[i])*16777619u;
   if (!name[0] || !retro_save_directory || !retro_save_directory[0]) goto ready;
   if (snprintf(storage_path, sizeof(storage_path), "%s/cap32-gun-%08x.cal",
         retro_save_directory, hash) >= sizeof(storage_path)) {
      storage_path[0] = 0;
      goto ready;
   }
   file = filestream_open(storage_path, RETRO_VFS_FILE_ACCESS_READ, RETRO_VFS_FILE_ACCESS_HINT_NONE);
   if (!file) goto ready;
   filestream_read(file, text, sizeof(text)-1);
   filestream_close(file);
   {
      gun_calibration old[2] = {{0}};
      int count = sscanf(text, "CAP32_GUN_CAL_2\n%d %d %d %lf %lf %lf %lf %lf %lf",
         &profile.valid, &profile.width, &profile.height,
         &profile.m[0], &profile.m[1], &profile.m[2], &profile.m[3], &profile.m[4], &profile.m[5]);
      if (count != 9) {
         identity();
         /* Migrate the previous per-port format, preferring Gun 1. */
         if (sscanf(text, "CAP32_GUN_CAL_1\n%d %d %d %lf %lf %lf %lf %lf %lf\n%d %d %d %lf %lf %lf %lf %lf %lf",
            &old[0].valid, &old[0].width, &old[0].height,
            &old[0].m[0], &old[0].m[1], &old[0].m[2], &old[0].m[3], &old[0].m[4], &old[0].m[5],
            &old[1].valid, &old[1].width, &old[1].height,
            &old[1].m[0], &old[1].m[1], &old[1].m[2], &old[1].m[3], &old[1].m[4], &old[1].m[5]) == 18) {
            for (i=0; i<2; i++)
               if (old[i].valid == 1 && valid_matrix(&old[i])) { profile = old[i]; break; }
         }
      }
      if (profile.valid != 1 || !valid_matrix(&profile)) identity();
   }
ready:
   /* Ignore cross-axis terms from earlier calibration files. */
   profile.m[1] = profile.m[3] = 0;
   gun_calibration_refresh_options();
   gun_calibration_sync_options();
}

void gun_calibration_option(const char *value)
{
   int port;
   size_t bytes;
   void *buffer;
   if (!value) value = "off";
   if (!strcmp(value, last_option)) return;
   snprintf(last_option, sizeof(last_option), "%s", value);
   if (!loaded) return;
   if (!strcmp(value, "off")) { active_port = -1; return; }
   port = lightgun_active(0) ? 0 : 1;
   if (!strcmp(value, "reset")) {
      identity();
      gun_calibration_refresh_options();
      gun_calibration_sync_options();
      retro_message(save_profiles() ? "Gun calibration reset" : "Calibration reset for this session; save failed");
      finish();
      return;
   }
   if (strcmp(value, "start")) return;
   if (!lightgun_active(port)) {
      retro_message("Select Amstrad Lightgun and enable Gunstick first");
      finish();
      return;
   }
   bytes = (size_t)EMULATION_SCREEN_WIDTH * EMULATION_SCREEN_HEIGHT * retro_video.pitch;
   buffer = realloc(saved_frame, bytes);
   if (!buffer) { retro_message("Cannot allocate calibration screen"); finish(); return; }
   saved_frame = buffer;
   saved_bytes = bytes;
   active_port = port;
   exiting = frame_saved = false;
   session_width = retro_video.screen_render_width;
   session_height = retro_video.screen_render_height;
   session_depth = retro_video.depth;
   step = 0;
   settle = 20;
   notice = "Shoot the centre of each target";
   trigger_down = middle_down = right_down = true;
}

bool gun_calibration_active(void) { return active_port >= 0; }

bool gun_calibration_apply(unsigned port, int *x, int *y)
{
   double nx, ny, rx, ry;
   const gun_calibration *c;
   if (port >= 2) return false;
   c = &profile;
   if (!c->valid)
      return true;
   rx = *x/32767.0; ry = *y/32767.0;
   nx = c->m[0]*rx+c->m[2];
   ny = c->m[4]*ry+c->m[5];
   if (!isfinite(nx) || !isfinite(ny) || fabs(nx)>1.0 || fabs(ny)>1.0) return false;
   *x = (int)lround(nx*32767.0); *y = (int)lround(ny*32767.0);
   return true;
}

void gun_calibration_frame(void)
{
   unsigned port = active_port;
   bool trigger = input_state_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_TRIGGER);
   bool middle = input_state_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_MIDDLE);
   bool right = input_state_cb(port, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT);
   int x = input_state_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_X);
   int y = input_state_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_SCREEN_Y);
   int ox = retro_video.screen_crop ? EMULATION_CROP : 0;
   int w = retro_video.screen_render_width, h = retro_video.screen_render_height;
   uint32_t white = retro_video.depth == DEPTH_24BPP ? 0xffffff : 0xffff;
   char title[64];
   frame_saved = false;
   if (!lightgun_active(port) || w != session_width || h != session_height ||
       retro_video.depth != session_depth) {
      retro_message("Calibration cancelled: video or input changed");
      finish(); return;
   }
   /* Wait for release before returning to the game/keyboard toggle. */
   if (exiting) {
      if (!trigger && !middle && !right) finish();
      return;
   }
   if (settle) settle--;
   if (middle && !middle_down) { exiting = true; return; }
   if (right && !right_down) { step = 0; settle = 20; notice = "Shoot the centre of each target"; }
   if (trigger && !trigger_down && !settle && x != -32768 && y != -32768 &&
       !input_state_cb(port, RETRO_DEVICE_LIGHTGUN, 0, RETRO_DEVICE_ID_LIGHTGUN_IS_OFFSCREEN)) {
      double rx = x/32767.0, ry = y/32767.0;
      if (step < 3) {
         samples[step][0] = rx; samples[step][1] = ry;
         step++;
         if (step == 3 && !solve()) {
            step = 0; notice = "Invalid points. Please try again";
         } else if (step == 3) notice = "Shoot check target to save";
         settle = 20;
      } else {
         double dx = candidate.m[0]*rx+candidate.m[2]-.7;
         double dy = candidate.m[4]*ry+candidate.m[5]-.7;
         if (dx*dx+dy*dy < .0036) {
            profile = candidate;
            gun_calibration_refresh_options();
            gun_calibration_sync_options();
            retro_message(save_profiles() ? "Gun calibration saved" : "Calibration active for this session; save failed");
            exiting = true; return;
         }
         notice = "Check missed. Retry or right-click";
      }
   }
   trigger_down = trigger; middle_down = middle; right_down = right;
   memcpy(saved_frame, video_buffer, saved_bytes);
   frame_saved = true;
   memset(video_buffer, 0, saved_bytes);
   snprintf(title, sizeof(title), "GUN %u - %s %d/4", port+1, step==3 ? "CHECK" : "TARGET", step+1);
   draw_text(video_buffer, ox+32, h/2-24, title, white);
   draw_text(video_buffer, ox+32, h/2, notice, white);
   draw_text(video_buffer, ox+32, h/2+24, "Right: repeat  Middle: cancel", white);
   x = ox + (int)((targets[step][0]+1)*.5*(w-1));
   y = (int)((targets[step][1]+1)*.5*(h-1));
   draw_rect(video_buffer, x-14, y-1, 28, 2, white);
   draw_rect(video_buffer, x-2, y-10, 4, 20, white);
}

void gun_calibration_restore(void)
{
   if (frame_saved) memcpy(video_buffer, saved_frame, saved_bytes);
   frame_saved = false;
}
