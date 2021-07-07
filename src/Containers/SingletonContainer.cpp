#include "Containers/SingletonContainer.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Object.hpp"

DEFINE_TYPE(Qosmetics, SingletonContainer);

using namespace Qosmetics::UI;
using namespace UnityEngine;

namespace Qosmetics
{
    //SafePtr<SingletonContainer>* SingletonContainer::instance = nullptr;
    SingletonContainer* SingletonContainer::instance = nullptr;
    void SingletonContainer::ctor()
    {
        qosmeticsFlowCoordinator = nullptr;
        saberManager = nullptr;
        noteManager = nullptr;
        wallManager = nullptr;
        colorManager = nullptr;

        saberSwitcherViewController = nullptr;
        saberSettingsViewController = nullptr;
        saberPreviewViewController = nullptr;

        noteSwitcherViewController = nullptr;
        noteSettingsViewController = nullptr;
        notePreviewViewController = nullptr;

        wallSwitcherViewController = nullptr;
        wallSettingsViewController = nullptr;
        wallPreviewViewController = nullptr;

        qosmeticsViewController = nullptr;
        patronViewController = nullptr;
        floorLogoViewController = nullptr;
        userProfileViewController = nullptr;
    }

    void SingletonContainer::Delete()
    {
        instance->qosmeticsFlowCoordinator = nullptr;
        instance->saberSwitcherViewController = nullptr;
        instance->saberSettingsViewController = nullptr;
        instance->saberPreviewViewController = nullptr;

        instance->noteSwitcherViewController = nullptr;
        instance->noteSettingsViewController = nullptr;
        instance->notePreviewViewController = nullptr;

        instance->wallSwitcherViewController = nullptr;
        instance->wallSettingsViewController = nullptr;
        instance->wallPreviewViewController = nullptr;

        instance->qosmeticsViewController = nullptr;
        instance->patronViewController = nullptr;
        instance->floorLogoViewController = nullptr;
        instance->userProfileViewController = nullptr;

        /*
        if (instance)
        {
            free(instance);
            instance = nullptr;
        }
        */
    }
    void SingletonContainer::Init()
    {
        //if (!instance) instance = new SafePtr<SingletonContainer>();
        if (!instance) instance = *il2cpp_utils::New<SingletonContainer*, il2cpp_utils::CreationType::Manual>();
    }

    SingletonContainer* SingletonContainer::get_instance()
    {
        if (!instance)// || !instance->operator bool())
        {
            //if (!instance) instance = new SafePtr<SingletonContainer>();
            instance = *il2cpp_utils::New<SingletonContainer*, il2cpp_utils::CreationType::Manual>();
        }

        return (SingletonContainer*)instance;
    }
    
    Qosmetics::UI::QosmeticsFlowCoordinator* SingletonContainer::get_qosmeticsFlowCoordinator()
    {
        SingletonContainer* instance = get_instance();

        if (!instance->qosmeticsFlowCoordinator) 
        {
            instance->qosmeticsFlowCoordinator = QuestUI::BeatSaberUI::CreateFlowCoordinator<QosmeticsFlowCoordinator*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->qosmeticsFlowCoordinator->get_gameObject());
            instance->qosmeticsFlowCoordinator->Init(   get_saberSwitcherViewController(), 
                                                        get_saberSettingsViewController(), 
                                                        get_saberPreviewViewController(), 
                                                        get_noteSwitcherViewController(),
                                                        get_noteSettingsViewController(),
                                                        get_notePreviewViewController(),
                                                        get_wallSwitcherViewController(),
                                                        get_wallSettingsViewController(),
                                                        get_wallPreviewViewController(),
                                                        get_patronViewController(),
                                                        get_qosmeticsViewController(),
                                                        get_floorLogoViewController(),
                                                        get_userProfileViewController());
        }

