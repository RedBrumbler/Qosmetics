#pragma once
#include "UnityEngine/Color.hpp"
#include "Types/Trail/TrailConfig.hpp"

struct TrailInitData
{
    int TrailLength;
    float Whitestep;
    UnityEngine::Color TrailColor;
    int Granularity;
    int SamplingFrequency;
    float SamplingStepMultiplier;

    TrailInitData(Qosmetics::TrailConfig& config)
    {
        TrailLength = config.get_length();
        Whitestep = config.get_whiteStep();
        TrailColor = config.get_color();
        Granularity = (int)(((float)config.get_length() / 10.0f) * 60.0f);
    }
};