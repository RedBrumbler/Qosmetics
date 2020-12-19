#pragma once
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/macros.hpp"

#include "UnityEngine/Material.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Color.hpp"
#include "GlobalNamespace/SimpleColorSO.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "Qosmetic/QosmeticsColorScheme.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, ColorManager, UnityEngine::MonoBehaviour,

    public: static void Menu();
    public: static void Init();
    public: static void Init(GlobalNamespace::ColorManager* BaseGameManager);

    DECLARE_METHOD(void, SetColorScheme, GlobalNamespace::ColorScheme*);
    DECLARE_METHOD(void, Awake);

    DECLARE_METHOD(void, SetLeftSaberColor, GlobalNamespace::SimpleColorSO* colorSO);
    DECLARE_METHOD(void, SetRightSaberColor, GlobalNamespace::SimpleColorSO* colorSO);

    DECLARE_METHOD(void, SetLeftTrailColor, GlobalNamespace::SimpleColorSO* colorSO);
    DECLARE_METHOD(void, SetRightTrailColor, GlobalNamespace::SimpleColorSO* colorSO);

    DECLARE_METHOD(void, SetLeftNoteColor, GlobalNamespace::SimpleColorSO* colorSO);
    DECLARE_METHOD(void, SetRightNoteColor, GlobalNamespace::SimpleColorSO* colorSO);

    DECLARE_METHOD(void, SetLeftColor, GlobalNamespace::SimpleColorSO* colorSO);
    DECLARE_METHOD(void, SetRightColor, GlobalNamespace::SimpleColorSO* colorSO);

    DECLARE_METHOD(void, SetObstacleColor, GlobalNamespace::SimpleColorSO* colorSO);
    DECLARE_METHOD(void, UpdateAllColors);
    
    DECLARE_METHOD(UnityEngine::Color, ColorForNoteType, GlobalNamespace::ColorType type);
    DECLARE_METHOD(UnityEngine::Color, ColorForSaberType, GlobalNamespace::ColorType type);
    DECLARE_METHOD(UnityEngine::Color, ColorForTrailType, GlobalNamespace::ColorType type);
    DECLARE_METHOD(UnityEngine::Color, ColorForObstacle);

    DECLARE_INSTANCE_FIELD_DEFAULT(Qosmetics::ColorScheme*, colorScheme, nullptr);

    REGISTER_FUNCTION(ColorManager,
        REGISTER_METHOD(SetColorScheme);
        REGISTER_METHOD(Awake);

        REGISTER_METHOD(SetLeftSaberColor);
        REGISTER_METHOD(SetRightSaberColor);

        REGISTER_METHOD(SetLeftTrailColor);
        REGISTER_METHOD(SetRightTrailColor);

        REGISTER_METHOD(SetLeftNoteColor);
        REGISTER_METHOD(SetRightNoteColor);

        REGISTER_METHOD(SetLeftColor);
        REGISTER_METHOD(SetRightColor);

        REGISTER_METHOD(SetObstacleColor);
        REGISTER_METHOD(UpdateAllColors);

        REGISTER_METHOD(ColorForSaberType);
        REGISTER_METHOD(ColorForNoteType);
        REGISTER_METHOD(ColorForTrailType);
        REGISTER_METHOD(ColorForObstacle);
        
        REGISTER_FIELD(colorScheme);
    )
)