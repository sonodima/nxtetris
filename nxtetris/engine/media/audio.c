#include "audio.h"

#include <stdlib.h>

#define SAMPLES_PER_BUFFER 512

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
            sound->position = 0;
            if (sound->looped) {
                return paContinue;
            } else {
                return paComplete;
            }
        }
    }
    
    return paComplete;
}

Sound* make_sound(Audio* audio, const char* path, int looped) {
    Sound* sound;
    PaError error;
    PaStreamParameters stream_parameters;
    
    sound = malloc(sizeof(Sound));
    sound->looped = looped;
    
    sound->sound_file = sf_open(path, SFM_READ, &sound->file_info);
    if (!sound->sound_file) {
        Pa_Terminate();
        printf("[%s] %s\n", path, sf_strerror(sound->sound_file));
        return 0;
    }
    
    stream_parameters.device = Pa_GetDefaultOutputDevice();
    stream_parameters.sampleFormat = paInt32;
    stream_parameters.channelCount = sound->file_info.channels;
    stream_parameters.suggestedLatency =
        Pa_GetDeviceInfo(stream_parameters.device)->defaultLowOutputLatency;
    stream_parameters.hostApiSpecificStreamInfo = 0;
    
    /*
     Setup the stream for the sample with the proper callback.
     */
    error = Pa_OpenStream(&sound->stream, 0, &stream_parameters, sound->file_info.samplerate,
                          SAMPLES_PER_BUFFER, paClipOff, &portaudio_callback, sound);
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
        sound->position = 0;
        Pa_AbortStream(sound->stream);
        
        /*
         Start the stream if it is not already running.
         */
        if (!Pa_IsStreamActive(sound->stream)) {
            error = Pa_StartStream(sound->stream);
            if (error != paNoError) {
                Pa_Terminate();
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
            Pa_Terminate();
            printf("%s\n", Pa_GetErrorText(error));
            return 0;
        }
    }
    
    return 1;
}
