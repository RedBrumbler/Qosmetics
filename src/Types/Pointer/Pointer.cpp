#include "Config.hpp"
#include "Types/Pointer/Pointer.hpp"
#include "QosmeticsLogger.hpp"

#include "Containers/SingletonContainer.hpp"

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "Utils/PointerUtils.hpp"
#include "Utils/SaberUtils.hpp"
#include "Utils/UnityUtils.hpp"
#include "UnityEngine/Resources.hpp"
#include "Types/Trail/QosmeticsTrail.hpp"
#include "Types/Trail/AltTrail.hpp"
#include "Types/Trail/TrailHelper.hpp"
#include "chroma/shared/SaberAPI.hpp"

DEFINE_TYPE(Qosmetics, Pointer);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Pointer").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Pointer").error(value)

using namespace UnityEngine;

static Il2CppString* LeftPointer = nullptr;
static Il2CppString* RightPointer = nullptr;
namespace Qosmetics
{
    void Pointer::ctor()
    {
        modelManager = nullptr;
        colorManager = nullptr;
        vrController = nullptr;
    }
    
    void Pointer::AddToAll()
    {
        ArrayW<GlobalNamespace::VRController*> controllers = Resources::FindObjectsOfTypeAll<GlobalNamespace::VRController*>();
        int controllerCount = controllers.Length();
        for (int i = 0; i < controllerCount; i++)
        {
            GlobalNamespace::VRController* controller = controllers.get(i);
            if (!controller) continue;
            if (!(controller->get_node().value == 4 || controller->get_node().value == 5)) continue;
            Qosmetics::Pointer* pointer = controller->get_gameObject()->GetComponent<Qosmetics::Pointer*>();
            
            if (!pointer)
            {
                pointer = controller->get_gameObject()->AddComponent<Qosmetics::Pointer*>();
                pointer->Init(SingletonContainer::get_saberManager(), SingletonContainer::get_colorManager());
            }    

            pointer->vrController = controller;
            pointer->Replace();
        }
    }
    

    void Pointer::UpdateAll(bool reinstantiate)
    {
        ArrayW<Qosmetics::Pointer*> pointers = Resources::FindObjectsOfTypeAll<Qosmetics::Pointer*>();

        int pointerCount = pointers.Length();
        for (int i = 0; i < pointerCount; i++)
        {
            Qosmetics::Pointer* pointer = pointers.get(i);
            if (!pointer) continue;
            pointer->UpdateModel(reinstantiate);
        }
    }

    void Pointer::Replace(bool reinstantiate)
    {
        INFO("ModelManager: %p", modelManager);
        auto type = modelManager ? modelManager->get_type() : ItemType::invalid; 
        if (!modelManager || (type != ItemType::saber && type != ItemType::pointer) || !config.saberConfig.enableMenuPointer)
        {
            ERROR("Modelmanager was %p, or wrong type", modelManager);
            replaced = true;
            Restore();
            return;
        }

        if (replaced)
        {
            UpdateModel();
            return;
        }
        if (!vrController) return;
        PointerUtils::HideMenuHandle(get_transform(), true);

        bool isLeft = vrController->get_node().value == 4;

        if (!LeftPointer) LeftPointer = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("LeftPointer");
        if (!RightPointer) RightPointer = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("RightPointer");
        
        Transform* prefab = isLeft ? modelManager->get_leftSaber() : modelManager->get_rightSaber();
        if (prefab)
        {
            Il2CppString* name = isLeft ? LeftPointer : RightPointer;
            PointerUtils::RemoveColliders(prefab);
            prefab->SetParent(get_transform());
            prefab->get_gameObject()->set_name(name);
            PointerUtils::SetPointerSize(prefab);

            SetupTrails();

            replaced = true;
            UpdateModel(false, true);
        }
    }

    void Pointer::UpdateModel(bool reinstantiate, bool firstUpdate)
    {
        if (!vrController) return;
        bool isLeft = vrController->get_node().value == 4;

        // if pointer disabled, or model not valid, just restore always
        if (!config.saberConfig.enableMenuPointer || (modelManager && !modelManager->get_item().get_descriptor().isValid()))
        {
            Restore();
            return;
        }
        
        if (!LeftPointer) LeftPointer = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("LeftPointer");
        if (!RightPointer) RightPointer = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("RightPointer");
        Transform* oldPointer = isLeft ? get_transform()->Find(LeftPointer) : get_transform()->Find(RightPointer);

        if (reinstantiate || !oldPointer) 
        {
            // destroy previous version of the object
            if (replaced && oldPointer)
            {
                Restore();
            }

            // replace it with the new one
            replaced = false;
            Replace();
            return;
        }
        else if (!replaced) return;

        // actually update stuff about the model
        PointerUtils::SetPointerSize(oldPointer);

        if (!firstUpdate)
        {
            ArrayW<QosmeticsTrail*> trails = GetComponentsInChildren<QosmeticsTrail*>(true);
            if (trails)
            {
                int trailLength = trails.Length();
                for (int i = 0; i < trailLength; i++)
                {
                    Object::DestroyImmediate(trails.get(i));
                    //QosmeticsTrail* trail = trails->values[i];
                    //if (trail) trail->Reset();
                }
            }

            SetupTrails();
        }

        UpdateColors();
    }

