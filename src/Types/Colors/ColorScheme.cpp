#include "Types/Colors/ColorScheme.hpp"

DEFINE_CLASS(Qosmetics::ColorScheme);

namespace Qosmetics
{
    void ColorScheme::CopyFromBaseGame(GlobalNamespace::ColorScheme* orig)
    {
        if (orig)
        {
            this->saberAColor = orig->saberAColor;
            this->saberBColor = orig->saberBColor;
            this->noteAColor = orig->saberAColor;
            this->noteBColor = orig->saberBColor;
            this->obstaclesColor = orig->obstaclesColor;
            this->trailAColor = orig->saberAColor;
            this->trailBColor = orig->saberBColor;
        }
        else
        {
            this->saberAColor = UnityEngine::Color(0.75f, 0.09f, 0.09f, 1.0f);
            this->saberBColor = UnityEngine::Color(0.0f, 0.46f, 0.82f, 1.0f);
            this->noteAColor = this->saberAColor;
            this->noteBColor = this->saberBColor;
            this->trailAColor = this->saberAColor;
            this->trailBColor = this->saberBColor;
            this->obstaclesColor = this->saberAColor;
        }
    }

    void ColorScheme::CopyFromCustom(Qosmetics::ColorScheme* orig)
    {
        if (orig)
        {
            this->saberAColor = orig->saberAColor;
            this->saberBColor = orig->saberBColor;
            this->noteAColor = orig->noteAColor;
            this->noteBColor = orig->noteBColor;
            this->obstaclesColor = orig->obstaclesColor;
            this->trailAColor = orig->trailAColor;
            this->trailBColor = orig->trailBColor;
        }
        else
        {
            this->saberAColor = UnityEngine::Color(0.75f, 0.09f, 0.09f, 1.0f);
            this->saberBColor = UnityEngine::Color(0.0f, 0.46f, 0.82f, 1.0f);
            this->noteAColor = this->saberAColor;
            this->noteBColor = this->saberBColor;
            this->trailAColor = this->saberAColor;
            this->trailBColor = this->saberBColor;
            this->obstaclesColor = this->saberAColor;
        }
    }

    void ColorScheme::SetLeftColor(UnityEngine::Color color)
    {
        this->saberAColor = color;
        this->trailAColor = color;
        this->noteAColor = color;
    }

    void ColorScheme::SetRightColor(UnityEngine::Color color)
    {
        this->saberBColor = color;
        this->trailBColor = color;
        this->noteBColor = color;
    }
    void ColorScheme::ctor()
    {
        CopyFromBaseGame(nullptr);
    }

    void ColorScheme::ctorbasegame(GlobalNamespace::ColorScheme* scheme)
    {
        CopyFromBaseGame(scheme);
    }

    void ColorScheme::ctorcustom(Qosmetics::ColorScheme* scheme)
    {
        CopyFromCustom(scheme);
    }
}