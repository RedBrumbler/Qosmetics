#include "UI/Saber/SaberSwitcherViewController.hpp"
#include "config.hpp"
#include "Config/SaberConfig.hpp"
#include "Data/Descriptor.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "UnityEngine/Coroutine.hpp"
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
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"

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
#include "Data/CreatorCache.hpp"

#include "UI/Saber/SaberPreviewViewController.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

DEFINE_CLASS(Qosmetics::SaberSwitcherViewController);

#define INFO(value...) Qosmetics::UILogger::GetLogger().WithContext("Saber Switching").info(value)
#define ERROR(value...) Qosmetics::UILogger::GetLogger().WithContext("Saber Switching").error(value)


static int saberCount = 0;
static int saberIndex = 0;
static int programIndex = 0;
static VerticalLayoutGroup* selectList = nullptr;
static VerticalLayoutGroup* deleteList = nullptr;
static VerticalLayoutGroup* infoLayout = nullptr;
static HorizontalLayoutGroup* buttonLayout = nullptr;
static inline std::vector<Qosmetics::Descriptor*> descriptors = {};

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
    else ERROR("Couldn't find preview controller");

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
                else ERROR("Couldn't find preview controller");
            }));
            
            HorizontalLayoutGroup* selectionLayout = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(settingsLayout->get_transform());
            selectionLayout->set_spacing(3.0f);
            infoLayout = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(selectionLayout->get_transform());
            selectList = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(selectionLayout->get_transform());
            deleteList = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(selectionLayout->get_transform());
            
            descriptors = DescriptorCache::GetSaberDescriptors();
            saberCount = descriptors.size();

            auto coroutine = WaitUntil::New_ctor(il2cpp_utils::MakeDelegate<System::Func_1<bool>*>(classof(System::Func_1<bool>*), this, 
            +[](Qosmetics::SaberSwitcherViewController* self){ 
                if (saberIndex >= saberCount) return true;
                switch (programIndex)
                {
                    case 0:
                        self->AddButtonsForDescriptor(selectList->get_transform(), descriptors[saberIndex]);
                        programIndex++;
                        return false;
                    case 1:
                        self->AddButtonsForDescriptor(deleteList->get_transform(), descriptors[saberIndex]);
                        programIndex++;
                        return false;
                    case 2:
                        buttonLayout = nullptr;
                    case 3:
                        self->AddTextForDescriptor(infoLayout->get_transform(), descriptors[saberIndex]);
                        programIndex++;
                        return false;
                    default:
                        programIndex = 0;
                        saberIndex++; 
                        if(saberIndex < saberCount) 
                            return false; 
                        return true; 
                }
                return true;
            }));
            StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(coroutine));
        }
    }

    void SaberSwitcherViewController::AddButtonsForDescriptor(Transform* layout, Descriptor* descriptor)
    {
        if (!layout || !descriptor) return;

        std::string stringName = descriptor->get_fileName();
        if (programIndex == 0)
        {
            Button* selectButton = QuestUI::BeatSaberUI::CreateUIButton(layout, "select", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), il2cpp_utils::createcsstr(stringName, il2cpp_utils::StringType::Manual), OnSelectButtonClick));
            selectButton->get_gameObject()->set_name(il2cpp_utils::createcsstr(stringName));
        }
        else if (programIndex == 1)
        {
            Button* eraseButton = QuestUI::BeatSaberUI::CreateUIButton(layout, "delete", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), il2cpp_utils::createcsstr(stringName, il2cpp_utils::StringType::Manual), +[](Il2CppString* fileName, Button* button){
                if (!fileName) return;
                std::string name = to_utf8(csstrtostr(fileName));
                Descriptor* descriptor = DescriptorCache::GetDescriptor(name, saber);
                if (!descriptor) return;
                if (fileexists(descriptor->get_filePath())) 
                {
                    INFO("Deleting %s", descriptor->get_filePath().c_str());
                    deletefile(descriptor->get_filePath());
                }
            }));
        }
    }

    bool shouldRainbow(UnityEngine::Color color)
    {
        bool should = false;
        
        if (color.r >= 0.99 && color.g >= 0.99 && color.b >= 0.99) should = true;

        return should;
    }

    void SaberSwitcherViewController::AddTextForDescriptor(Transform* layout, Descriptor* descriptor)
    {
        if (!layout || !descriptor) return; // if either is nullptr, early return
        if (programIndex == 2)
        {
            std::string buttonName = descriptor->get_name();
            INFO("%s", buttonName.c_str());
            if (buttonName == "") // if the name is empty, use the filename instead
            {
                buttonName = descriptor->get_fileName();
                if (buttonName != "" && buttonName.find(".") != std::string::npos) buttonName.erase(buttonName.find_last_of("."));
            }

            if (buttonName.find("rainbow") != std::string::npos || buttonName.find("Rainbow") != std::string::npos) buttonName = FileUtils::rainbowIfy(buttonName);

            TMPro::TextMeshProUGUI* name = QuestUI::BeatSaberUI::CreateText(layout, buttonName);
            QuestUI::BeatSaberUI::AddHoverHint(name->get_gameObject(), descriptor->get_description());
        }
        else if (programIndex == 3)
        {
            std::string authorName = descriptor->get_author();

            if (authorName == "")
            {
                authorName = "---";
            }

            UnityEngine::Color textColor = CreatorCache::GetCreatorColor(authorName);

            if (shouldRainbow(textColor))
                authorName = FileUtils::rainbowIfy(authorName);
            TMPro::TextMeshProUGUI* authorText = QuestUI::BeatSaberUI::CreateText(layout, authorName);

            authorText->set_color(textColor);
            authorText->set_fontSize(authorText->get_fontSize() * 0.5f);
        }
    }
}