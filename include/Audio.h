//
// Created by jul3x on 14.01.2021.
//

#ifndef BEATSY_AUDIO_H
#define BEATSY_AUDIO_H

#include <string>
#include <vector>
#include <tuple>

#include "../third-party/Gist/src/Gist.h"
#include "../third-party/AudioFile.h"

class Audio {
public:
    explicit Audio(const std::string& path);

    std::tuple<const std::vector<float>&, float> getAnalizedData(double time_point);

    [[nodiscard]] int getSampleRate() const;

private:
    AudioFile<float> file;
    Gist<float> gist;

    std::vector<float> audio_frame;

};



#endif //BEATSY_AUDIO_H
