#pragma once
#include "custom-types/shared/macros.hpp"
#include "Data/Descriptor.hpp"
#include "Data/QosmeticItem.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics, ModelManager, Il2CppObject,
        DECLARE_METHOD(virtual UnityEngine::GameObject*, GetActivePrefab);
        DECLARE_METHOD(virtual void, SetActiveModel, Il2CppString* csbane);
        
    public:
        virtual QosmeticItem& get_item()
        {
            return *activeItem;
        }
        
        ItemType get_type()
        {
            return activeItem->get_type();
        }

        virtual void internalSetActiveModel(std::string name);
    protected:
        QosmeticItem* activeItem = nullptr;

    REGISTER_FUNCTION(ModelManager,
        REGISTER_METHOD(GetActivePrefab);
        REGISTER_METHOD(SetActiveModel);
    )
)