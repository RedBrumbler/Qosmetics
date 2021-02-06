#pragma once
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Component.hpp"
#include "UnityEngine/Renderer.hpp"

class UnityUtils
{
    public:

        template<typename T>
        static T GetAddComponent(UnityEngine::GameObject* obj);

        template<typename T>
        static T FindAddComponent(bool manual = false);

        static void HideRenderersOnObject(UnityEngine::GameObject* obj, bool doHide = true);

        static void HideRenderer(UnityEngine::Renderer* renderer, bool doHide = true);
};