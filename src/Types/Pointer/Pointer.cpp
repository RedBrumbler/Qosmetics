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
    void Pointer::Awake()
    {
        modelManager = nullptr;
        colorManager = nullptr;
        vrController = nullptr;
    }
    
    void Pointer::AddToAll()
    {
        Array<GlobalNamespace::VRController*>* controllers = Resources::FindObjectsOfTypeAll<GlobalNamespace::VRController*>();
        int controllerCount = controllers->Length();
        for (int i = 0; i < controllerCount; i++)
        {
            GlobalNamespace::VRController* controller = controllers->values[i];
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
        Array<Qosmetics::Pointer*>* pointers = Resources::FindObjectsOfTypeAll<Qosmetics::Pointer*>();

        int pointerCount = pointers->Length();
        for (int i = 0; i < pointerCount; i++)
        {
            Qosmetics::Pointer* pointer = pointers->values[i];
            if (!pointer) continue;
            pointer->UpdateModel(reinstantiate);
        }
    }

    void Pointer::Replace(bool reinstantiate)
    {
        if (!modelManager || (modelManager->get_type() != ItemType::saber && modelManager->get_type() != ItemType::pointer) || (modelManager->get_item().get_type() != ItemType::saber && modelManager->get_item().get_type() != ItemType::pointer) || !config.saberConfig.enableMenuPointer)
        {
            ERROR("Modelmanager was nullptr, or wrong type");
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
            Array<QosmeticsTrail*>* trails = GetComponentsInChildren<QosmeticsTrail*>(true);
            if (trails)
            {
                int trailLength = trails->Length();
                for (int i = 0; i < trailLength; i++)
                {
                    Object::DestroyImmediate(trails->values[i]);
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
        Array<Qosmetics::AltTrail*>* trails = GetComponentsInChildren<Qosmetics::AltTrail*>(true);

        int trailCount = trails->Length();
        for (int i = 0; i < trailCount; i++)
        {
            auto trail = trails->values[i];
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
            auto altTrail = UnityUtils::GetAddComponent<Qosmetics::AltTrail*>(trailObj->get_gameObject());
            auto helper = UnityUtils::GetAddComponent<Qosmetics::TrailHelper*>(trailObj->get_gameObject());
            helper->set_trailConfig(&trail);
            helper->Init(colorManager);
            helper->TrailSetup();

            /*
            QosmeticsTrail* trailComponent = UnityUtils::GetAddComponent<Qosmetics::QosmeticsTrail*>(trailObj->get_gameObject());
            trailComponent->SetColorManager(colorManager);
            trailComponent->SetTrailConfig(&trail);
            */
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