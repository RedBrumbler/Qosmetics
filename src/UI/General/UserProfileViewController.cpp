#include "UI/General/UserProfileViewController.hpp"
#include "Config.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "QosmeticsLogger.hpp"
#include "static-defines.hpp"

#include "HMUI/ButtonSpriteSwap.hpp"
#include "HMUI/Touchable.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Texture.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/TextureWrapMode.hpp"
#include "Utils/UIUtils.hpp"

#include "Types/Pointer/Pointer.hpp"
#include "Utils/UnityUtils.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"

#include "Containers/SingletonContainer.hpp"

#include <sstream>

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::Events;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

DEFINE_TYPE(Qosmetics::UI, UserProfileViewController);

#define INFO(value...) QosmeticsLogger::GetContextLogger("User Profile View Controller").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("User Profile View Controller").error(value);

namespace Qosmetics::UI
{
    void UserProfileViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<Touchable*>();
            Color titleColor = Color(0.3f, 0.15f, 0.6f, 1.0f);
            UIUtils::AddHeader(get_transform(), "User Profiles (Experimental)", titleColor);
            GameObject* container = CreateScrollableSettingsContainer(get_transform());
            ExternalComponents* components = container->GetComponent<ExternalComponents*>();
            RectTransform* rect = components->Get<RectTransform*>();
            rect->set_sizeDelta({0.0f, 0.0f});

            HorizontalLayoutGroup* horizontal = CreateHorizontalLayoutGroup(container->get_transform());
            VerticalLayoutGroup* layout = CreateVerticalLayoutGroup(horizontal->get_transform());

            std::stringstream stream;

            float height = 5.0f;

            TMPro::TextMeshProUGUI* text = BeatSaberUI::CreateText(layout->get_transform(), "This menu lets users create and choose different profiles.");
            text->set_alignment(TMPro::TextAlignmentOptions::Center);
            LayoutElement* layoutelem = UnityUtils::GetAddComponent<LayoutElement*>(text->get_gameObject());
            layoutelem->set_preferredHeight(height);
            layoutelem->set_preferredWidth(70.0f);
            text = BeatSaberUI::CreateText(layout->get_transform(), "While it works, it's very experimental, so it might not");
            text->set_alignment(TMPro::TextAlignmentOptions::Center);
            layoutelem = UnityUtils::GetAddComponent<LayoutElement*>(text->get_gameObject());
            layoutelem->set_preferredHeight(height);
            layoutelem->set_preferredWidth(70.0f);
            text = BeatSaberUI::CreateText(layout->get_transform(), "always work correctly");
            text->set_alignment(TMPro::TextAlignmentOptions::Center);
            layoutelem = UnityUtils::GetAddComponent<LayoutElement*>(text->get_gameObject());
            layoutelem->set_preferredHeight(height);
            layoutelem->set_preferredWidth(70.0f);

            HMUI::SimpleTextDropdown* trailDropdown = BeatSaberUI::CreateDropdown(container->get_transform(), "User", masterConfig.lastUsedConfig, Config::get_configNames(), [this](auto value)
                                                                                  {
                                                                                      if (value == masterConfig.lastUsedConfig)
                                                                                          return;
                                                                                      Config::LoadConfig(value);

                                                                                      SingletonContainer::ResetSelectionUI();

                                                                                      // update all the models too
                                                                                      if (config.lastActiveSaber != "")
                                                                                          saberManager->internalSetActiveModel(config.lastActiveSaber, true);
                                                                                      else
                                                                                          saberManager->SetDefault();
                                                                                      if (config.lastActiveNote != "")
                                                                                          noteManager->internalSetActiveModel(config.lastActiveNote, true);
                                                                                      else
                                                                                          noteManager->SetDefault();
                                                                                      if (config.lastActiveWall != "")
                                                                                          wallManager->internalSetActiveModel(config.lastActiveWall, true);
                                                                                      else
                                                                                          wallManager->SetDefault();

                                                                                      Pointer::UpdateAll(true);
                                                                                  });

            layoutelem = UnityUtils::GetAddComponent<LayoutElement*>(trailDropdown->get_gameObject());
            layoutelem->set_preferredHeight(8.0f);
            layoutelem->set_preferredWidth(50.0f);

            HorizontalLayoutGroup* hor = CreateHorizontalLayoutGroup(container->get_transform());

            HMUI::InputFieldView* textField = BeatSaberUI::CreateStringSetting(hor->get_transform(), "name", "");
            layoutelem = UnityUtils::GetAddComponent<LayoutElement*>(textField->get_gameObject());
            layoutelem->set_preferredHeight(8.0f);
            layoutelem->set_preferredWidth(50.0f);

            Button* confirmAdd = BeatSaberUI::CreateUIButton(hor->get_transform(), "Add User", "QosmeticsTemplateButton", [textField, trailDropdown]
                                                             {
                                                                 std::vector<std::string>& configs = Config::get_configNames();
                                                                 Il2CppString* newUserCS = textField->get_text();
                                                                 std::string newUser = to_utf8(csstrtostr(newUserCS));
                                                                 // min user name length of 3 letters
                                                                 if (newUser.size() < 3)
                                                                     return;
                                                                 textField->ClearInput();
                                                                 std::vector<std::string>::iterator it = std::find(configs.begin(), configs.end(), newUser);
                                                                 if (it == configs.end())
                                                                 {
                                                                     configs.push_back(newUser);

                                                                     List<Il2CppString*>* users = List<Il2CppString*>::New_ctor();
                                                                     for (auto u : configs)
                                                                     {
                                                                         users->Add(il2cpp_utils::newcsstr(u));
                                                                     }

                                                                     trailDropdown->SetTexts(reinterpret_cast<System::Collections::Generic::IReadOnlyList_1<Il2CppString*>*>(users));
                                                                     trailDropdown->SelectCellWithIdx(configs.size());

                                                                     masterConfig.lastUsedConfig = newUser;
                                                                     SaveConfig();
                                                                 }
                                                             });

            TMPro::TextMeshProUGUI* createNew = BeatSaberUI::CreateText(container->get_transform(), "\n Create a New profile from the current profile");
            createNew->set_alignment(TMPro::TextAlignmentOptions::Center);
        }
    }

    void UserProfileViewController::Init(Qosmetics::SaberManager* saberManager, Qosmetics::NoteManager* noteManager, Qosmetics::WallManager* wallManager)
    {
        this->saberManager = saberManager;
        this->noteManager = noteManager;
        this->wallManager = wallManager;
    }
}