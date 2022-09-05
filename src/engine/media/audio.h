#ifndef NXTETRIS_SRC_ENGINE_MEDIA_AUDIO_H_
#define NXTETRIS_SRC_ENGINE_MEDIA_AUDIO_H_

#include <stdio.h>

#include <portaudio.h>
#include <sndfile.h>

typedef struct {
	PaStream* stream;
	SNDFILE* sound_file;
	SF_INFO file_info;
	int position;
	int looped;
	float volume;
	int completed;
} Sound;

/**
 * Sets up the audio system.
 * Call this before using any other audio functions.
 *
 * If this function fails, it will print an error message.
 * @return 1 if successful, 0 otherwise.
 */
int initialize_audio(void);

/**
 * Cleans up the audio system.
 * Call this when you are done using the audio system.
 */
void uninitialize_audio(void);

/**
 * Processes and fills the output buffers.
 * @param input Unused input buffer.
 * @param output Output audio device buffer.
 * @param frame_count Number of the current frame.
 * @param time_info Timing information for the buffers passed to the stream callback.
 * @param flags Flag bit constants for the statusFlags to PaStreamCallback.
 * @param user_data Pointer to the sound data.
 * @return paContinue if the routine has not finished.
 */
int portaudio_callback(
		const void* input, void* output,
		unsigned long frame_count,
		const PaStreamCallbackTimeInfo* time_info,
		PaStreamCallbackFlags flags,
		void* user_data
);

/**
 * Loads an audio file in memory and sets up its stream.
 * The audio device used is the default one, and the playback channels are obtained from the audio file info.
 *
 * If this function fails, it will print an error message.
 * @param path Filepath (absolute/relative) of the sound source.
 * @param looped Sound loop control.
 * @param volume The volume of the sound. (0f to 1f)
 * @see free_sound()
 * @see start_sound()
 * @return Pointer to the sound if successful, 0 if failed.
 */
Sound* make_sound(const char* path, int looped, float volume);

/**
 * Deallocates all resources allocated on make_sound() call.
 * @param sound Pointer to the sound.
 * @see make_sound()
 * @see stop_sound()
 */
void free_sound(Sound* sound);

/**
 * Starts sound playback from initial position.
 * If the stream is already active it will get interrupted to avoid conflicts.
 * @param sound Pointer to the sound.
 * @see make_sound()
 * @see stop_sound()
 * @return 1 if the stream has started.
 */
int start_sound(Sound* sound);

/**
 * Stops playback of a sound.
 * @param sound Pointer to the sound.
 * @see start_sound()
 * @see free_sound()
 * @return 1 if the stream has been stopped.
 */
int stop_sound(Sound* sound);

#endif
