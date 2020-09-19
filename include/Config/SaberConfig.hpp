#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "Data/CustomTrail.hpp"
#include <vector>

namespace Qosmetics
{
    class SaberConfig
    {
        public:
            SaberConfig()
            {
                enableFakeGlow = false;
                hasCustomTrails = false;
                hasCustomWallParticles = false;
                hasCustomSliceParticles = false;
                leftTrails.clear();
                rightTrails.clear();
            }

            SaberConfig(rapidjson::Document &config)
            {
                enableFakeGlow = config["enableFakeGlow"].GetBool();
                hasCustomTrails = config["hasCustomTrails"].GetBool();
                hasCustomWallParticles = config["hasCustomWallParticles"].GetBool();
                hasCustomSliceParticles = config["hasCustomSliceParticles"].GetBool();

                const rapidjson::Value& leftTrails = config["leftTrails"];
                assert(leftTrails.IsArray());
                for (auto& trail : leftTrails.GetArray())
                {
                    int length = trail["length"].GetInt();
                    int type = trail["colorType"].GetInt();
                    int whiteStep = trail["whiteStep"].GetInt();
                    std::string name = trail["name"].GetString();
                    UnityEngine::Color trailColor;
                    {
                        trailColor.r = trail["trailColor"]["R"].GetFloat();
                        trailColor.g = trail["trailColor"]["G"].GetFloat();
                        trailColor.b = trail["trailColor"]["B"].GetFloat();
                        trailColor.a = trail["trailColor"]["A"].GetFloat();
                    }

                    UnityEngine::Color multiplierColor;
                    {
                        multiplierColor.r = trail["multiplierColor"]["R"].GetFloat();
                        multiplierColor.g = trail["multiplierColor"]["G"].GetFloat();
                        multiplierColor.b = trail["multiplierColor"]["B"].GetFloat();
                        multiplierColor.a = trail["multiplierColor"]["A"].GetFloat();
                    }

                    this->leftTrails.push_back(CustomTrail(name, type, trailColor, multiplierColor, length, whiteStep));
                }

                const rapidjson::Value& rightTrails = config["rightTrails"];
                assert(rightTrails.IsArray());
                for (auto& trail : rightTrails.GetArray())
                {
                    int length = trail["length"].GetInt();
                    int type = trail["colorType"].GetInt();
                    int whiteStep = trail["whiteStep"].GetInt();
                    std::string name = trail["name"].GetString();
                    UnityEngine::Color trailColor;
                    {
                        trailColor.r = trail["trailColor"]["R"].GetFloat();
                        trailColor.g = trail["trailColor"]["G"].GetFloat();
                        trailColor.b = trail["trailColor"]["B"].GetFloat();
                        trailColor.a = trail["trailColor"]["A"].GetFloat();
                    }

                    UnityEngine::Color multiplierColor;
                    {
                        multiplierColor.r = trail["multiplierColor"]["R"].GetFloat();
                        multiplierColor.g = trail["multiplierColor"]["G"].GetFloat();
                        multiplierColor.b = trail["multiplierColor"]["B"].GetFloat();
                        multiplierColor.a = trail["multiplierColor"]["A"].GetFloat();
                    }

                    this->rightTrails.push_back(CustomTrail(name, type, trailColor, multiplierColor, length, whiteStep));
                }
            }

            std::vector<Qosmetics::CustomTrail>* get_leftTrails()
            {
                return &leftTrails;
            }

            std::vector<Qosmetics::CustomTrail>* get_rightTrails()
            {
                return &rightTrails;
            }

            bool get_hasCustomTrails()
            {
                return hasCustomTrails;
            }

            bool get_enableFakeGlow()
            {
                return enableFakeGlow;
            }

            bool get_hasCustomWallParticles()
            {
                return hasCustomWallParticles;
            }

            bool get_hasCustomSliceParticles()
            {
                return hasCustomSliceParticles;
            }

        private:
            bool enableFakeGlow;
            bool hasCustomTrails;
            bool hasCustomWallParticles;
            bool hasCustomSliceParticles;

            std::vector<Qosmetics::CustomTrail> leftTrails;
            std::vector<Qosmetics::CustomTrail> rightTrails;
    };
}