#ifndef NXTETRIS_ENGINE_MEDIA_AUDIO_H_
#define NXTETRIS_ENGINE_MEDIA_AUDIO_H_

#include <stdio.h>

#include <portaudio.h>
#include <sndfile.h>

typedef struct {
    PaStream* stream;
    SNDFILE* sound_file;
    SF_INFO file_info;
    int position;
    int looped;
} Sound;

int initialize_audio(void);

void uninitialize_audio(void);

/**
 * Processes and fills output buffers.
 * @param input unused input buffer.
 * @param output output audio device buffer.
 * @param frame_count number of the current frame.
 * @param time_info timing information for the buffers passed to the stream callback.
 * @param flags flag bit constants for the statusFlags to PaStreamCallback.
 * @param user_data pointer to the sound data.
 * @see start_sound()
 * @return paContinue if the routine has not finished.
 */
int portaudio_callback(const void* input, void* output, unsigned long frame_count,
                       const PaStreamCallbackTimeInfo* time_info,
                       PaStreamCallbackFlags flags, void* user_data);

/**
 * Loads an audio file in memory and sets up its stream.
 * The audio device used is the default one, and the
 * playback channels are obtained from the audio file info.
 * @param path filepath (absolute/relative) of the sound source.
 * @param looped sound loop control.
 * @see free_sound()
 * @see start_sound()
 * @return Pointer to the sound if succesful, 0 if failed.
 */
Sound* make_sound(const char* path, int looped);

/**
 * Deallocates all resources allocated on make_sound() call.
 * @param sound pointer to the sound.
 * @see make_sound()
 * @see stop_sound()
 */
void free_sound(Sound* sound);

/**
 * Starts sound playback from initial position.
 * If the stream is already active it will get interrupted
 * to avoid conflicts.
 * @param sound pointer to the sound.
 * @see portaudio_callback()
 * @see make_sound()
 * @see stop_sound()
 * @return 1 if the stream has started.
 */
int start_sound(Sound* sound);

/**
 * Stops playback of a sound.
 * @param sound pointer to the sound.
 * @see start_sound()
 * @see free_sound()
 * @return 1 if the stream has been stopped.
 */
int stop_sound(Sound* sound);

#endif /* NXTETRIS_ENGINE_MEDIA_AUDIO_H_ */
