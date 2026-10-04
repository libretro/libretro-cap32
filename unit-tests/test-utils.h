/****************************************************************************
 *  Caprice32 libretro port
 *
 *   David Colmenero - D_Skywalk (2019-2021)
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
#include "libretro/retro_utils.h"

int test_dsk(char * filename_dsk, char * result_string, char * format_expected);
int test_dsk_hashed(char * file_path, char * result_string, uint32_t file_hash);

void generate_synthetic_disk(const char *filepath, bool is_system, bool is_hidden, int tweak_case);
void run_and_check(const char *filepath, int expected_count, const char* name0, bool hide0, const char* name1, bool hide1, int expected_track);
void generate_empty_disk(const char *filepath);
void generate_full_catalog_disk(const char *filepath);
void generate_corrupt_disk(const char *filepath);


/* Struct to handle the 24 different test variations */
typedef struct {
    const char* name0;
    const char* name1;
    uint16_t start0;
    uint16_t start1;
    uint16_t load;
    uint16_t length;
    int bad_header_idx;  // -1 for none
    bool bad_block;
    bool headerless;
    bool system;
    bool interleave;
    bool hidden;
    int plus3_idx;       // -1 for none
    bool bad_plus3_csum;
    bool bad_signature;
    bool empty_tracks;
} autorun_opts_t;


void generate_autorun_disk(const char *filepath, autorun_opts_t opts);
void check_autorun(autorun_opts_t opts, const char* expected_filename);


/**
 * @enum tail_mode_t
 * @brief Defines the tail corruption modes to simulate EOF anomalies in DSK files.
 */
typedef enum {
    TAIL_NONE = 0,               /**< Clean EOF, no extra data appended */
    TAIL_EXTRA_HEADER_ONLY = 1,  /**< Extra track header present, but no sector data */
    TAIL_PARTIAL_HEADER = 2,     /**< Truncated track header (e.g., 100 bytes instead of 256) */
    TAIL_PARTIAL_SECTOR = 3,     /**< Complete track header, but truncated sector payload */
    TAIL_FULL_SECTOR = 4         /**< Complete track header and sector data for a partial cylinder */
} tail_mode_t;

void check_overdump(int declared, int complete, int sides, tail_mode_t tail_mode, int expected_result);

extern uint8_t *pbGPBuffer;
