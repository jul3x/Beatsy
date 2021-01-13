//
// Created by jul3x on 27.02.19.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <fstream>
#include <iostream>

#include "../third-party/jsoncpp/include/json/json.h"

class Config {

public:
    Config(const Config&) = delete;

    Config& operator=(const Config&) = delete;

    static Config& getInstance()
    {
        static Config instance;
        return instance;
    }

    void initialize(const std::string& filename)
    {
        std::ifstream ifs;
        ifs.open(filename);

        Json::CharReaderBuilder builder;
        builder["collectComments"] = true;
        JSONCPP_STRING errs;
        if (!parseFromStream(builder, ifs, &root, &errs))
        {
            std::cout << errs << std::endl;
            return;
        }
        std::cout << "Config file parsed successfully." << std::endl;
    }

    [[nodiscard]] const Json::Value& get(const std::string& key) const
    {
        return root[key];
    }

private:
    Config() = default;

    Json::Value root;

};

#define CFG Config::getInstance()
#define CONF CFG.get


#endif //CONFIG_H
