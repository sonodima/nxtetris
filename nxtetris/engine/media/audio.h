#ifndef engine_media_audio_h
#define engine_media_audio_h

#include <stdio.h>

#include <portaudio.h>
#include <sndfile.h>

typedef struct {

} Audio;

typedef struct {
    PaStream* stream;
    SNDFILE* sound_file;
    SF_INFO file_info;
    int position;
    int looped;
} Sound;

Audio* make_audio(void);

void free_audio(Audio* audio);

int portaudio_callback(const void* input, void* output, unsigned long frame_count,
                       const PaStreamCallbackTimeInfo* time_info,
                       PaStreamCallbackFlags flags, void* user_data);
/**
 * Loads an audio file in memory and sets up its stream.
 * The audio device used is the default one, and the
 * playback channels are obtained from the audio file info.
 * @param audio pointer to the audio context.
 * @param path filepath (absolute/relative) of the sound source.
 * @param looped sound loop control.
 * @see make_sound()
 * @return Pointer to the sound if succesful, 0 if failed.
 */
Sound* make_sound(Audio* audio, const char* path, int looped);

void free_sound(Sound* sound);

/**
 * Starts sound playback from initial position.
 * If the stream is already active it will get interrupted
 * to avoid conflicts.
 * @param sound pointer to the sound.
 * @see start_sound()
 * @return Pointer to the sound if succesful, 0 if failed.
 */
int start_sound(Sound* sound);

int stop_sound(Sound* sound);

#endif /* engine_media_audio_h */
