#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// min -114dB (mute) -> max 12dB (max volume), 0xFF in HEX is fast mute
#define dB2Volume(dB) (dB + 0x73)
#define FASTMUTE_MSP 0xFF
typedef struct
{
    union
    {
        struct
        {
            // tryb dźwieku stereo itp
            sound_mode_msp_enum sound_mode : 8;
            // wejscie sygnału
            source_msp_enum source : 8;
        };
        unsigned short val;
    } data;
} SRC_AUX_REG;
// volume headphone
typedef struct
{
    union
    {
        struct
        {
            unsigned char reduce_value : 1;
            unsigned char reduce_tone_control : 1;
            unsigned char compromise : 1;
            unsigned char dynamic : 1;
            unsigned char reserved4 : 1;
            // 0.125dB*value
            unsigned char higher_resolution_volume : 3;
            // 0xFF is fast mute -> use dB2Volume funcion
            unsigned char volume : 8;
        };
        unsigned short val;
    } data;
} VOL_AUX_REG;
typedef enum source_msp_enum
{
    SOURCE_FM_AM_MSP,
    SOURCE_STEREO_MSP,
    SOURCE_STEREO_A_MSP,
    SOURCE_STEREO_B_MSP,
    SOURCE_SCART_INPUT_MSP,
    SOURCE_I2S1_INPUT_MSP,
    SOURCE_I2S2_INPUT_MSP
} source_msp_enum;
typedef enum sound_mode_msp_enum
{
    SOUND_MODE_SOUND_A_MONO_MSP = 0x00,
    SOUND_MODE_SOUND_B_MONO_MSP = 0x10,
    SOUND_MODE_STEREO_MSP = 0x20,
    SOUND_MODE_MONO_MSP = 0x30,
} sound_mode_msp_enum;
// REG ADRESS 0x0009
int main(int argc, char *argv[])
{
    short _a = 0, _b = 0;
    VOL_AUX_REG *VOL_AUX_0006 = (void *)&_b;
    SRC_AUX_REG *SRC_AUX_0009 = (void *)&_a;
    VOL_AUX_0006->data.volume = dB2Volume(0);
    SRC_AUX_0009->data.sound_mode = SOUND_MODE_STEREO_MSP;
    SRC_AUX_0009->data.source = SOURCE_I2S1_INPUT_MSP;
    printf("stop");
}