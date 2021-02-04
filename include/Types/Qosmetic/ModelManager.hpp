#pragma once
#include "custom-types/shared/macros.hpp"
#include "Data/Descriptor.hpp"
#include "Types/Qosmetic/QosmeticItem.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, ModelManager, UnityEngine::MonoBehaviour,
        DECLARE_METHOD(UnityEngine::GameObject*, GetActivePrefab);
        DECLARE_METHOD(void, SetActiveModel, Il2CppString* csname);
        DECLARE_METHOD(void, SetDefault);
        
    public:
        virtual QosmeticItem& get_item()
        {
            return *activeItem;
        }
        
        virtual ItemType get_type()
        {
            return activeItem->get_type();
        }

        virtual void internalSetActiveModel(std::string name);
    protected:
        QosmeticItem* activeItem = nullptr;

    REGISTER_FUNCTION(ModelManager,
        REGISTER_METHOD(GetActivePrefab);
        REGISTER_METHOD(SetActiveModel);
        REGISTER_METHOD(SetDefault);
    )
)