#include "Config.hpp"
#include "UI/Wall/WallSwitcherViewController.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"

#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"

#include "Data/DescriptorCache.hpp"

#include "Utils/UIUtils.hpp" 
#include "UI/Wall/WallSelectionElement.hpp"
#include "UI/datastructs.hpp"
DEFINE_TYPE(Qosmetics::UI::WallSwitcherViewController);

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

namespace Qosmetics::UI
{
    void WallSwitcherViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            //UIUtils::SetupViewController(this);
            GameObject* container = CreateScrollableSettingsContainer(get_transform());

            ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

            VerticalLayoutGroup* layout = container->GetComponent<VerticalLayoutGroup*>();
            //layout->set_childForceExpandWidth(true);
            //layout->set_childControlWidth(false);

            CreateUIButton(container->get_transform(), "Default Wall", "PlayButton", 
                [&](){
                    config.lastActiveWall = "";
                    this->modelManager->SetDefault();
                    this->previewViewController->UpdatePreview();
                    SaveConfig();
                });
            
            Cache& cache = DescriptorCache::GetCache(ItemType::wall);
            switcherInfo* info = new switcherInfo(cache, container->get_transform());

            StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(SetupSelectionsRoutine(info))));
        }
    }

    custom_types::Helpers::Coroutine WallSwitcherViewController::SetupSelectionsRoutine(switcherInfo* info)
    {
        while (info->it != info->cache.end())
        {
            if (!fileexists(info->it->second.get_filePath()))
            {
                info->it++;
                continue;
            }
            HorizontalLayoutGroup* layout = CreateHorizontalLayoutGroup(info->layout);
            WallSelectionElement* element = layout->get_gameObject()->AddComponent<WallSelectionElement*>();
            element->Init(this->modelManager, this->previewViewController);
            element->SetDescriptor(&info->it->second);
            info->it++;
            co_yield nullptr;
        }

        free (info);
        co_return;
    }

    void WallSwitcherViewController::Init(WallManager* wallManager, WallPreviewViewController* previewViewController)
    {
        this->modelManager = wallManager;
        this->previewViewController = previewViewController;
    }
}