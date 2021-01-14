//
// Created by julex on 07.01.2021.
//

#include <Audio.h>
#include <Config.h>

Audio::Audio(const std::string& path) : file(path),
                                        gist(file.getSampleRate(), file.getSampleRate()),
                                        audio_frame(file.getSampleRate()) {}

std::tuple<const std::vector<float>&, float> Audio::getAnalizedData(double time_point) {
    if (time_point >= file.getLengthInSeconds() - 1.0 || time_point <= 0.0)
        return {gist.getMagnitudeSpectrum(), {}};

    for (int i = 0; i < file.getSampleRate(); ++i)
        audio_frame[i] = file.samples[0][static_cast<size_t>(time_point * file.getSampleRate()) + i];

    gist.processAudioFrame(audio_frame);

    return {gist.getMagnitudeSpectrum(), gist.energyDifference()};
}

int Audio::getSampleRate() const {
    return file.getSampleRate();
}
