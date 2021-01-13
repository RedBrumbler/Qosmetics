#include "UI/Saber/SaberSwitcherViewController.hpp"
#include "config.hpp"
#include "Config/SaberConfig.hpp"
#include "Data/Descriptor.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
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
#include "Qosmetic/QuestSaber.hpp"
#include "Utils/FileUtils.hpp"
#include "Data/QosmeticsDescriptorCache.hpp"

#include "UI/Saber/SaberPreviewViewController.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

DEFINE_CLASS(Qosmetics::SaberSwitcherViewController);

#define INFO(value...) Qosmetics::UILogger::GetLogger().info(value)


void OnSelectButtonClick(Il2CppString* fileName, Button* button)
{
    if (!fileName) return;
    if (Qosmetics::QuestSaber::GetActiveSaber() && Qosmetics::QuestSaber::GetActiveSaber()->get_isLoading()) return; // if the active saber is still loading, return
    std::string name = to_utf8(csstrtostr(fileName));
    Qosmetics::Descriptor* descriptor = Qosmetics::DescriptorCache::GetDescriptor(name, saber);
    Qosmetics::QuestSaber::SetActiveSaber(descriptor, true); // set new active saber

    // update preview
    Qosmetics::SaberPreviewViewController* previewController = Object::FindObjectOfType<Qosmetics::SaberPreviewViewController*>();
    if (previewController) previewController->UpdatePreview();
    else INFO("Couldn't find preview controller");

    Qosmetics::DescriptorCache::Write();
    INFO("Selected saber %s", descriptor->get_name().c_str());
}

namespace Qosmetics
{
    void SaberSwitcherViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        DescriptorCache::Write();
        SaveConfig();
        QuestSaber::SelectionDefinitive();

    }

    void SaberSwitcherViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<Touchable*>();
            GameObject* settingsLayout = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());

            ExternalComponents* externalComponents = settingsLayout->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

            Button* defaultButton = QuestUI::BeatSaberUI::CreateUIButton(settingsLayout->get_transform(), "default saber", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), il2cpp_utils::createcsstr("", il2cpp_utils::StringType::Manual), +[](Il2CppString* fileName, Button* button){
                INFO("Default saber selected!");
                if (QuestSaber::GetActiveSaber() && QuestSaber::GetActiveSaber()->get_isLoading()) return;
                QuestSaber::SetActiveSaber((SaberData*)nullptr);
                SaberPreviewViewController* previewController = Object::FindObjectOfType<SaberPreviewViewController*>();//
                if (previewController) previewController->UpdatePreview();
                else INFO("Couldn't find preview controller");
            }));

            HorizontalLayoutGroup* selectionLayout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(settingsLayout->get_transform());
            VerticalLayoutGroup* infoLayout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(selectionLayout->get_transform());
            VerticalLayoutGroup* buttonList = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(selectionLayout->get_transform());

            std::vector<Descriptor*>& descriptors = DescriptorCache::GetSaberDescriptors();
            for (int i = 0; i < descriptors.size(); i++)
            {
                HorizontalLayoutGroup* buttonLayout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(buttonList->get_transform());

                AddButtonsForDescriptor(buttonLayout->get_transform(), descriptors[i]);
                AddTextForDescriptor(infoLayout->get_transform(), descriptors[i]);
            }
        }
    }

    void SaberSwitcherViewController::AddButtonsForDescriptor(Transform* layout, Descriptor* descriptor)
    {
        if (!layout || !descriptor) return;

        std::string stringName = descriptor->get_fileName();

        Button* selectButton = QuestUI::BeatSaberUI::CreateUIButton(layout, "select", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), il2cpp_utils::createcsstr(stringName, il2cpp_utils::StringType::Manual), OnSelectButtonClick));

        selectButton->get_gameObject()->set_name(il2cpp_utils::createcsstr(stringName));
        Button* eraseButton = QuestUI::BeatSaberUI::CreateUIButton(layout, "delete", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), il2cpp_utils::createcsstr(stringName, il2cpp_utils::StringType::Manual), +[](Il2CppString* fileName, Button* button){
            if (!fileName) return;
            std::string name = to_utf8(csstrtostr(fileName));
            Descriptor* descriptor = DescriptorCache::GetDescriptor(name, saber);
            if (fileexists(descriptor->get_filePath())) 
            {
                INFO("Deleting %s", descriptor->get_filePath().c_str());
                deletefile(descriptor->get_filePath());
                INFO("Deleted %s", descriptor->get_filePath().c_str());
            }
        }));
    }

    void SaberSwitcherViewController::AddTextForDescriptor(Transform* layout, Descriptor* descriptor)
    {
        if (!layout || !descriptor) return; // if either is nullptr, early return
        
        std::string buttonName = descriptor->get_name();
        
        if (buttonName == "") // if the name is empty, use the filename instead
        {
            buttonName = descriptor->get_fileName();
            if (buttonName != "" && buttonName.find(".") != std::string::npos) buttonName.erase(buttonName.find_last_of("."));
        }

        TMPro::TextMeshProUGUI* name = QuestUI::BeatSaberUI::CreateText(layout, buttonName + " ");
        TMPro::TextMeshProUGUI* authorText = QuestUI::BeatSaberUI::CreateText(layout, descriptor->get_author() + " ");

        QuestUI::BeatSaberUI::AddHoverHint(name->get_gameObject(), descriptor->get_description());
        authorText->set_color(UnityEngine::Color(0.8f, 0.8f, 0.8f, 0.8f));
        authorText->set_fontSize(authorText->get_fontSize() * 0.5f);
    }
}