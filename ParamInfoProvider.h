#pragma once

#ifndef LABEL_TEXT_PROVIDER_H
#define LABEL_TEXT_PROVIDER_H

#include <string>
#include <stdexcept>
#include "vendor/nlohmann/json.hpp"

class ParamInfoProvider
{
private:
    static nlohmann::json jsonData;
    static std::string jsonString;
    static bool isInitialized;

    // Private constructor to prevent instantiation
    ParamInfoProvider() = delete;

public:
    static void initialize();
    static std::string get(const std::string &key);
    static std::string getParamLabel(int page, int param);
    static std::string getParamLabel(int page, int param, int mod);
    static const std::string &getJsonString();
    static std::string getPageTitle(int page);
    static std::string getPageShortTitle(int page);
    static std::string getPageAbbreviation(int page);
    static std::string getParamShortLabel(int page, int param);
    static std::string getParamName(int page, int param);
    static std::string getModParamLabel(int param);
    static std::string getModSpeedParamName(int page, int param);
    static std::string getModSpeedParamId(int page, int param);
};

#endif // LABEL_TEXT_PROVIDER_H