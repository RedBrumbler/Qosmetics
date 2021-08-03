#include "conditional-dependencies/shared/main.hpp"
#include "Types/Trail/TrailHelper.hpp"
#include "GlobalNamespace/BeatmapLevelPackCollectionSO.hpp"
#pragma GCC diagnostic ignored "-Wreturn-type-c-linkage"
#pragma GCC diagnostic push

EXPOSE_API(TrailHelper_GetOrAddTrail, void, Il2CppObject* trailHelper, bool remake) {
    ((Qosmetics::TrailHelper*)trailHelper)->GetOrAddTrail(remake);
}

EXPOSE_API(TrailHelper_Init, void, Il2CppObject* trailHelper, Il2CppObject* colorManager, Il2CppObject* saberModelController) {
    ((Qosmetics::TrailHelper*)trailHelper)->Init((Qosmetics::ColorManager*)colorManager, (GlobalNamespace::SaberModelController*)saberModelController);
}

EXPOSE_API(TrailHelper_TrailSetup, void, Il2CppObject* trailHelper) {
    ((Qosmetics::TrailHelper*)trailHelper)->TrailSetup();
}

EXPOSE_API(TrailHelper_SetTrailActive, void, Il2CppObject* trailHelper, bool active) {
    ((Qosmetics::TrailHelper*)trailHelper)->SetTrailActive(active);
}

EXPOSE_API(TrailHelper_UpdateColors, void, Il2CppObject* trailHelper) {
    ((Qosmetics::TrailHelper*)trailHelper)->UpdateColors();
}

EXPOSE_API(TrailHelper_GetColor, void, Il2CppObject* trailHelper, int colorType, Sombrero::FastColor& out) {
    out = ((Qosmetics::TrailHelper*)trailHelper)->GetColor(colorType);
}

EXPOSE_API(TrailHelper_SetColors, void, Il2CppObject* trailHelper, const UnityEngine::Color& leftColor, const UnityEngine::Color& rightColor) {
    ((Qosmetics::TrailHelper*)trailHelper)->SetColors(leftColor, rightColor);
}
#pragma GCC diagnostic pop