        return instance->qosmeticsFlowCoordinator;
    }

    Qosmetics::SaberManager* SingletonContainer::get_saberManager()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->saberManager)
        {
            instance->saberManager = CRASH_UNLESS(il2cpp_utils::New<SaberManager*, il2cpp_utils::CreationType::Manual>());
        }

        return instance->saberManager;
    }

    Qosmetics::NoteManager* SingletonContainer::get_noteManager()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->noteManager)
        {
            instance->noteManager = CRASH_UNLESS(il2cpp_utils::New<NoteManager*, il2cpp_utils::CreationType::Manual>());
        }
        
        return instance->noteManager;
    }

    Qosmetics::WallManager* SingletonContainer::get_wallManager()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->wallManager)
        {
            instance->wallManager = CRASH_UNLESS(il2cpp_utils::New<WallManager*, il2cpp_utils::CreationType::Manual>());
        }
        
        return instance->wallManager;
    }

    Qosmetics::ColorManager* SingletonContainer::get_colorManager()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->colorManager)
        {
            instance->colorManager = CRASH_UNLESS(il2cpp_utils::New<ColorManager*, il2cpp_utils::CreationType::Manual>());
        }
        
        return instance->colorManager;
    }

    Qosmetics::UI::SaberSwitcherViewController* SingletonContainer::get_saberSwitcherViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->saberSwitcherViewController)
        {
            instance->saberSwitcherViewController = QuestUI::BeatSaberUI::CreateViewController<SaberSwitcherViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->saberSwitcherViewController->get_gameObject());
            instance->saberSwitcherViewController->Init(get_saberManager(), get_saberPreviewViewController());
        }
        
        return instance->saberSwitcherViewController;
    }

    Qosmetics::UI::SaberSettingsViewController* SingletonContainer::get_saberSettingsViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->saberSettingsViewController)
        {
            instance->saberSettingsViewController = QuestUI::BeatSaberUI::CreateViewController<SaberSettingsViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->saberSettingsViewController->get_gameObject());
            instance->saberSettingsViewController->Init(get_saberPreviewViewController());
        }
        
        return instance->saberSettingsViewController;
    }

    Qosmetics::UI::SaberPreviewViewController* SingletonContainer::get_saberPreviewViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->saberPreviewViewController)
        {
            instance->saberPreviewViewController = QuestUI::BeatSaberUI::CreateViewController<SaberPreviewViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->saberPreviewViewController->get_gameObject());
            instance->saberPreviewViewController->Init(get_saberManager(), get_colorManager());
        }
        
        return instance->saberPreviewViewController;
    }

    
    Qosmetics::UI::NoteSwitcherViewController* SingletonContainer::get_noteSwitcherViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->noteSwitcherViewController)
        {
            instance->noteSwitcherViewController = QuestUI::BeatSaberUI::CreateViewController<NoteSwitcherViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->noteSwitcherViewController->get_gameObject());
            instance->noteSwitcherViewController->Init(get_noteManager(), get_notePreviewViewController());
        }
        
        return instance->noteSwitcherViewController;
    }

    Qosmetics::UI::NoteSettingsViewController* SingletonContainer::get_noteSettingsViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->noteSettingsViewController)
        {
            instance->noteSettingsViewController = QuestUI::BeatSaberUI::CreateViewController<NoteSettingsViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->noteSettingsViewController->get_gameObject());
            instance->noteSettingsViewController->Init(get_notePreviewViewController());
        }
        
        return instance->noteSettingsViewController;
    }

    Qosmetics::UI::NotePreviewViewController* SingletonContainer::get_notePreviewViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->notePreviewViewController)
        {
            instance->notePreviewViewController = QuestUI::BeatSaberUI::CreateViewController<NotePreviewViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->notePreviewViewController->get_gameObject());
            instance->notePreviewViewController->Init(get_noteManager(), get_colorManager());
        }
        
        return instance->notePreviewViewController;
    }
    
    Qosmetics::UI::WallSwitcherViewController* SingletonContainer::get_wallSwitcherViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->wallSwitcherViewController)
        {
            instance->wallSwitcherViewController = QuestUI::BeatSaberUI::CreateViewController<WallSwitcherViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->wallSwitcherViewController->get_gameObject());
            instance->wallSwitcherViewController->Init(get_wallManager(), get_wallPreviewViewController());
        }
        
        return instance->wallSwitcherViewController;
    }

    Qosmetics::UI::WallSettingsViewController* SingletonContainer::get_wallSettingsViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->wallSettingsViewController)
        {
            instance->wallSettingsViewController = QuestUI::BeatSaberUI::CreateViewController<WallSettingsViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->wallSettingsViewController->get_gameObject());
            instance->wallSettingsViewController->Init(get_wallPreviewViewController());
        }
        
        return instance->wallSettingsViewController;
    }

    Qosmetics::UI::WallPreviewViewController* SingletonContainer::get_wallPreviewViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->wallPreviewViewController)
        {
            instance->wallPreviewViewController = QuestUI::BeatSaberUI::CreateViewController<WallPreviewViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->wallPreviewViewController->get_gameObject());
            instance->wallPreviewViewController->Init(get_wallManager(), get_colorManager());
        }
        
        return instance->wallPreviewViewController;
    }

    Qosmetics::UI::QosmeticsViewController* SingletonContainer::get_qosmeticsViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->qosmeticsViewController)
        {
            instance->qosmeticsViewController = QuestUI::BeatSaberUI::CreateViewController<QosmeticsViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->qosmeticsViewController->get_gameObject());
            instance->qosmeticsViewController->Init();
        }
        
        return instance->qosmeticsViewController;
    }

    Qosmetics::UI::PatronViewController* SingletonContainer::get_patronViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->patronViewController)
        {
            instance->patronViewController = QuestUI::BeatSaberUI::CreateViewController<PatronViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->patronViewController->get_gameObject());
            instance->patronViewController->Init();
        }
        
        return instance->patronViewController;
    }

    Qosmetics::UI::FloorLogoViewController* SingletonContainer::get_floorLogoViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->floorLogoViewController)
        {
            instance->floorLogoViewController = QuestUI::BeatSaberUI::CreateViewController<FloorLogoViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->floorLogoViewController->get_gameObject());
            instance->floorLogoViewController->Init();
        }
        
        return instance->floorLogoViewController;
    }

    Qosmetics::UI::UserProfileViewController* SingletonContainer::get_userProfileViewController()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->userProfileViewController)
        {
            instance->userProfileViewController = QuestUI::BeatSaberUI::CreateViewController<UserProfileViewController*>();
            UnityEngine::Object::DontDestroyOnLoad(instance->userProfileViewController->get_gameObject());
            instance->userProfileViewController->Init(get_saberManager(), get_noteManager(), get_wallManager());
        }
        
        return instance->userProfileViewController;
    }
}