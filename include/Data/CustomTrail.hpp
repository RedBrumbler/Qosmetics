#pragma once
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "modLoader/shared/modloader.hpp"
#include "UnityEngine/Material.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Component.hpp"
#include "GlobalNamespace/SaberTrail.hpp"
#include "GlobalNamespace/SaberTrailRenderer.hpp"
#include "UnityEngine/Color.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "Utils/UnityUtils.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include "UnityEngine/MeshFilter.hpp"
#include "Logging/SaberLogger.hpp"
#include "GlobalNamespace/SaberMovementData.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/IBladeMovementData.hpp"
#include "GlobalNamespace/BladeMovementDataElement.hpp"
#include "System/Math.hpp"
#include "GlobalNamespace/TrailElementCollection.hpp"

#include "Data/QosmeticsTrail.hpp"
namespace Qosmetics
{
    class CustomTrail
    {
        public:
            static Logger& getLogger() 
            {
                return SaberLogger::GetLogger();
            }

            enum ColorType 
            {
                leftSaber = 0,
                rightSaber = 1,
                custom = 2
            };

            /// @brief ctor for customtrail
            /// @param name string reference from the name field in the trail config
            /// @param type int reference for color type, 0 = leftsaber, 1 = rightsaber, 2 = custom color
            /// @param trailColor the color the trail would be with colortype 2
            /// @param multiplierColor color to multiply by, always used
            /// @param length the length of the trail, in the weapontrail passed in as width for some reason
            /// @param whiteStep
            CustomTrail(const std::string &name, const int &type, const UnityEngine::Color &trailColor, const UnityEngine::Color &multiplierColor, const int &length, const int &whiteStep) 
            {
                ObjectPath = name;
                this->trailColor = trailColor;
                this->multiplierColor = multiplierColor;
                this->length = length;
                this->whiteStep = whiteStep;
                trailType = (ColorType)type;
            }

            /// @brief finds the objectpath in the given transform
            /// @param saber LeftSaber or Rightsaber generally
            void FindMaterial(UnityEngine::Transform* saber);

            void set_material(UnityEngine::Material* material) 
            {
                this->trailMaterial = material;
            }

            const std::string get_objectPath() 
            {
                return ObjectPath;
            }

            UnityEngine::Material* get_material() 
            {
                return trailMaterial;
            }

            const int get_length() 
            {
                return length;
            }

            const ColorType get_type() 
            {
                return trailType;
            }

            const UnityEngine::Color get_trailColor() 
            {
                return trailColor;
            }

            const UnityEngine::Color get_multiplierColor() 
            {
                return multiplierColor;
            }

            const UnityEngine::Color get_color()
            {
                // premultiplies the color for us
                return trailColor * multiplierColor;
            }

            int get_whiteStep()
            {
                return whiteStep;
            }  
        private:
            std::string ObjectPath;
            ColorType trailType; // 0 = leftTrail, 1 = rightTrail, 2 = custom color
            UnityEngine::Material* trailMaterial = nullptr;
            UnityEngine::Color trailColor;
            UnityEngine::Color multiplierColor;
            int length = 0;
            int whiteStep = 0;
    };
}

