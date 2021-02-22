#include "UI/Saber/SaberSwitcherViewController.hpp"

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
#include "UI/Saber/SaberSelectionElement.hpp"

DEFINE_CLASS(Qosmetics::UI::SaberSwitcherViewController);

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

struct switcherInfo {
    Cache& cache;
    Cache::iterator it;
    Transform* layout;
    SaberSwitcherViewController* self;

    switcherInfo(Cache& cache, Transform* layout, SaberSwitcherViewController* self) : cache(cache)
    {
        it = this->cache.begin();
        this->layout = layout;
        this->self = self;
    }
};

namespace Qosmetics::UI
{
    void SaberSwitcherViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            UIUtils::SetupViewController(this);
            GameObject* container = CreateScrollableSettingsContainer(get_transform());

            ExternalComponents* externalComponents = container->GetComponent<ExternalComponents*>();
            RectTransform* scrollTransform = externalComponents->Get<RectTransform*>();
            scrollTransform->set_sizeDelta(UnityEngine::Vector2(0.0f, 0.0f));

            VerticalLayoutGroup* layout = container->GetComponent<VerticalLayoutGroup*>();
            //layout->set_childForceExpandWidth(true);
            //layout->set_childControlWidth(false);

            CreateUIButton(container->get_transform(), "Default Saber", "PlayButton", il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(classof(UnityEngine::Events::UnityAction*), this, +[](SaberSwitcherViewController* self){
                self->modelManager->SetDefault();
            }));
            
            Cache& cache = DescriptorCache::GetCache(ItemType::saber);
            CustomDataType* wrapper = CRASH_UNLESS(il2cpp_utils::New<CustomDataType*, il2cpp_utils::CreationType::Manual>());
            wrapper->data = new switcherInfo(cache, container->get_transform(), this);
            auto coroutine = WaitUntil::New_ctor(il2cpp_utils::MakeDelegate<System::Func_1<bool>*>(classof(System::Func_1<bool>*), wrapper, 
                +[](CustomDataType* data){
                    switcherInfo* info = (switcherInfo*)data->data;
                    if (info->it == info->cache.end())
                    {
                        free (info);
                        free (data);
                        return true;
                    }
                    HorizontalLayoutGroup* layout = CreateHorizontalLayoutGroup(info->layout);
                    SaberSelectionElement* element = info->self->container->InstantiateComponent<SaberSelectionElement*>(layout->get_gameObject());
                    element->SetDescriptor(&info->it->second);
                    info->it++;
                    return false;
                }));
            StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(coroutine));
        }
    }

    void SaberSwitcherViewController::Init(SaberManager* saberManager, SaberPreviewViewController* previewViewController, Zenject::DiContainer* container)
    {
        this->modelManager = saberManager;
        this->previewViewController = previewViewController;
        this->container = container;
    }
}