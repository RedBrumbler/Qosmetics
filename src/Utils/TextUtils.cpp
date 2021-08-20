#include "Utils/TextUtils.hpp"

bool TextUtils::shouldRainbow(std::string name)
{
    std::string temp = toLower(name);

    if (temp.find("rainbow") != std::string::npos) return true;
    return false;
}

bool TextUtils::shouldRainbow(UnityEngine::Color color)
{
    if (color.r <= 0.99f) return false;
    if (color.g <= 0.99f) return false;
    if (color.b <= 0.99f) return false;
    return true;
}

std::string TextUtils::rainbowify(std::string input)
{
    std::string result = "";

    for (auto c : input)
    {
        result += string_format("<color=%s>%c</color>", colors[rainbowIndex].c_str(), c);
        rainbowIndex++;
        rainbowIndex %= colors.size();
    }

    return result;
}

std::string TextUtils::toLower(std::string in)
{
    std::string result = "";
    for (auto c : in)
    {
        result += tolower(c);
    }
    return result;
}
