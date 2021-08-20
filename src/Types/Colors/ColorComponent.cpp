#include "Types/Colors/ColorComponent.hpp"
#include "Types/Trail/TrailHelper.hpp"

#include "chroma/shared/SaberAPI.hpp"
#include "chroma/shared/NoteAPI.hpp"
#include "chroma/shared/ObstacleAPI.hpp"

#include "Utils/SaberUtils.hpp"
#include "Utils/NoteUtils.hpp"
#include "Utils/WallUtils.hpp"

DEFINE_TYPE(Qosmetics, ColorComponent);

namespace Qosmetics
{
    void ColorComponent::UpdateColors()
    {
        switch (objectType)
        {
            case 0://saber
                UpdateSaberColors();
                break;
            case 1://note
                UpdateNoteColors();
                break;
            case 2://wall
                UpdateWallColors();
                break;
            case 3://pointer
                break;
            case 4://platform
                break;
            default:
                break;
        }
    }

    void ColorComponent::SetColors(Sombrero::FastColor leftColor, Sombrero::FastColor rightColor)
    {
        switch (objectType)
        {
            case 0://saber
                SetSaberColors(leftColor, rightColor);
                break;
            case 1://note
                SetNoteColors(leftColor, rightColor);
                break;
            case 2://wall
                SetWallColor(leftColor);
                break;
            case 3://pointer
                break;
            case 4://platform
                break;
            default:
                break;
        }
    }

    void ColorComponent::Init(Qosmetics::ColorManager* colorManager, int objectType, int colorType)
    {
        this->colorManager = colorManager;
        this->objectType = objectType;
        this->colorType = colorType;
    }

    void ColorComponent::SetSaberColors(const Sombrero::FastColor& leftColor, const Sombrero::FastColor& rightColor)
    {
        if (colorType) // not 0, right
        {
            SaberUtils::SetColors(get_gameObject(), rightColor, leftColor);
        }
        else
        {
            SaberUtils::SetColors(get_gameObject(), leftColor, rightColor);
        }

        auto trails = get_gameObject()->GetComponentsInChildren<Qosmetics::TrailHelper*>();
        int length = trails->Length();
        for (int i = 0; i < length; i++)
        {
            trails->values[i]->SetColors(leftColor, rightColor);
        }
    }

    void ColorComponent::SetNoteColors(const Sombrero::FastColor& leftColor, const Sombrero::FastColor& rightColor)
    {
        if (colorType) // not 0, right
        {
            NoteUtils::SetColors(get_gameObject(), rightColor, leftColor);
        }
        else
        {
            NoteUtils::SetColors(get_gameObject(), leftColor, rightColor);
        }
    }

    void ColorComponent::SetWallColor(const Sombrero::FastColor& color)
    {
        WallUtils::SetColors(get_gameObject(), color);
    }

    void ColorComponent::UpdateSaberColors()
    {
        if (!colorManager) return;
        auto sabersColorOptional = Chroma::SaberAPI::getGlobalSabersColorSafe();
        
        Sombrero::FastColor leftColor = sabersColorOptional.first.value_or(colorManager->ColorForSaberType(0));
        Sombrero::FastColor rightColor = sabersColorOptional.second.value_or(colorManager->ColorForSaberType(1));

        SetSaberColors(leftColor, rightColor);
    }

    void ColorComponent::UpdateNoteColors()
    {
        if (!colorManager) return;

        Sombrero::FastColor leftColor = Chroma::NoteAPI::getNoteColorSafe(0).value_or(colorManager->ColorForNoteType(0));
        Sombrero::FastColor rightColor = Chroma::NoteAPI::getNoteColorSafe(1).value_or(colorManager->ColorForNoteType(1));

        SetNoteColors(leftColor, rightColor);
    }

    void ColorComponent::UpdateWallColors()
    {
        if (!colorManager) return;

        SetWallColor(colorManager->ColorForObstacle());
    }
}