#include "Qosmetic/QosmeticsColorManager.hpp"
#include "Qosmetic/QuestNote.hpp"
#include "Qosmetic/QuestSaber.hpp"
#include "Qosmetic/QuestWall.hpp"
#include "Qosmetic/QosmeticsColorScheme.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/ColorManager.hpp"

#include "UnityEngine/Object.hpp"

DEFINE_CLASS(Qosmetics::ColorManager);

extern bool sabersEnabled;
extern bool notesEnabled;
extern bool wallsEnabled;
static bool setColors = true;

namespace Qosmetics
{
    void ColorManager::SetLeftSaberColor(GlobalNamespace::SimpleColorSO* colorSO)
    {
        if (!sabersEnabled) return;
        this->colorScheme->saberAColor = colorSO->get_color();
        QuestSaber::HandleColorsDidChangeEvent();
    }

    void ColorManager::SetRightSaberColor(GlobalNamespace::SimpleColorSO* colorSO)
    {
        if (!sabersEnabled) return;
        this->colorScheme->saberBColor = colorSO->get_color();
        QuestSaber::HandleColorsDidChangeEvent();
    }

    void ColorManager::SetLeftTrailColor(GlobalNamespace::SimpleColorSO* colorSO)
    {
        if (!sabersEnabled) return;
        this->colorScheme->trailBColor = colorSO->get_color();
        QuestSaber::HandleColorsDidChangeEvent();
    }

    void ColorManager::SetRightTrailColor(GlobalNamespace::SimpleColorSO* colorSO)
    {
        if (!sabersEnabled) return;
        this->colorScheme->trailAColor = colorSO->get_color();
        QuestSaber::HandleColorsDidChangeEvent();
    }

    void ColorManager::SetLeftNoteColor(GlobalNamespace::SimpleColorSO* colorSO)
    {
        if (!notesEnabled) return;
        this->colorScheme->noteAColor = colorSO->get_color();
        QuestNote::HandleColorsDidChangeEvent();
    }

    void ColorManager::SetRightNoteColor(GlobalNamespace::SimpleColorSO* colorSO)
    {
        if (!notesEnabled) return;
        this->colorScheme->noteBColor = colorSO->get_color();
        QuestNote::HandleColorsDidChangeEvent();
    }

    void ColorManager::SetObstacleColor(GlobalNamespace::SimpleColorSO* colorSO)
    {
        if (!notesEnabled) return;
        this->colorScheme->obstaclesColor = colorSO->get_color();
        QuestWall::HandleColorsDidChangeEvent();
    }

    void ColorManager::UpdateAllColors()
    {
        if (sabersEnabled) QuestSaber::HandleColorsDidChangeEvent();
        if (notesEnabled) QuestNote::HandleColorsDidChangeEvent();
        if (wallsEnabled) QuestWall::HandleColorsDidChangeEvent();
    }

    void ColorManager::SetLeftColor(GlobalNamespace::SimpleColorSO* colorSO)
    {
        this->colorScheme->SetLeftColor(colorSO->get_color());
        if (sabersEnabled) QuestSaber::HandleColorsDidChangeEvent();
        if (notesEnabled) QuestNote::HandleColorsDidChangeEvent();
    }

    void ColorManager::SetRightColor(GlobalNamespace::SimpleColorSO* colorSO)
    {
        this->colorScheme->SetRightColor(colorSO->get_color());
        if (sabersEnabled) QuestSaber::HandleColorsDidChangeEvent();
        if (notesEnabled) QuestNote::HandleColorsDidChangeEvent();
    }


    UnityEngine::Color ColorManager::ColorForSaberType(GlobalNamespace::ColorType type)
    {
        switch (type.value)
        {
            case 0:
                return this->colorScheme->saberAColor;
                break;
            case 1:
                return this->colorScheme->saberBColor;
                break;
            default:
                return UnityEngine::Color::get_black();
                break;
        }
    }

    UnityEngine::Color ColorManager::ColorForNoteType(GlobalNamespace::ColorType type)
    {
        switch (type.value)
        {
            case 0:
                return this->colorScheme->noteAColor;
                break;
            case 1:
                return this->colorScheme->noteBColor;
                break;
            default:
                return UnityEngine::Color::get_black();
                break;
        }
    }

    UnityEngine::Color ColorManager::ColorForTrailType(GlobalNamespace::ColorType type)
    {
        switch (type.value)
        {
            case 0:
                return this->colorScheme->trailAColor;
                break;
            case 1:
                return this->colorScheme->trailBColor;
                break;
            default:
                return UnityEngine::Color::get_black();
                break;
        }
    }

    UnityEngine::Color ColorManager::ColorForObstacle()
    {
        return this->colorScheme->obstaclesColor;
    }

    void ColorManager::SetColorScheme(GlobalNamespace::ColorScheme* orig)
    {
        this->colorScheme->Copy(orig);
        this->UpdateAllColors();
    }

    void ColorManager::Init()
    {
        if (!setColors) return;
        GlobalNamespace::ColorManager* BaseGameManager = UnityEngine::Object::FindObjectOfType<GlobalNamespace::ColorManager*>();
        GlobalNamespace::ColorScheme* orig = BaseGameManager->colorScheme;
        Qosmetics::ColorManager* QosmeticsManager = UnityEngine::Object::FindObjectOfType<Qosmetics::ColorManager*>();

        if (!QosmeticsManager->colorScheme)
        {
            QosmeticsManager->colorScheme = CRASH_UNLESS(il2cpp_utils::New<Qosmetics::ColorScheme*>(orig));
        }
        else
        {
            QosmeticsManager->colorScheme->saberAColor = orig->saberAColor;
            QosmeticsManager->colorScheme->saberBColor = orig->saberBColor;
            QosmeticsManager->colorScheme->noteAColor = orig->saberAColor;
            QosmeticsManager->colorScheme->noteBColor = orig->saberBColor;
            QosmeticsManager->colorScheme->obstaclesColor = orig->obstaclesColor;
            QosmeticsManager->colorScheme->trailAColor = orig->saberAColor;
            QosmeticsManager->colorScheme->trailBColor = orig->saberBColor;
        }
        setColors = false;
    }

    void ColorManager::Menu()
    {
        Qosmetics::ColorManager* qosmeticsColorManager = UnityEngine::Object::FindObjectOfType<Qosmetics::ColorManager*>();
        if (!qosmeticsColorManager)
        {
            UnityEngine::GameObject* newObject = UnityEngine::GameObject::New_ctor();//UnityEngine::Object::Instantiate(UnityEngine::GameObject::New_ctor());
            newObject->set_name(il2cpp_utils::createcsstr("QosmeticsColorManager"));
            newObject->DontDestroyOnLoad(newObject);
            qosmeticsColorManager = newObject->AddComponent<Qosmetics::ColorManager*>();
        }
        setColors = true;
    }
}