#include "Installers/UIInstaller.hpp"

#include "QosmeticsLogger.hpp"

#include "UI/Saber/SaberPreviewElement.hpp"
#include "UI/Saber/SaberPreviewViewController.hpp"
#include "UI/Saber/SaberSelectionElement.hpp"
#include "UI/Saber/SaberSettingsViewController.hpp"
#include "UI/Saber/SaberSwitcherViewController.hpp"

#include "UI/Note/NotePreviewElement.hpp"
#include "UI/Note/NotePreviewViewController.hpp"
#include "UI/Note/NoteSelectionElement.hpp"
#include "UI/Note/NoteSettingsViewController.hpp"
#include "UI/Note/NoteSwitcherViewController.hpp"

#include "UI/Wall/WallPreviewElement.hpp"
#include "UI/Wall/WallPreviewViewController.hpp"
#include "UI/Wall/WallSelectionElement.hpp"
#include "UI/Wall/WallSettingsViewController.hpp"
#include "UI/Wall/WallSwitcherViewController.hpp"

#include "UI/General/PatronViewController.hpp"
#include "UI/General/FloorLogoViewController.hpp"
#include "UI/General/QosmeticsFlowCoordinator.hpp"
#include "UI/General/QosmeticsViewController.hpp"

#include "Containers/FlowCoordinatorContainer.hpp"
#include "Utils/UIUtils.hpp"
#include "UnityEngine/Object.hpp"

DEFINE_CLASS(Qosmetics::UIInstaller);

#define INFO(value...) QosmeticsLogger::GetContextLogger("UIInstaller").info(value);
#define ERROR(value...) QosmeticsLogger::GetContextLogger("UIInstaller").error(value);

using namespace UnityEngine;
template<class T>
T instantiateAndName(Zenject::DiContainer* container, GameObject* orig, Il2CppString* name)
{
    GameObject* result = container->InstantiatePrefab(orig);
    result->set_name(name);
    T type = result->AddComponent<T>();
    container->Inject(type);
    return type;
}

#define BindView(typename, name) \
{ \
    typename instance = instantiateAndName<typename>(container, viewControllerBase, il2cpp_utils::createcsstr(name)); \
    container->Bind<typename>() \
        ->FromInstance(instance) \
        ->AsSingle(); \
}

namespace Qosmetics
{
    void UIInstaller::ctor() {};

    void UIInstaller::InstallBindings()
    {
        INFO("Installing UI Bindings");
        viewControllerBase = GameObject::New_ctor(il2cpp_utils::createcsstr("viewControllerBase"));
        UIUtils::AddViewComponents(viewControllerBase, container);

        container->InstantiatePrefab(viewControllerBase);
        // Saber stuff
        INFO("Installing Saber Bindings");
        BindView(UI::SaberPreviewViewController*, "SaberPreviewViewController");        
        BindView(UI::SaberSettingsViewController*, "SaberSettingsViewController");
        BindView(UI::SaberSwitcherViewController*, "SaberSwitcherViewController");

        BindView(UI::NotePreviewViewController*, "NotePreviewViewController");        
        BindView(UI::NoteSettingsViewController*, "NoteSettingsViewController");
        BindView(UI::NoteSwitcherViewController*, "NoteSwitcherViewController");

        BindView(UI::WallPreviewViewController*, "WallPreviewViewController");        
        BindView(UI::WallSettingsViewController*, "WallSettingsViewController");
        BindView(UI::WallSwitcherViewController*, "WallSwitcherViewController");

        BindView(UI::PatronViewController*, "PatronViewController");
        BindView(UI::FloorLogoViewController*, "FloorLogoViewController");
        BindView(UI::QosmeticsViewController*, "QosmeticsViewController");
        
        /*INFO("Preview");
        container->Bind<UI::SaberPreviewViewController*>()
            ->FromInstance(instantiateAndName<UI::SaberPreviewViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("SaberPreviewViewController")))
            ->AsSingle();
        INFO("Settings");
        container->Bind<UI::SaberSettingsViewController*>()
            ->FromInstance(instantiateAndName<UI::SaberSettingsViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("SaberSettingsViewController")))
            ->AsSingle();
        INFO("Switcher");
        container->Bind<UI::SaberSwitcherViewController*>()
            ->FromInstance(instantiateAndName<UI::SaberSwitcherViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("SaberSwitcherViewController")))
            ->AsSingle();

        // Note stuff
        INFO("Installing Note Bindings");
        container->Bind<UI::NotePreviewViewController*>()
            ->FromInstance(instantiateAndName<UI::NotePreviewViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("NotePreviewViewController")))
            ->AsSingle();
        container->Bind<UI::NoteSettingsViewController*>()
            ->FromInstance(instantiateAndName<UI::NoteSettingsViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("NoteSettingsViewController")))
            ->AsSingle();
        container->Bind<UI::NoteSwitcherViewController*>()
            ->FromInstance(instantiateAndName<UI::NoteSwitcherViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("NoteSwitcherViewController")))
            ->AsSingle();

        // Wall stuff
        INFO("Installing Wall Bindings");
        container->Bind<UI::WallPreviewViewController*>()
            ->FromInstance(instantiateAndName<UI::WallPreviewViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("WallPreviewViewController")))
            ->AsSingle();
        container->Bind<UI::WallSettingsViewController*>()
            ->FromInstance(instantiateAndName<UI::WallSettingsViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("WallSettingsViewController")))
            ->AsSingle();
        container->Bind<UI::WallSwitcherViewController*>()
            ->FromInstance(instantiateAndName<UI::WallSwitcherViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("WallSwitcherViewController")))
            ->AsSingle();
        
        /// General stuff
        INFO("Installing General Bindings");
        container->Bind<UI::PatronViewController*>()
            ->FromInstance(instantiateAndName<UI::PatronViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("PatronViewController")))
            ->AsSingle();
        container->Bind<UI::QosmeticsViewController*>()
            ->FromInstance(instantiateAndName<UI::QosmeticsViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("QosmeticsViewController")))
            ->AsSingle();
        container->Bind<UI::FloorLogoViewController*>()
            ->FromInstance(instantiateAndName<UI::FloorLogoViewController*>(container, viewControllerBase, il2cpp_utils::createcsstr("FloorLogoViewController")))
            ->AsSingle();
        */
        // Flow coordinator
        container->Bind<UI::QosmeticsFlowCoordinator*>()
            ->FromNewComponentOn(GameObject::New_ctor(il2cpp_utils::createcsstr("QosmeticsFlowCoordinator")))
            ->AsSingle()
            ->NonLazy();
    }
}