/****************************************************************************
 *  Caprice32 libretro port
 *
 *  Copyright David Colmenero - D_Skywalk (2019-2021)
 *  Copyright Daniel De Matteis (2012-2021)
 *
 *  Redistribution and use of this code or any derivative works are permitted
 *  provided that the following conditions are met:
 *
 *   - Redistributions may not be sold, nor may they be used in a commercial
 *     product or activity.
 *
 *   - Redistributions that are modified from the original source must include the
 *     complete source code, including the source code for all components used by a
 *     binary built from the modified sources. However, as a special exception, the
 *     source code distributed need not include anything that is normally distributed
 *     (in either source or binary form) with the major components (compiler, kernel,
 *     and so on) of the operating system on which the executable runs, unless that
 *     component itself accompanies the executable.
 *
 *   - Redistributions must reproduce the above copyright notice, this list of
 *     conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <libretro.h>
#include <libretro-core.h>
#include <retro_endianness.h>

#include "retro_snd.h"

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
extern retro_log_printf_t log_cb;


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
      LOGE("Incompatible audio type (%dch/%dbits) (1ch/16bits req) \n", snd->head.NumChannels, snd->head.BitsPerSample);
      return false;
   }

   //LOGI(" | sizeChunk: %d | channels: %d | BPS: %d\n", snd->head.Subchunk2Size, snd->head.NumChannels, snd->head.BitsPerSample);

   snd->samples_tot      = snd->head.Subchunk2Size / AUDIO_BYTES;
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
 * load internal sounds and prepare mixer to be used
 * return false if cannot allocate memory or invalid wav file is used.
 */
bool init_retro_snd(void){
   memset(sounds, 0, sizeof(sounds));

   if(!sound_load(&sounds[SND_FDCMOTOR], motor, motor_size))
      return false;
   if(!sound_load(&sounds[SND_FDCREAD], read_drive, read_size))
      return false;
   if(!sound_load(&sounds[SND_FDCSEEK], seek_drive, seek_size))
      return false;

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
 * clamp_16:
 * @val: The 32-bit audio sample value to be clamped.
 *
 * Restricts a 32-bit integer to the valid range of a signed 16-bit integer 
 * (-32768 to 32767). Used during audio mixing to prevent integer overflow
 * and audio clipping when combining multiple sound sources.
 *
 * Returns: The clamped 16-bit audio sample.
 */
static inline int16_t clamp_16(int32_t val)
{
   if (val > 32767) return 32767;
   if (val < -32768) return -32768;
   return (int16_t)val;
}

/**
 * mix_audio_batch:
 * @snd:         Sample structure to be mixed.
 * @dest_buffer: Pointer to the emulator's main stereo audio buffer.
 * @frames:      Number of audio frames to process.
 *
 * A lightweight mixer loop that adds a 16-bit mono internal sound 
 * (e.g., floppy drive noise) into the main 16-bit stereo output buffer.
 */
static void mix_audio_batch(retro_guisnd_t* snd, int16_t* dest_buffer, unsigned frames)
{
   if (snd->sample_pos + frames > snd->samples_tot) {
      if(snd->state == ST_ON) {
         sound_stop(snd);
         return;
      }
      snd->sample_pos = 0;
   }

   int16_t* samples = dest_buffer;
   int16_t* rawsamples16 = (int16_t*) ((uint8_t*) snd->rawsamples + (AUDIO_BYTES * snd->sample_pos));
   unsigned i = frames;

   while (i--)
   {
      // addd 32bits and limit to 16 bits to avoid any distorsion (clamping)
      samples[0] = clamp_16((int32_t)samples[0] + (int32_t)*rawsamples16);
      samples[1] = clamp_16((int32_t)samples[1] + (int32_t)*rawsamples16);

      rawsamples16++;
      samples += 2;
   }

   snd->sample_pos += frames;
}


/**
 * retro_snd_mixer_batch:
 * mixes each sound (active or looped) into emulator buffer for batch audio cb
 */
void retro_snd_mixer_batch(int16_t* dest_buffer, unsigned frames)
{
   int n;
   for(n = 0; n < SND_LAST; n++) {
      if (sounds[n].state != ST_OFF) {
         mix_audio_batch(&sounds[n], dest_buffer, frames);
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

