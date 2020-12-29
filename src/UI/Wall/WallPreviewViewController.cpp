#include "UI/Wall/WallPreviewViewController.hpp"
#include "config.hpp"
#include "Data/Descriptor.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Toggle_ToggleEvent.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/InputFieldView.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "Logging/UILogger.hpp"

#include "Data/WallData.hpp"
#include "Qosmetic/QuestWall.hpp"
#include "Utils/WallUtils.hpp"

#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/Vector4.hpp"

#define INFO(value...) UILogger::GetLogger().info(value)
extern config_t config;
DEFINE_CLASS(Qosmetics::WallPreviewViewController);

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

namespace Qosmetics
{
    void WallPreviewViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        if (previewprefab) 
        {
            Object::Destroy(previewprefab);
            previewprefab = nullptr;
        }
    }

    void WallPreviewViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            VerticalLayoutGroup* textlayout = BeatSaberUI::CreateVerticalLayoutGroup(get_transform());

            title = BeatSaberUI::CreateText(textlayout->get_transform(), "placeholder text");
            title->set_fontSize(10.0f);
        }
        UpdatePreview();
    }

    void WallPreviewViewController::UpdatePreview()
    {
        if (QuestWall::GetActiveWall())
        {
            if (previewprefab) 
            {
                Object::Destroy(previewprefab);
                previewprefab = nullptr;
            }
            WallData& selected = *QuestWall::GetActiveWall();
            Descriptor& wallDescriptor = *selected.get_descriptor();
            GameObject* prefab = selected.get_wallPrefab();
            
            if (!prefab)
            {
                title->set_text(il2cpp_utils::createcsstr("Loading .qwall File"));
                std::thread waitForLoadedPrefab([]{
                    bool complete = false;
                    while (!complete)
                    {
                        if (QuestWall::GetActiveWall())
                        {
                            complete = QuestWall::GetActiveWall()->get_complete();
                        }
                        usleep(1000);
                    }
                    WallPreviewViewController* previewController = Object::FindObjectOfType<WallPreviewViewController*>();//
                    if (previewController) previewController->UpdatePreview();
                    else INFO("Couldn't find preview controller");
                });
                waitForLoadedPrefab.detach();
                return;
            }

            if (!prefab) return;
            WallUtils::SetObstacleColors(selected);
            title->set_text(il2cpp_utils::createcsstr(wallDescriptor.get_name()));

            previewprefab = Object::Instantiate(prefab);
            previewprefab->SetActive(true);
            previewprefab->get_transform()->set_localPosition(UnityEngine::Vector3(2.1f, 1.2f, 1.1f));
            previewprefab->get_transform()->set_localEulerAngles(UnityEngine::Vector3(0.0f, 60.0f, 0.0f));
            previewprefab->get_transform()->set_localScale(UnityEngine::Vector3(1.5f, 1.0f, 0.5f));

            Array<MeshRenderer*>* meshrenderers = previewprefab->GetComponentsInChildren<MeshRenderer*>(true);

            typedef function_ptr_t<Array<UnityEngine::Material*>*, UnityEngine::Renderer*> GetMaterialArrayFunctionType;
            auto GetMaterialArray = *reinterpret_cast<GetMaterialArrayFunctionType>(il2cpp_functions::resolve_icall("UnityEngine.Renderer::GetMaterialArray"));

            UnityEngine::Vector4 sizeParams = UnityEngine::Vector4(previewprefab->get_transform()->get_localScale().x * 0.5f, previewprefab->get_transform()->get_localScale().y * 0.5f, previewprefab->get_transform()->get_localScale().z * 0.5f, 0.05f);
            
            int paramsID = Shader::PropertyToID(il2cpp_utils::createcsstr("_SizeParams"));
            
            for (int i = 0; i < meshrenderers->Length(); i++)
            {
                Array<Material*>* materials = GetMaterialArray(meshrenderers->values[i]);
                for (int j = 0; j < materials->Length(); j++)
                {
                    if (materials->values[j]->HasProperty(paramsID)) materials->values[j]->SetVector(paramsID, sizeParams);
                }
            }
        }
        else 
        {
            INFO("there is no active wall configured");
            if (previewprefab) 
            {
                Object::Destroy(previewprefab);
                previewprefab = nullptr;
                title->set_text(il2cpp_utils::createcsstr("Default walls (no preview)"));
            }
        }
    }
}