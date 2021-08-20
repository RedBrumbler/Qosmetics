#pragma once
#include "UnityEngine/Color.hpp"
#include "Types/Trail/TrailConfig.hpp"

struct TrailInitData
{
    int TrailLength = 30;
    float Whitestep = 0.2f;
    UnityEngine::Color TrailColor = {1.0f, 1.0f, 1.0f, 1.0f};
    int Granularity = 80;

    TrailInitData() {};
    TrailInitData(Qosmetics::TrailConfig& config)
    {
        TrailLength = config.get_length();
        Whitestep = config.get_whiteStep();
        TrailColor = config.get_color();
        Granularity = (int)(((float)config.get_length() / 10.0f) * 60.0f);
    }
};