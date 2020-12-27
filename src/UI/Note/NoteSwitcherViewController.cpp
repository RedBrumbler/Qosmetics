#include "UI/Note/NoteSwitcherViewController.hpp"
#include "config.hpp"
#include "Config/NoteConfig.hpp"
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
#include "Qosmetic/QuestNote.hpp"
#include "Utils/FileUtils.hpp"
#include "Data/QosmeticsDescriptorCache.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

DEFINE_CLASS(Qosmetics::NoteSwitcherViewController);

#define INFO(value...) UILogger::GetLogger().info(value)
namespace Qosmetics
{
    void NoteSwitcherViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        DescriptorCache::Write();
        SaveConfig();
    }

    void NoteSwitcherViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<Touchable*>();
            GameObject* settingsLayout = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());

            ExternalComponents* externalComponents = settingsLayout->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));
            
            Button* defaultButton = QuestUI::BeatSaberUI::CreateUIButton(settingsLayout->get_transform(), "default bloqs", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), il2cpp_utils::createcsstr("", il2cpp_utils::Manual), +[](Il2CppString* fileName, Button* button){
                INFO("Default note selected!");
                QuestNote::SetActiveNote((NoteData*)nullptr);
            }));

            HorizontalLayoutGroup* selectionLayout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(settingsLayout->get_transform());
            VerticalLayoutGroup* infoLayout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(selectionLayout->get_transform());
            VerticalLayoutGroup* buttonList = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(selectionLayout->get_transform());

            std::vector<Descriptor*>& descriptors = DescriptorCache::GetNoteDescriptors();
            for (int i = 0; i < descriptors.size(); i++)
            {
                HorizontalLayoutGroup* buttonLayout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(buttonList->get_transform());

                AddButtonsForDescriptor(buttonLayout->get_transform(), descriptors[i]);
                AddTextForDescriptor(infoLayout->get_transform(), descriptors[i]);
            }
        }
    }

    void NoteSwitcherViewController::AddButtonsForDescriptor(Transform* layout, Descriptor* descriptor)
    {
        if (!layout || !descriptor) return;

        std::string stringName = descriptor->get_fileName();

        Button* selectButton = QuestUI::BeatSaberUI::CreateUIButton(layout, "select", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), il2cpp_utils::createcsstr(stringName, il2cpp_utils::Manual), +[](Il2CppString* fileName, Button* button){
            if (!fileName) return;
            std::string name = to_utf8(csstrtostr(fileName));
            Descriptor* descriptor = DescriptorCache::GetDescriptor(name, note);
            QuestNote::SetActiveNote(descriptor, true);
            INFO("Selected note %s", descriptor->get_name().c_str());
            INFO("filePath %s", descriptor->get_filePath().c_str());
        }));

        selectButton->get_gameObject()->set_name(il2cpp_utils::createcsstr(stringName));
        Button* eraseButton = QuestUI::BeatSaberUI::CreateUIButton(layout, "erase", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), il2cpp_utils::createcsstr(stringName, il2cpp_utils::Manual), +[](Il2CppString* fileName, Button* button){
            if (!fileName) return;
            std::string name = to_utf8(csstrtostr(fileName));
            Descriptor* descriptor = DescriptorCache::GetDescriptor(name, note);
            if (fileexists(descriptor->get_filePath())) 
            {
                INFO("Deleting %s", descriptor->get_filePath().c_str());
                deletefile(descriptor->get_filePath());
                INFO("Deleted %s", descriptor->get_filePath().c_str());
            }
        }));
    }

    void NoteSwitcherViewController::AddTextForDescriptor(Transform* layout, Descriptor* descriptor)
    {
        if (!layout || !descriptor) return; // if either is nullptr, early return
        
        std::string buttonName = descriptor->get_name();
        
        if (buttonName == "") // if the name is empty, use the filename instead
        {
            descriptor->get_fileName();
            buttonName.erase(buttonName.find_last_of("."));
        }

        TMPro::TextMeshProUGUI* name = QuestUI::BeatSaberUI::CreateText(layout, buttonName + " ");
        TMPro::TextMeshProUGUI* authorText = QuestUI::BeatSaberUI::CreateText(layout, descriptor->get_author() + " ");

        QuestUI::BeatSaberUI::AddHoverHint(name->get_gameObject(), descriptor->get_description());
        authorText->set_color(UnityEngine::Color(0.8f, 0.8f, 0.8f, 0.8f));
        authorText->set_fontSize(authorText->get_fontSize() * 0.5f);
    }

}