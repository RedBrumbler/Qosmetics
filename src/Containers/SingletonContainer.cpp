#include "Containers/SingletonContainer.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

DEFINE_TYPE(Qosmetics::SingletonContainer);

using namespace Qosmetics::UI;

namespace Qosmetics
{
    SafePtr<SingletonContainer> SingletonContainer::instance = SafePtr<SingletonContainer>();
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

    void SingletonContainer::Init()
    {
        instance = *il2cpp_utils::New<SingletonContainer*>();
    }

    SingletonContainer* SingletonContainer::get_instance()
    {
        if (!instance)
        {
            instance = *il2cpp_utils::New<SingletonContainer*>();
        }

        return (SingletonContainer*)instance;
    }
    
    Qosmetics::UI::QosmeticsFlowCoordinator* SingletonContainer::get_qosmeticsFlowCoordinator()
    {
        SingletonContainer* instance = get_instance();
        if (!instance->qosmeticsFlowCoordinator) 
        {
            instance->qosmeticsFlowCoordinator = QuestUI::BeatSaberUI::CreateFlowCoordinator<QosmeticsFlowCoordinator*>();
            instance->qosmeticsFlowCoordinator->Init(nullptr,
                                            get_saberSwitcherViewController(), 
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
            instance->userProfileViewController->Init(get_saberManager(), get_noteManager(), get_wallManager());
        }
        
        return instance->userProfileViewController;
    }
}