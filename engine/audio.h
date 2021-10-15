#ifndef engine_audio_h
#define engine_audio_h

#include <stdio.h>

#include <portaudio.h>
#include <sndfile.h>

typedef struct {
    PaStreamParameters stream_parameters;
    PaStream* stream;
    SNDFILE* sound_file;
    SF_INFO file_info;
    float* buffer;
} Audio;

Audio* make_audio(void);

void free_audio(Audio* audio);

#endif /* engine_audio_h */
