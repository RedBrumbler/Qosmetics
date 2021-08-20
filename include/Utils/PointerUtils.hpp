#pragma once

#include "UnityEngine/Transform.hpp"

namespace PointerUtils
{
    void SetPointerSize(UnityEngine::Transform* pointer);
    void HideMenuHandle(UnityEngine::Transform* controller, bool doHide = true);
}