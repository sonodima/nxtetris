#include "audio.h"

#include <stdlib.h>

#define SAMPLES_PER_BUFFER 512

int initialize_audio(void) {
	PaError error;

	/* Initialize PortAudio library's internal data structures and prepare underlying host APIs for use */
	error = Pa_Initialize();
	if (error != paNoError) {
		printf("%s\n", Pa_GetErrorText(error));
		return 0;
	}

	return 1;
}

void uninitialize_audio(void) {
	/* Deallocate all resources allocated by PortAudio on Pa_Initialize() call */
	Pa_Terminate();
}

int portaudio_callback(
		const void* input,
		void* output,
		unsigned long frame_count,
		const PaStreamCallbackTimeInfo* time_info,
		PaStreamCallbackFlags flags,
		void* user_data
) {
	Sound* sound;
	sf_count_t read_length;
	int i;

	sound = (Sound*)user_data;
	(void)(input);
	(void)(time_info);
	(void)(flags);

	if (frame_count > 0) {
		/* Update the current read location of the file */
		sf_seek(sound->sound_file, sound->position, SEEK_SET);

		/* Read data from the sound file (at the sought position) to the output buffer */
		read_length = sf_readf_int(sound->sound_file, output, frame_count);

		/* Multiply the pcm buffer by the volume value */
		for (i = 0; i < SAMPLES_PER_BUFFER * 2; ++i) {
			((int*)output)[i] *= sound->volume;
		}

		if (read_length > 0) {
			/*
			 * Some samples have been read.
			 * Tell PortAudio to continue executing the callback.
			 */
			sound->position += read_length;
			sound->completed = 0;
			return paContinue;
		} else {
			/*
			 * No samples read. Reset the position and tell PortAudio to continue executing the callback
			 * if the sound is looped, complete the operation if the sound is not looped.
			 */
			if (sound->looped) {
				sound->position = 0;
				sound->completed = 0;
				return paContinue;
			} else {
				sound->completed = 1;
				/*
				 * This should return paComplete, but due to some issues with PortAudio on macOS,
				 * We need to keep the stream open if we want to quickly restart the sound multiple times
				 * without freezes.
				 * This way, to restart the sound, we just need to reset the position to 0.
				 *
				 * This is not a problem as long as we don't have too many sounds.
				 */
				return paContinue;
			}
		}
	}

	/* Here we should ideally return paComplete too, but see above */
	sound->completed = 1;
	return paContinue;
}

Sound* make_sound(const char* path, int looped, float volume) {
	Sound* sound;
	PaError error;
	PaStreamParameters stream_parameters;

	sound = malloc(sizeof(Sound));
	sound->looped = looped;
	sound->volume = volume;
	sound->completed = 0;

	/* Open a handle to the file at the given path, in read mode */
	sound->sound_file = sf_open(path, SFM_READ, &sound->file_info);
	if (!sound->sound_file) {
		printf("[%s] %s\n", path, sf_strerror(sound->sound_file));
		return 0;
	}

	/* Setup stream parameters using the default output device */
	stream_parameters.device = Pa_GetDefaultOutputDevice();
	stream_parameters.sampleFormat = paInt32;
	stream_parameters.channelCount = sound->file_info.channels;
	stream_parameters.suggestedLatency = Pa_GetDeviceInfo(stream_parameters.device)->defaultLowOutputLatency;
	stream_parameters.hostApiSpecificStreamInfo = 0;

	/* Set up the stream for the sample using the callback mode */
	error = Pa_OpenStream(
			&sound->stream,
			0, &stream_parameters,
			sound->file_info.samplerate,
			SAMPLES_PER_BUFFER, paClipOff,
			&portaudio_callback,
			sound
	);
	if (error != paNoError) {
		printf("[%s] %s\n", path, Pa_GetErrorText(error));
		return 0;
	}

	return sound;
}

void free_sound(Sound* sound) {
	PaError error;

	if (sound) {
		error = Pa_CloseStream(sound->stream);
		if (error != paNoError) {
			printf("%s\n", Pa_GetErrorText(error));
		}

		sf_close(sound->sound_file);
		free(sound);
	}
}

int start_sound(Sound* sound) {
	PaError error;

	if (sound) {
		/* Reset position without interrupting the stream */
		sound->position = 0;

		/* If the stream is stopped, start it (happens the first time a sound is played) */
		if (Pa_IsStreamStopped(sound->stream)) {
			error = Pa_StartStream(sound->stream);
			if (error != paNoError) {
				printf("%s\n", Pa_GetErrorText(error));
				return 0;
			}
		}
	}

	return 1;
}

int stop_sound(Sound* sound) {
	PaError error;

	if (sound) {
		error = Pa_StopStream(sound->stream);
		if (error != paNoError) {
			printf("%s\n", Pa_GetErrorText(error));
			return 0;
		}
	}

	return 1;
}
