#pragma once
#include "Types/Qosmetic/ModelManager.hpp"
#include "Types/Saber/SaberItem.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("Saber Manager").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Saber Manager").error(value)

DECLARE_CLASS_CODEGEN(Qosmetics, SaberManager, UnityEngine::MonoBehaviour, 
        DECLARE_METHOD(UnityEngine::GameObject*, GetActivePrefab);
        DECLARE_METHOD(void, SetActiveModel, Il2CppString* csname);
        DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::GameObject*, prefab, nullptr);
    
        DECLARE_METHOD(static Il2CppString*, get_leftSaberName);
        DECLARE_METHOD(static Il2CppString*, get_rightSaberName);
        DECLARE_METHOD(UnityEngine::Transform*, get_leftSaber);
        DECLARE_METHOD(UnityEngine::Transform*, get_rightSaber);
    public:
        void internalSetActiveModel(std::string name);
        void SetActiveSaber(std::string name)
        {
            internalSetActiveModel(name);
        }

        ItemType get_type()
        {
            return activeItem->get_type();
        }
        
        SaberItem& get_item()
        {
            INFO("Item ptr: %p", activeItem);
            return *this->activeItem;
        }
        
        
        
        
    protected:
        SaberItem* activeItem = nullptr;

    REGISTER_FUNCTION(SaberManager,
        REGISTER_METHOD(SetActiveModel);
        REGISTER_METHOD(GetActivePrefab);
        REGISTER_METHOD(get_leftSaberName);
        REGISTER_METHOD(get_rightSaberName);
        REGISTER_METHOD(get_leftSaber);
        REGISTER_METHOD(get_rightSaber);
        REGISTER_FIELD(prefab);
    )
)

#undef INFO
#undef ERROR