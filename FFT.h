//
// Created by julex on 06.01.2021.
//

#ifndef BEATSY_FFT_H
#define BEATSY_FFT_H

#ifndef M_PI
#define M_PI 3.14159265358979324
#endif

#include "third-party/kissfft/kiss_fft.h"


#define N 16

void TestFft(const char* title, const kiss_fft_cpx in[N], kiss_fft_cpx out[N / 2 + 1])
{
    kiss_fft_cfg cfg;

    printf("%s\n", title);

    if ((cfg = kiss_fft_alloc(N, 0/*is_inverse_fft*/, NULL, NULL)) != NULL)
    {
        size_t i;

        kiss_fft(cfg, in, out);
        free(cfg);

        for (i = 0; i < N; i++)
        {
            printf(" in[%2zu] = %+f    ",
                   i, in[i]);
            if (i < N / 2 + 1)
                printf("out[%2zu] = %+f , %+f",
                       i, out[i].r, out[i].i);
            printf("\n");
        }
    }
    else
    {
        printf("not enough memory?\n");
        exit(-1);
    }
}


void applyFFT()
{
    kiss_fft_cpx in[N];
    kiss_fft_cpx out[N / 2 + 1];
    size_t i;

    for (i = 0; i < N; i++)
        in[i] = {i, 0};
    TestFft("Zeroes (real)", in, out);
}


#endif //BEATSY_FFT_H
