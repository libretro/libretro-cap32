/* Copyright (C) 2018
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>

#include <libretro.h>
#include <libretro-core.h>

#include "retro_snd.h"

#define BUFSIZE 44100 / 50
#define BYTESPERSAMPLE 2
#define AMP_MUL 64

typedef struct {
   char ChunkID[4];
   uint32_t ChunkSize;
   char Format[4];

   char Subchunk1ID[4];
   uint32_t Subchunk1Size;
   uint16_t AudioFormat;
   uint16_t NumChannels;
   uint32_t SampleRate;
   uint32_t ByteRate;
   uint16_t BlockAlign;
   uint16_t BitsPerSample;

   char Subchunk2ID[4];
   uint32_t Subchunk2Size;
} WAVhead;

typedef struct {
   WAVhead head;
   void* rawsamples;
   unsigned int sample_pos;
   unsigned int samples_tot;

   unsigned int sample_rate;
   unsigned int bytes_per_sample;
   audio_status_t state;
   bool ready_to_play;
} retro_guisnd_t;

static int16_t* snd_buffer;

#ifndef MSB_FIRST
#include "snd/motor.h"
#include "snd/seek_drive.h"
#include "snd/read_drive.h"
#else
#include "snd/motor_be.h"
#include "snd/seek_drive_be.h"
#include "snd/read_drive_be.h"
#endif

retro_guisnd_t sounds[SND_LAST];


/**
 * sound_free:
 * free allocated samples memory
 **/
void sound_free(retro_guisnd_t* snd){
   snd->ready_to_play = false;
   if(snd->rawsamples)
      free(snd->rawsamples);
   snd->rawsamples = NULL;
   snd->samples_tot = 0;
   snd->sample_pos=0;
   snd->state = ST_OFF;
}


/**
 * sound_load:
 * @snd: sample struct to load
 * @buffer: original wav buffer
 * @buffer_size: wav buffer size
 *
 * loads and allocate memory from a wav file.
 * warning! audio file requirements: 16bits/mono
 *
 * return false if cannot allocate memory or invalid wav file is used.
 */
bool sound_load(retro_guisnd_t* snd, const void* buffer, const int buffer_size) {
   //LOGI("wav_size: %d", buffer_size);
   memcpy(&snd->head, buffer, 44);

   if (snd->head.NumChannels!=1 || snd->head.BitsPerSample!=16){
      LOGI(" - Incompatible audio type (%dch/%dbits) (1ch/16bits req) \n", snd->head.NumChannels, snd->head.BitsPerSample);
      return false;
   }

   //LOGI(" | sizeChunk: %d | channels: %d | BPS: %d\n", snd->head.Subchunk2Size, snd->head.NumChannels, snd->head.BitsPerSample);

   snd->samples_tot      = snd->head.Subchunk2Size / BYTESPERSAMPLE;
   snd->rawsamples       = malloc(snd->head.Subchunk2Size);
   if(!snd->rawsamples)
      return false;

   memcpy(snd->rawsamples, (uint8_t*)buffer + 44, buffer_size - 44);

   snd->state = ST_OFF;
   snd->sample_pos=0;
   snd->ready_to_play = true;

   return true;
}


/**
 * init_retro_snd:
 * @pbuffer: emulator sound buffer, to be used by the mixer
 *
 * load internal sounds and prepare mixer to be used
 * return false if cannot allocate memory or invalid wav file is used.
 */
bool init_retro_snd(int16_t* pbuffer){
   memset(sounds, 0, sizeof(sounds));

   if(!sound_load(&sounds[SND_FDCMOTOR], motor, motor_size))
      return false;
   if(!sound_load(&sounds[SND_FDCREAD], read_drive, read_size))
      return false;
   if(!sound_load(&sounds[SND_FDCSEEK], seek_drive, seek_size))
      return false;

   snd_buffer = pbuffer;

   return true;
}


/**
 * free_retro_snd:
 * free internal sounds and disables mixer
 */
void free_retro_snd(){
   sound_free(&sounds[SND_FDCMOTOR]);
   sound_free(&sounds[SND_FDCREAD]);
   sound_free(&sounds[SND_FDCSEEK]);
}


/**
 * sound_stop:
 * @snd: sample struct to be freed
 * free internal sounds and disables mixer
 */
void sound_stop(retro_guisnd_t* snd) {
   snd->sample_pos = 0;
   snd->state = ST_OFF;
}


/**
 * mix_audio:
 * @snd: sample struct to mix in emulator buffer
 *
 * this is a very simple mixer loop that just send a full BUFSIZE
 * mix 16bits / mono (internal audio) into a 16bits stereo buffer (emulator)
 */
static void mix_audio(retro_guisnd_t* snd)
{
   if (snd->sample_pos + BUFSIZE > snd->samples_tot) {
      // exits if no loop sound...
      if(snd->state == ST_ON) {
         sound_stop(snd);
         return;
      }
      snd->sample_pos = 0;
   }


   // prepare loop vars
   int16_t* samples = snd_buffer;
   int16_t* rawsamples16 = (int16_t*) ((uint8_t*) snd->rawsamples + (BYTESPERSAMPLE * snd->sample_pos));
   unsigned i = BUFSIZE;

   while (i--)
   {
      *samples += *rawsamples16;
      *(samples + 1) += *rawsamples16;

      // prepare next loop
      rawsamples16++;
      samples += 2;
   }

   snd->sample_pos     += BUFSIZE;
}


/**
 * retro_snd_mixer:
 * mixes each sound (active or looped) into emulator buffer
 */
void retro_snd_mixer() {
   int n;
   for(n = 0; n < SND_LAST; n++) {
      if (sounds[n].state != ST_OFF) {
         mix_audio(&sounds[n]);
      }
   }
}


/**
 * retro_snd_cmd:
 * @snd_type: select sound, see retro_samples_snd
 * @new_status: set the new status (off, active or looped), see audio_status_t
 *
 * sets a new sound command status.
 */
void retro_snd_cmd(int snd_type, audio_status_t new_status) {
   if((snd_type >= SND_LAST)||(!sounds[snd_type].ready_to_play))
      return;

   sounds[snd_type].state = new_status;
   if(new_status == ST_OFF)
      sounds[snd_type].sample_pos = 0;
}
