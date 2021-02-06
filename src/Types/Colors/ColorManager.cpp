#include "Types/Colors/ColorManager.hpp"
#include "QosmeticsLogger.hpp"

DEFINE_CLASS(Qosmetics::ColorManager);

using namespace UnityEngine;

#define INFO(value...) QosmeticsLogger::GetContextLogger("ColorManager").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("ColorManager").error(value)

namespace Qosmetics
{
    void ColorManager::SetColorSchemeFromBase(GlobalNamespace::ColorScheme* base)
    {
        this->colorScheme->CopyFromBaseGame(base);
        UpdateAllColors();
    }

    void ColorManager::SetColorSchemeFromCustom(Qosmetics::ColorScheme* newScheme)
    {
        this->colorScheme = newScheme;
        UpdateAllColors();
    }

    void ColorManager::ctor()
    {
        Array<GlobalNamespace::ColorManager*>* colorManagers = UnityEngine::Object::FindObjectsOfType<GlobalNamespace::ColorManager*>();

        // if not nullptr return or 0 found
        if (colorManagers && colorManagers->Length() != 0)
        {
            INFO("Did find a color manager!");
            GlobalNamespace::ColorManager* last = colorManagers->values[colorManagers->Length() - 1];
            GlobalNamespace::ColorScheme* scheme = last ? last->colorScheme : nullptr; 
            if (this->colorScheme) this->colorScheme->CopyFromBaseGame(scheme);
            else this->colorScheme = CRASH_UNLESS(il2cpp_utils::New<Qosmetics::ColorScheme*, il2cpp_utils::CreationType::Manual>(scheme));
        }
        else
        {
            INFO("Didnt find any color managers...");
            this->colorScheme = CRASH_UNLESS(il2cpp_utils::New<Qosmetics::ColorScheme*, il2cpp_utils::CreationType::Manual>((GlobalNamespace::ColorScheme*)nullptr));
        }
    }

    void ColorManager::SetLeftSaberColor(Color color)
    {
        this->colorScheme->saberAColor = color;
        RunCallbacks(saberCallbacks);
        RunCallbacks(pointerCallbacks);
    }

    void ColorManager::SetRightSaberColor(Color color)
    {
        this->colorScheme->saberBColor = color;
        RunCallbacks(saberCallbacks);
        RunCallbacks(pointerCallbacks);
    }

    void ColorManager::SetLeftTrailColor(Color color)
    {
        this->colorScheme->trailAColor = color;
        RunCallbacks(trailCallbacks);
    }

    void ColorManager::SetRightTrailColor(Color color)
    {
        this->colorScheme->trailBColor = color;
        RunCallbacks(trailCallbacks);
    }

    void ColorManager::SetLeftNoteColor(Color color)
    {
        this->colorScheme->noteAColor = color;
        RunCallbacks(noteCallbacks);
    }

    void ColorManager::SetRightNoteColor(Color color)
    {
        this->colorScheme->noteBColor = color;
        RunCallbacks(noteCallbacks);
    }

    void ColorManager::SetLeftColor(Color color)
    {
        this->colorScheme->saberAColor = color;
        this->colorScheme->trailAColor = color;
        this->colorScheme->noteAColor = color;
        RunCallbacks(saberCallbacks);
        RunCallbacks(noteCallbacks);
        RunCallbacks(trailCallbacks);
    }

    void ColorManager::SetRightColor(Color color)
    {
        this->colorScheme->saberBColor = color;
        this->colorScheme->trailBColor = color;
        this->colorScheme->noteBColor = color;
        RunCallbacks(saberCallbacks);
        RunCallbacks(noteCallbacks);
        RunCallbacks(trailCallbacks);
    }

    void ColorManager::SetObstacleColor(Color color)
    {
        this->colorScheme->obstaclesColor = color;
        RunCallbacks(wallCallbacks);
    }

    void ColorManager::UpdateAllColors()
    {
        RunCallbacks(saberCallbacks);
        RunCallbacks(trailCallbacks);
        RunCallbacks(noteCallbacks);
        RunCallbacks(wallCallbacks);
        RunCallbacks(pointerCallbacks);
    }

    Color ColorManager::ColorForNoteType(GlobalNamespace::ColorType type)
    {
        switch (type.value)
        {
            case 0:
                return this->colorScheme->noteAColor;
            case 1:
                return this->colorScheme->noteBColor;
            default:
                return UnityEngine::Color::get_black();
        }
    }

    Color ColorManager::ColorForSaberType(GlobalNamespace::ColorType type)
    {
        switch (type.value)
        {
            case 0:
                return this->colorScheme->saberAColor;
            case 1:
                return this->colorScheme->saberBColor;
            default:
                return UnityEngine::Color::get_black();
        }
    }

    Color ColorManager::ColorForTrailType(GlobalNamespace::ColorType type)
    {
        switch (type.value)
        {
            case 0:
                return this->colorScheme->trailAColor;
            case 1:
                return this->colorScheme->trailBColor;
            default:
                return UnityEngine::Color::get_black();
        }
    }

    Color ColorManager::ColorForObstacle()
    {
        return this->colorScheme->obstaclesColor;
    }

    void ColorManager::RegisterCallback(std::function<void()> callback, callbackType type)
    {
        switch (type)
        {
            case saber:
                saberCallbacks.push_back(callback);
                break;
            case trail:
                trailCallbacks.push_back(callback);
                break;
            case note:
                noteCallbacks.push_back(callback);
                break;
            case wall:
                wallCallbacks.push_back(callback);
                break;
            case pointer:
                pointerCallbacks.push_back(callback);
                break;
            default:
                break;
        }
    }

    void ColorManager::ClearCallbacks()
    {
        saberCallbacks.clear();
        trailCallbacks.clear();
        noteCallbacks.clear();
        wallCallbacks.clear();
        pointerCallbacks.clear();
    }

    void ColorManager::RunCallbacks(std::vector<std::function<void()>>& callbacks)
    {
        for (auto& c : callbacks)
        {
            c();
        }
    }
}