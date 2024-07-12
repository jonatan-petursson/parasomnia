#include <JuceHeader.h>
#include "LabelTextProvider.h"
#include "BinaryData.h"

nlohmann::json LabelTextProvider::jsonData;
std::string LabelTextProvider::jsonString;
bool LabelTextProvider::isInitialized = false;

void LabelTextProvider::initialize()
{
    if (!isInitialized)
    {
        int labels_jsonSize = BinaryData::labels_jsonSize;
        auto json = std::string(BinaryData::getNamedResource("labels_json", labels_jsonSize));

        try
        {
            jsonData = nlohmann::json::parse(json);
            jsonString = json;
            isInitialized = true;
        }
        catch (const nlohmann::json::parse_error &e)
        {
            throw std::runtime_error("Invalid JSON string: " + std::string(e.what()));
        }
    }
}

std::string LabelTextProvider::get(const std::string &key)
{
    if (!isInitialized)
    {
        LabelTextProvider::initialize();
    }

    try
    {
        juce::Logger::writeToLog("LabelTextProvider: Key '" + key + "'");
        nlohmann::json::json_pointer ptr(key);
        return jsonData.at(ptr).get<std::string>();
    }

    catch (const nlohmann::json::out_of_range &e)
    {
        throw std::out_of_range("Key not found: " + key);
        // return "notfound";
    }
    catch (const nlohmann::json::type_error &e)
    {
        throw std::runtime_error("Value is not a string for key: " + key);
    }
}

const std::string &LabelTextProvider::getJsonString()
{
    if (!isInitialized)
    {
        LabelTextProvider::initialize();
    }
    return jsonString;
}

std::string LabelTextProvider::getParamLabel(int page, int param)
{
    return LabelTextProvider::get("/pages/" + std::to_string(page) + "/parameters/" + std::to_string(param) + "/label");
}

std::string LabelTextProvider::getPageTitle(int page)
{
    return LabelTextProvider::get("/pages/" + std::to_string(page) + "/title");
}

std::string LabelTextProvider::getPageShortTitle(int page)
{
    return LabelTextProvider::get("/pages/" + std::to_string(page) + "/shortTitle");
}

std::string LabelTextProvider::getParamShortLabel(int page, int param)
{
    return LabelTextProvider::get("/pages/" + std::to_string(page) + "/parameters/" + std::to_string(param) + "/shortLabel");
}

std::string LabelTextProvider::getParamName(int page, int param)
{
    return LabelTextProvider::getPageShortTitle(page) + " " + LabelTextProvider::getParamShortLabel(page, param);
}