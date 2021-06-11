#include "Config.hpp"
#include "UI/Note/NoteSwitcherViewController.hpp"
#include "UI/General/ContainerFindType.hpp"

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
#include "UI/Note/NoteSelectionElement.hpp"
#include "UI/datastructs.hpp"
DEFINE_TYPE(Qosmetics::UI::NoteSwitcherViewController);

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
    void NoteSwitcherViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            GameObject* container = CreateScrollableSettingsContainer(get_transform());
            container->AddComponent<ContainerFindType*>();

            ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

            VerticalLayoutGroup* layout = container->GetComponent<VerticalLayoutGroup*>();
            HorizontalLayoutGroup* topButtonGroup = CreateHorizontalLayoutGroup(layout->get_transform());

            CreateUIButton(topButtonGroup->get_transform(), "Default Notes", "PlayButton", 
                [&](){
                    config.lastActiveNote = "";
                    this->modelManager->SetDefault();
                    this->previewViewController->UpdatePreview();
                    SaveConfig();
                });
                
            CreateUIButton(topButtonGroup->get_transform(), "Reload", "QosmeticsTemplateButton", 
                [&](){
                    // save the cache and load it, which makes it check for new files
                    DescriptorCache::Save();
                    DescriptorCache::Load();

                    // updating the selection is as simple as running the coro again, since it has all the checks built in to properly add new stuff and ignore old stuff
                    Cache& cache = DescriptorCache::GetCache(ItemType::note);
                    Transform* containerT = GetComponentInChildren<ContainerFindType*>()->get_transform();
                    switcherInfo* info = new switcherInfo(cache, containerT);
                    StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(SetupSelectionsRoutine(info))));
                });
        }

        Cache& cache = DescriptorCache::GetCache(ItemType::note);
        switcherInfo* info = new switcherInfo(cache, GetComponentInChildren<ContainerFindType*>()->get_transform());
        StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(SetupSelectionsRoutine(info))));
    }

    custom_types::Helpers::Coroutine NoteSwitcherViewController::SetupSelectionsRoutine(switcherInfo* info)
    {
        while (info->it != info->cache.end())
        {
            // get a possibly already existing transform for the selector for current descriptor
            Transform* existingSelection = info->layout->Find(il2cpp_utils::newcsstr(info->it->second.GetFileName()));
            // if the file doesnt exist, OR this descriptor already had a selector, dont add a new one
            bool fileExists = fileexists(info->it->second.get_filePath());
            if (!fileExists || existingSelection)
            {
                // if the file doesnt exist AND there is an existing selection, remove it
                if (!fileExists && existingSelection)
                {
                    NoteSelectionElement* element = existingSelection->get_gameObject()->GetComponent<NoteSelectionElement*>();
                    element->Delete();
                }

                info->it++;
                continue;
            }

            HorizontalLayoutGroup* layout = CreateHorizontalLayoutGroup(info->layout);
            NoteSelectionElement* element = layout->get_gameObject()->AddComponent<NoteSelectionElement*>();
            element->Init(this->modelManager, this->previewViewController);
            element->SetDescriptor(&info->it->second);
            layout->get_gameObject()->set_name(il2cpp_utils::newcsstr(info->it->second.GetFileName()));

            info->it++;
            co_yield nullptr;
        }
        
        free (info);
        co_return;
    }

    void NoteSwitcherViewController::Init(NoteManager* noteManager, NotePreviewViewController* previewViewController)
    {
        this->modelManager = noteManager;
        this->previewViewController = previewViewController;
    }
}