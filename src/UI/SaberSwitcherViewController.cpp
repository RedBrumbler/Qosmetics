#include "UI/SaberSwitcherViewController.hpp"
#include "config.hpp"
#include "Config/SaberConfig.hpp"
#include "Data/Descriptor.hpp"

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
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

#include "Logging/SaberLogger.hpp"
#include "Qosmetic/QuestSaber.hpp"
#include "Utils/FileUtils.hpp"
#include "Data/QosmeticsDescriptorCache.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

DEFINE_CLASS(Qosmetics::SaberSwitcherViewController);

#define INFO(value...) SaberLogger::GetLogger().info(value)
#define toString = 
namespace Qosmetics
{
    //void OnButtonClick(QuestUI::ModSettings)
    void SaberSwitcherViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        DescriptorCache::Write();
        SaveConfig();
        /*
        Transform* transform = get_transform();
        for (int i = 0; i < transform->get_childCount(); i++)
        {
            Transform* child = transform->GetChild(i);
            std::string childName = to_utf8(csstrtostr(child->get_name()));
            INFO("Found %s", childName.c_str());
            for (int j = 0; j < child->get_childCount(); j++)
            {
                Transform* child2 = child->GetChild(j);
                std::string child2Name = to_utf8(csstrtostr(child2->get_name()));

                INFO("\t%s", child2Name.c_str());
                for (int k = 0; k < child2->get_childCount(); k++)
                {
                    Transform* child3 = child2->GetChild(k);
                    std::string child3Name = to_utf8(csstrtostr(child3->get_name()));

                    INFO("\t\t%s", child3Name.c_str());
                    for (int l = 0; l < child3->get_childCount(); l++)
                    {
                        Transform* child4 = child3->GetChild(l);

                        std::string child4Name = to_utf8(csstrtostr(child4->get_name()));
                        INFO("\t\t\t%s", child4Name.c_str());
                        for (int m = 0; m < child4->get_childCount(); m++)
                        {
                            Transform* child5 = child4->GetChild(m);

                            std::string child5Name = to_utf8(csstrtostr(child5->get_name()));
                            INFO("\t\t\t\t%s", child5Name.c_str());
                            //if (child5Name.find(".qsaber") != std::string::npos) Object::Destroy(child5);
                        }
                    }
                }
            }
        }
        */
    }

    void SaberSwitcherViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<Touchable*>();
            GameObject* layout = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
		    //layout->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));

            std::vector<Descriptor*>& descriptors = DescriptorCache::GetSaberDescriptors();
            for (int i = 0; i < descriptors.size(); i++)
            {
                std::string stringName = descriptors[i]->get_fileName();

                std::string buttonName = descriptors[i]->get_fileName();
                buttonName.erase(buttonName.find_last_of("."));
                Button* descriptorButton = QuestUI::BeatSaberUI::CreateUIButton(layout->get_transform(), buttonName, il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), il2cpp_utils::createcsstr(stringName, il2cpp_utils::Manual), +[](Il2CppString* fileName, Button* button){
                    INFO("a saber toggle was clicked!");
                    if (!fileName) return;
                    std::string name = to_utf8(csstrtostr(fileName));
                    Descriptor* descriptor = DescriptorCache::GetDescriptor(name, saber);
                    QuestSaber::SetActiveSaber(descriptor, true);
                    INFO("Saber %s was selected", descriptor->get_name().c_str());
                }));
                descriptorButton->get_gameObject()->set_name(il2cpp_utils::createcsstr(descriptors[i]->get_fileName()));
            }
        }
    }
}