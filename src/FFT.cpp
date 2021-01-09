//
// Created by julex on 07.01.2021.
//

#include <iostream>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include <FFT.h>


#include "../third-party/Gist/src/Gist.h"
#include "../third-party/AudioFile.h"

// Get 2048 samples
#define TIME 100048
#define N TIME/2

std::tuple<const std::vector<float>&, float> applyFFT(double time_point)
{
    static AudioFile<float> samples("test.wav");
    static int frameSize = samples.getSampleRate();
    static int sampleRate = samples.getSampleRate();
    static Gist<float> gist (frameSize, sampleRate);

    static std::vector<float> audioFrame(frameSize);
//
//    if (offset > samples.getNumSamplesPerChannel() - frameSize - 1)
//        return gist.getMagnitudeSpectrum();


    for (int i = 0; i < frameSize; ++i)
        audioFrame[i] = samples.samples[0][(int)(time_point * sampleRate) + i];

    gist.processAudioFrame (audioFrame);
    const std::vector<float>& magSpec = gist.getMagnitudeSpectrum();


    return {magSpec, gist.energyDifference()};
}
