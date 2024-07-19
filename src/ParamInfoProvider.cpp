#include <JuceHeader.h>
#include "ParamInfoProvider.h"
#include "BinaryData.h"

nlohmann::json ParamInfoProvider::jsonData;
std::string ParamInfoProvider::jsonString;
bool ParamInfoProvider::isInitialized = false;

void ParamInfoProvider::initialize()
{
    if (!isInitialized)
    {
        int params_jsonSize = BinaryData::params_jsonSize;
        auto json = std::string(BinaryData::getNamedResource("params_json", params_jsonSize));

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

std::string ParamInfoProvider::get(const std::string &key)
{
    if (!isInitialized)
    {
        ParamInfoProvider::initialize();
    }

    try
    {
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

const std::string &ParamInfoProvider::getJsonString()
{
    if (!isInitialized)
    {
        ParamInfoProvider::initialize();
    }
    return jsonString;
}

std::string ParamInfoProvider::getParamLabel(int page, int param)
{
    return ParamInfoProvider::get("/pages/" + std::to_string(page) + "/parameters/" + std::to_string(param) + "/label");
}

std::string ParamInfoProvider::getPageTitle(int page)
{
    return ParamInfoProvider::get("/pages/" + std::to_string(page) + "/title");
}

std::string ParamInfoProvider::getPageShortTitle(int page)
{
    return ParamInfoProvider::get("/pages/" + std::to_string(page) + "/shortTitle");
}

std::string ParamInfoProvider::getPageAbbreviation(int page)
{
    return ParamInfoProvider::get("/pages/" + std::to_string(page) + "/abbreviation");
}

std::string ParamInfoProvider::getParamShortLabel(int page, int param)
{
    return ParamInfoProvider::get("/pages/" + std::to_string(page) + "/parameters/" + std::to_string(param) + "/shortLabel");
}

std::string ParamInfoProvider::getParamName(int page, int param)
{
    return ParamInfoProvider::getPageShortTitle(page) + " " + ParamInfoProvider::getParamShortLabel(page, param);
}

std::string ParamInfoProvider::getModParamLabel(int param)
{
    return ParamInfoProvider::get("/modulation/" + std::to_string(param) + "/label");
}