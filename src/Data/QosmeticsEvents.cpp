#include "Data/QosmeticsEvents.hpp"

#define CSSTR(value...) il2cpp_utils::createcsstr(value)
namespace Qosmetics
{
    void QosmeticsEvent::RunEvent(UnityEngine::GameObject* object)
    {
        switch (effect)
        {
            case QosmeticsEvent::EffectType::GameObject:
                object->SetActive(boolean);
                break;
            case QosmeticsEvent::EffectType::Bool:
                if (animator == nullptr) animator = object->GetComponent<UnityEngine::Animator*>();
                animator->SetBool(CSSTR(nameToSet), boolean);
                break;
            case QosmeticsEvent::EffectType::Trigger:
                if (animator == nullptr) animator = object->GetComponent<UnityEngine::Animator*>();
                animator->SetTrigger(CSSTR(nameToSet));
                break;
        }
    }
}