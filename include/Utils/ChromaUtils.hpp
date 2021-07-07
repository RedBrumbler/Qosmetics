#pragma once

#include "QosmeticsLogger.hpp"
#include "chroma/shared/SaberAPI.hpp"
#include "chroma/shared/ObstacleAPI.hpp"
#include "chroma/shared/NoteAPI.hpp"

namespace GlobalNamespace {
    class SaberModelController;
}

#define INFO(value...) QosmeticsLogger::GetContextLogger("Chroma Utils").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Chroma Utils").error(value)

namespace ChromaUtils
{
    inline void registerSaberCallback(const ThinVirtualLayer<void (void*, int, GlobalNamespace::SaberModelController*, UnityEngine::Color)>& callback)
    {
        auto ev = Chroma::SaberAPI::getSaberChangedColorCallbackSafe();

        if (ev)
        {
            auto& callbackEvent = (*ev).get();

            callbackEvent += callback;
        }
    }

    inline void setSaberColoredByChroma(GlobalNamespace::SaberModelController* modelController, bool doColor)
    {
        Chroma::SaberAPI::setSaberColorable(modelController, !doColor);
    }

    inline void setObstacleColoredByChroma(bool doColor)
    {
        Chroma::ObstacleAPI::setObstacleColorable(!doColor);
    }

    inline void setNoteColoredByChroma(bool doColor)
    {
        Chroma::NoteAPI::setNoteColorable(!doColor);
    }
}

#undef INFO
#undef ERROR