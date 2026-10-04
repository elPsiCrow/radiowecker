// Audio pipeline: ICY stream -> buffer -> MP3 decoder -> I2S, plus the alarm beep generator.
#pragma once

#include <stdint.h>

class AudioGenerator;
extern AudioGenerator *decoder;  // Non-null while a stream is set up

void setup_audio();
void audio_loop();     // Call frequently from loop(); pumps the decoder or the beep
void stopPlaying();    // Tear down stream, decoder and I2S channel
bool startUrl(const char *url, bool useAlarmGain = false);
void setGain(float gain);  // Volume 0-100
void alarm_beep_start(void);
