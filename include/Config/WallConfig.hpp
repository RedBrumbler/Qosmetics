#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include <vector>

namespace Qosmetics
{
    class WallConfig
    {
        public:

            WallConfig(rapidjson::Document &config)
            {
                this->replaceCoreMaterial = config["replaceCoreMaterial"].GetBool();
                this->replaceCoreMesh = config["replaceCoreMesh"].GetBool();
                this->disableFrame = config["disableFrame"].GetBool();
                this->disableFakeGlow = config["disableFakeGlow"].GetBool();
                this->replaceFrameMaterial = config["replaceFrameMaterial"].GetBool();
                this->replaceFrameMesh = config["replaceFrameMesh"].GetBool();
                this->scoreSubmissionDisabled = config["scoreSubmissionDisabled"].GetBool();
                this->moreThan1CoreMat = config["moreThan1Core"].GetBool();
                this->moreThan1FrameMat = config["moreThan1Frame"].GetBool();
            }

            bool get_replaceCoreMaterial()
            {
                return replaceCoreMaterial;
            }

            bool get_replaceCoreMesh()
            {
                return replaceCoreMesh;
            }

            bool get_disableFrame()
            {
                return disableFrame;
            }

            bool get_disableFakeGlow()
            {
                return disableFakeGlow;
            }

            bool get_replaceFrameMaterial()
            {
                return replaceFrameMaterial;
            }

            bool get_replaceFrameMesh()
            {
                return replaceFrameMesh;
            }

            bool get_scoreSubmissionDisabled()
            {
                return scoreSubmissionDisabled;
            }

            bool get_moreThan1CoreMat()
            {
                return moreThan1CoreMat;
            }

            bool get_moreThan1FrameMat()
            {
                return moreThan1FrameMat;
            }

        private:
        
            bool replaceCoreMaterial;
            bool replaceCoreMesh;
            bool disableFrame;
            bool disableFakeGlow;
            bool replaceFrameMaterial;
            bool replaceFrameMesh;
            bool scoreSubmissionDisabled;

            bool moreThan1CoreMat;
            bool moreThan1FrameMat;
    };
}