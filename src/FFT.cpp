//
// Created by julex on 07.01.2021.
//

#include <iostream>
#include <FFT.h>
#include "../third-party/Gist/src/Gist.h"
#include "../third-party/AudioFile.h"

// Get 2048 samples
#define TIME 100048
#define N TIME/2

const std::vector<float>& applyFFT(double time_point)
{

    static AudioFile<float> samples("test.wav");
    static int frameSize = 512;
    static int sampleRate = samples.getSampleRate();
    static Gist<float> gist (frameSize, sampleRate);

    static std::vector<float> audioFrame(frameSize);
//
//    if (offset > samples.getNumSamplesPerChannel() - frameSize - 1)
//        return gist.getMagnitudeSpectrum();


    std::cout << time_point << std::endl;
    for (int i = 0; i < frameSize; ++i)
        audioFrame[i] = samples.samples[0][(int)(time_point * sampleRate) + i];

    gist.processAudioFrame (audioFrame);
    const std::vector<float>& magSpec = gist.getMagnitudeSpectrum();

    return magSpec;
}


short* loadMp3File(const std::string& name)
{
    mpg123_handle *m = NULL;
    int  channels = 0, encoding = 0;
    long rate = 0;
    int err = MPG123_OK;

    err = mpg123_init();
    m = mpg123_new(NULL, &err);
    mpg123_open(m, name.c_str());
    mpg123_getformat(m, &rate, &channels, &encoding);

    err = mpg123_format_none(m);
    err = mpg123_format(m, rate, channels, encoding);

    // 16-bit integer encoded in bytes, hence x2 size
    unsigned char* buffer = new unsigned char[TIME*2];
    size_t done = 0;
    err = mpg123_read(m, buffer, TIME*2, &done);

    short* samples = new short[done/2];
    int index = 0;

    // Iterate 2 bytes at a time
    for (int i = 0; i < done; i += 2)
    {
        unsigned char first = buffer[i];
        unsigned char second = buffer[i + 1];
        samples[index++] = (first | (second << 8));

    }

    return samples;
}
