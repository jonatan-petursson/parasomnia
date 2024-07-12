// LabelTextProvider.h

#ifndef LABEL_TEXT_PROVIDER_H
#define LABEL_TEXT_PROVIDER_H

#include <string>
#include <stdexcept>
#include "vendor/nlohmann/json.hpp"

class LabelTextProvider
{
private:
    static nlohmann::json jsonData;
    static std::string jsonString;
    static bool isInitialized;

    // Private constructor to prevent instantiation
    LabelTextProvider() = delete;

public:
    static void initialize();
    static std::string get(const std::string &key);
    static std::string getParamLabel(int page, int param);
    static std::string getParamLabel(int page, int param, int mod);
    static const std::string &getJsonString();
    static std::string getPageTitle(int page);
    static std::string getPageShortTitle(int page);
    static std::string getParamShortLabel(int page, int param);
    static std::string getParamName(int page, int param);
};

#endif // LABEL_TEXT_PROVIDER_H