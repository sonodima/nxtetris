#include "audio.h"

#include <stdlib.h>

#define FRAMES_PER_BUFFER 512

int portaudio_callback(const void* input, void* output, unsigned long frame_count,
                                     const PaStreamCallbackTimeInfo* time_info,
                                     PaStreamCallbackFlags flags, void* user_data) {
    Sound* sound;
    sf_count_t read_length;
    
    sound = (Sound*)user_data;
    
    if (frame_count > 0) {
        sf_seek(sound->sound_file, sound->position, SEEK_SET);
        read_length = sf_readf_int(sound->sound_file, output, frame_count);
        if (read_length > 0) {
            sound->position += read_length;
            return paContinue;
        } else {
            if (sound->looped) {
                /*
                 If the sound is looped, reset the
                 position for the next iteration.
                 */
                sound->position = 0;
                return paContinue;
            } else {
                return paComplete;
            }
        }
    }
    
    return paComplete;
}

Audio* make_audio(void) {
    Audio* audio;
    PaError error;
    
    audio = malloc(sizeof(Audio));

    error = Pa_Initialize();
    if (error != paNoError) {
        printf("%s\n", Pa_GetErrorText(error));
        return 0;
    }
    
    return audio;
}

void free_audio(Audio* audio) {
    Pa_Terminate();
    
    if (audio) {
        free(audio);
    }
}

Sound* make_sound(Audio* audio, const char* path, int looped) {
    Sound* sound;
    PaError error;
    
    sound = malloc(sizeof(Sound));
    sound->looped = looped;
    
    sound->sound_file = sf_open(path, SFM_READ, &sound->file_info);
    if (!sound->sound_file) {
        Pa_Terminate();
        printf("[%s] %s\n", path, sf_strerror(sound->sound_file));
        return 0;
    }
    
    sound->stream_parameters.device = Pa_GetDefaultOutputDevice();
    sound->stream_parameters.sampleFormat = paInt32;
    sound->stream_parameters.channelCount = sound->file_info.channels;
    sound->stream_parameters.suggestedLatency =
        Pa_GetDeviceInfo(sound->stream_parameters.device)->defaultLowOutputLatency;
    sound->stream_parameters.hostApiSpecificStreamInfo = 0;
    
    /*
     Setup the stream for the sample with the proper callback.
     */
    error = Pa_OpenStream(&sound->stream, 0, &sound->stream_parameters, sound->file_info.samplerate,
                          FRAMES_PER_BUFFER, paClipOff, &portaudio_callback, sound);
    if (error != paNoError) {
        Pa_Terminate();
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
        error = Pa_StartStream(sound->stream);
        if (error != paNoError) {
            Pa_Terminate();
            printf("%s\n", Pa_GetErrorText(error));
            return 0;
        }
    }
    
    return 1;
}

int stop_sound(Sound* sound) {
    PaError error;

    if (sound) {
        error = Pa_StopStream(sound->stream);
        if (error != paNoError) {
            Pa_Terminate();
            printf("%s\n", Pa_GetErrorText(error));
            return 0;
        }
    }
    
    return 1;
}
