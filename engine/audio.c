#include "audio.h"

#include <stdlib.h>

#define FRAMES_PER_BUFFER 512

int portaudio_callback(const void* input, void* output, unsigned long frame_count,
                                     const PaStreamCallbackTimeInfo* time_info,
                                     PaStreamCallbackFlags flags, void* user_data) {
    Audio* audio;
    float* out;
    sf_count_t read_length;
    unsigned int i, j, cur = 0;
    double gain = 1.0;
    
    audio = (Audio*)user_data;
    out = (float*)output;

    read_length = sf_readf_float(audio->sound_file, audio->buffer, FRAMES_PER_BUFFER);
    if (read_length > 0) {
        for (i = 0; i < read_length; ++i) {
            for (j = 0; j < audio->stream_parameters.channelCount; ++j) {
                *out++ = audio->buffer[cur++];
            }
        }
        return paContinue;
    } else {
        return paComplete;
    }
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

    audio->sound_file = sf_open("/Users/tommaso/Desktop/bg.wav", SFM_READ, &audio->file_info);
    if (!audio->sound_file) {
        Pa_Terminate();
        printf("%s\n", sf_strerror(audio->sound_file));
        return 0;
    }
    
    audio->stream_parameters.device = Pa_GetDefaultOutputDevice();
    audio->stream_parameters.sampleFormat = paInt32;
    audio->stream_parameters.channelCount = audio->file_info.channels;
    audio->stream_parameters.suggestedLatency =
        Pa_GetDeviceInfo(audio->stream_parameters.device)->defaultLowOutputLatency;
    audio->stream_parameters.hostApiSpecificStreamInfo = 0;
    
    /*
     Allocate the sound buffer.
     */
    audio->buffer = calloc(audio->file_info.channels * FRAMES_PER_BUFFER, sizeof(float));

    
    error = Pa_OpenStream(&audio->stream, 0, &audio->stream_parameters, audio->file_info.samplerate,
                          FRAMES_PER_BUFFER, paClipOff, &portaudio_callback, audio);
    if (error != paNoError) {
        Pa_Terminate();
        printf("%s\n", Pa_GetErrorText(error));
        return 0;
    }
    
    error = Pa_StartStream(audio->stream);
    if (error != paNoError) {
        Pa_Terminate();
        printf("%s\n", Pa_GetErrorText(error));
        return 0;
    }
    
    
    /*
    
    
    error = Pa_CloseStream(stream);
    if (error != paNoError) {
        Pa_Terminate();
        printf("%s\n", Pa_GetErrorText(error));
        return;
    }
    
    
    
    
    Pa_Terminate();
     */
    
    return audio;
}

void free_audio(Audio* audio) {
    
}
