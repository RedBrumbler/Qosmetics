#include "Config.hpp"
#include "Utils/PointerUtils.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Collider.hpp"
#include "Utils/UnityUtils.hpp"

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

static Il2CppString* MenuHandle = nullptr;

using namespace UnityEngine;
namespace PointerUtils
{
    void SetPointerSize(Transform* pointer)
    {
        Vector3 localPos = Vector3(0.0f, 0.0f, -0.1f * (1.0f - config.saberConfig.menuPointerSize));
        Vector3 localScale = Vector3(config.saberConfig.saberWidth, config.saberConfig.saberWidth, 1.0f) * config.saberConfig.menuPointerSize;
        Vector3 localAngles = Vector3::get_zero();

        pointer->set_localPosition(localPos);
        pointer->set_localScale(localScale);
        pointer->set_localEulerAngles(localAngles);
    }

    void HideMenuHandle(Transform* controller, bool doHide)
    {
        if (!MenuHandle) MenuHandle = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("MenuHandle");
        if (!controller) return;
        UnityUtils::HideRenderersOnObject(controller->Find(MenuHandle), doHide);
    }

    void RemoveColliders(Transform* pointer)
    {
        auto colliders = pointer->get_gameObject()->GetComponentsInChildren<Collider*>(true);
        
        int length = colliders.Length();
        for (int i = 0; i < length; i++) Object::DestroyImmediate(colliders.get(i));
    }

}
