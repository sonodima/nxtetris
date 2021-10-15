#ifndef engine_audio_h
#define engine_audio_h

#include <stdio.h>

#include <portaudio.h>
#include <sndfile.h>

typedef struct {

} Audio;

typedef struct {
    PaStreamParameters stream_parameters;
    PaStream* stream;
    SNDFILE* sound_file;
    SF_INFO file_info;
    float* buffer;
    int position;
    int* cursor;
} Sound;

Audio* make_audio(void);

void free_audio(Audio* audio);

Sound* make_sound(Audio* audio, const char* path);

void free_sound(Sound* sound);

int start_sound(Sound* sound);

int stop_sound(Sound* sound);


#endif /* engine_audio_h */
