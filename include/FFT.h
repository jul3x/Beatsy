//
// Created by julex on 06.01.2021.
//

#ifndef BEATSY_FFT_H
#define BEATSY_FFT_H


#include <string>
#include <vector>
#include <tuple>


std::tuple<const std::vector<float>&, float> applyFFT(double time_point);

short* loadMp3File(const std::string& name);

#endif //BEATSY_FFT_H