    void Pointer::UpdateColors()
    {
        bool isLeft = vrController->get_node().value == 4;
        int colorType = (int)isLeft;

        if (!LeftPointer) LeftPointer = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("LeftPointer");
        if (!RightPointer) RightPointer = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("RightPointer");

        Transform* oldPointer = colorType == 0 ? get_transform()->Find(LeftPointer) : get_transform()->Find(RightPointer);

        auto sabersColorOptional = Chroma::SaberAPI::getGlobalSabersColorSafe();
        
        Color thisColor;
        Color otherColor;
        
        switch (colorType)
        {   
            // left saber
            case 0:
                thisColor = (sabersColorOptional.first != std::nullopt) ? *(sabersColorOptional.first) : colorManager->ColorForSaberType(colorType);
                otherColor = (sabersColorOptional.second != std::nullopt) ? *(sabersColorOptional.second) : colorManager->ColorForSaberType(1 - colorType);
                break;
            // right saber
            case 1:
                thisColor = (sabersColorOptional.second != std::nullopt) ? *(sabersColorOptional.second) : colorManager->ColorForSaberType(colorType);
                otherColor = (sabersColorOptional.first != std::nullopt) ? *(sabersColorOptional.first) : colorManager->ColorForSaberType(1 - colorType);
                break;
        }

        if (oldPointer) SaberUtils::SetColors(oldPointer->get_gameObject(), thisColor, otherColor);

        //Array<Qosmetics::QosmeticsTrail*>* trails = GetComponentsInChildren<Qosmetics::QosmeticsTrail*>(true);
        ArrayW<Qosmetics::AltTrail*> trails = GetComponentsInChildren<Qosmetics::AltTrail*>(true);

        int trailCount = trails.Length();
        for (int i = 0; i < trailCount; i++)
        {
            auto trail = trails.get(i);
            if (!trail) continue;
            //trail->UpdateColors();
        }
    }

    void Pointer::Restore()
    {
        if (!vrController) return;
        bool isLeft = vrController->get_node().value == 4;
        if (!LeftPointer) LeftPointer = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("LeftPointer");
        if (!RightPointer) RightPointer = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("RightPointer");

        PointerUtils::HideMenuHandle(get_transform(), false);
        Transform* oldPointer = isLeft ? get_transform()->Find(LeftPointer) : get_transform()->Find(RightPointer);
        if (replaced && oldPointer)
        {
            Object::Destroy(oldPointer->get_gameObject());
        }

        replaced = false;
    }

    void Pointer::Init(SaberManager* modelManager, ColorManager* colorManager)
    {
        this->modelManager = modelManager;
        this->colorManager = colorManager;
    }

    void Pointer::SetupTrails()
    {
        SaberConfig& itemConfig = modelManager->get_item().get_config();
        // if the saber has no custom trails, there is not a lot we can do here poggers
        if (!itemConfig.get_hasCustomTrails()) return;
        // if the trail type is not custom, return
        if (config.saberConfig.trailType != TrailType::custom) return;

        if (!vrController) return;
        bool isLeft = vrController->get_node().value == 4;
        Transform* oldPointer = isLeft ? get_transform()->Find(LeftPointer) : get_transform()->Find(RightPointer);
        std::vector<TrailConfig>& trails = isLeft ? itemConfig.get_leftTrails() : itemConfig.get_rightTrails();

        for (auto& trail : trails)
        {
            Il2CppString* trailPath = trail.get_name();
            Transform* trailObj = oldPointer->Find(trailPath);
            if (!trailObj) continue;
            auto helper = UnityUtils::GetAddComponent<Qosmetics::TrailHelper*>(trailObj->get_gameObject());
            helper->set_trailConfig(trail);
            helper->Init(colorManager, nullptr);
            helper->TrailSetup();
        }
    }

    void Pointer::OnApplicationFocus(bool hasFocus)
    {
        INFO("VRController: %p", vrController);
        
        if (!vrController) return;
        bool isLeft = vrController->get_node().value == 4;
        Transform* oldPointer = isLeft ? get_transform()->Find(LeftPointer) : get_transform()->Find(RightPointer);

        if (oldPointer) // if custom
        {
            UnityUtils::HideRenderersOnObject(oldPointer, !hasFocus);
            if (hasFocus) PointerUtils::HideMenuHandle(get_transform()); // if coming back from pause
        }
    }
}