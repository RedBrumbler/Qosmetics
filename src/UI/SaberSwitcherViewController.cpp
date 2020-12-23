#include "UI/SaberSwitcherViewController.hpp"

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
namespace Qosmetics
{
    //void OnButtonClick(QuestUI::ModSettings)
    void SaberSwitcherViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling)
    {
        DescriptorCache::Write();
    }

    void SaberSwitcherViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<Touchable*>();

            GameObject* layout = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
		    layout->AddComponent<QuestUI::Backgroundable*>()->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));

            std::vector<Descriptor*>& descriptors = DescriptorCache::GetSaberDescriptors();
            for (int i = 0; i < descriptors.size(); i++)
            {
                DescriptorWrapper* descriptorWrapper = CRASH_UNLESS(il2cpp_utils::New<DescriptorWrapper*>());//CRASH_UNLESS(il2cpp_utils::New<DescriptorWrapper*>(((void*)&descriptors[i])));
                descriptorWrapper->descriptor = (void*)descriptors[i];
                QuestUI::BeatSaberUI::CreateUIButton(layout->get_transform(), descriptors[i]->get_fileName(), il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), descriptorWrapper, +[](DescriptorWrapper* wrapper, Button* button){
                    INFO("a saber toggle was clicked!");
                    if (!wrapper) return;
                    if (!wrapper->descriptor) return;
                    QuestSaber::SetActiveSaber((Descriptor*)wrapper->descriptor);
                    Descriptor& descriptor = *(Descriptor*)wrapper->descriptor;
                    INFO("saber name was: %s", descriptor.get_name().c_str());
                    INFO("saber author was: %s", descriptor.get_author().c_str());
                    INFO("saber description was: %s", descriptor.get_description().c_str());
                    INFO("saber file path was: %s", descriptor.get_filePath().c_str());
                    INFO("saber type was: %d", descriptor.get_type());
                    std::string saberDataPath = "";
                    saberDataPath = QuestSaber::get_saberMap()[(Descriptor*)wrapper->descriptor]->get_filePath();
                    INFO("Saber file path from saber data was %s", saberDataPath.c_str());

                    Descriptor* descriptorFromCache = DescriptorCache::GetDescriptor(FileUtils::GetFileName(saberDataPath), saber);
                    if (descriptorFromCache->valid) INFO("Saber Name from DescriptorCache was %s", descriptorFromCache->get_name().c_str());
                }));
            }
        }
    }
}