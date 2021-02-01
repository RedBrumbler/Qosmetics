#pragma once
#include "Data/ItemConfig.hpp"
#include "Trail/TrailConfig.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace Qosmetics
{
    class SaberConfig : public ItemConfig
    {
        public:
            SaberConfig(rapidjson::Document& d)
            {
                enableFakeGlow = d["enableFakeGlow"].GetBool();
                hasCustomTrails = d["hasCustomTrails"].GetBool();
                hasCustomWallParticles = d["hasCustomWallParticles"].GetBool();
                hasCustomSliceParticles = d["hasCustomSliceParticles"].GetBool();

                const rapidjson::Value& leftTrails = d["leftTrails"];
                assert(leftTrials.IsArray());

                for (auto& trail : leftTrails.GetArray())
                {
                    this->leftTrails.push_back(TrailConfig(trail));
                }

                const rapidjson::Value& rightTrails = d["rightTrails"];
                assert(leftTrials.IsArray());

                for (auto& trail : rightTrails.GetArray())
                {
                    this->rightTrails.push_back(TrailConfig(trail));
                }
            }

            std::vector<TrailConfig>& get_leftTrails()
            {
                return leftTrails;
            }

            std::vector<TrailConfig>& get_rightTrails()
            {
                return rightTrails;
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

            std::vector<TrailConfig> leftTrails;
            std::vector<TrailConfig> rightTrails;
    };
}