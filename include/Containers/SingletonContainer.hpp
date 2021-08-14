#pragma once

#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"
#include "custom-types/shared/macros.hpp"
#include "UI/General/QosmeticsFlowCoordinator.hpp"

#include "UI/Saber/SaberPreviewViewController.hpp"
#include "UI/Saber/SaberSettingsViewController.hpp"
#include "UI/Saber/SaberSwitcherViewController.hpp"

#include "UI/Note/NotePreviewViewController.hpp"
#include "UI/Note/NoteSettingsViewController.hpp"
#include "UI/Note/NoteSwitcherViewController.hpp"

#include "UI/Wall/WallPreviewViewController.hpp"
#include "UI/Wall/WallSettingsViewController.hpp"
#include "UI/Wall/WallSwitcherViewController.hpp"

#include "UI/General/PatronViewController.hpp"
#include "UI/General/FloorLogoViewController.hpp"
#include "UI/General/QosmeticsFlowCoordinator.hpp"
#include "UI/General/QosmeticsViewController.hpp"
#include "UI/General/UserProfileViewController.hpp"

namespace Qosmetics {
    class SingletonContainer;
}

#define MY_DECLARE_STATIC_FIELD(type_, name_) \
private: \
struct ___StaticFieldRegistrator_##name_ : ::custom_types::StaticFieldRegistrator { \
    size_t oft; \
    ___StaticFieldRegistrator_##name_() { \
        oft = ___TargetType::___TypeRegistration::addStaticField(size()); \
        ___TargetType::___TypeRegistration::addStaticFieldInstance(this); \
    } \
    constexpr const char* name() const override { \
        return #name_; \
    } \
    const Il2CppType* type() const override { \
        ::il2cpp_functions::Init(); \
        return ::il2cpp_functions::class_get_type(___TypeRegistration::klass_ptr); \
    } \
    constexpr uint16_t fieldAttributes() const override { \
        return FIELD_ATTRIBUTE_PUBLIC | FIELD_ATTRIBUTE_STATIC; \
    } \
    constexpr size_t size() const override { \
        return sizeof(type_); \
    } \
    int32_t offset() const override { \
        return oft; \
    } \
}; \
static inline ___StaticFieldRegistrator_##name_ ___##name_##_StaticFieldRegistrator; \
public: \
static type_& name_() { \
    CRASH_UNLESS(___TargetType::___TypeRegistration::st_fields); \
    return *reinterpret_cast<type_*>(&___TargetType::___TypeRegistration::st_fields[___##name_##_StaticFieldRegistrator.offset()]); \
}

DECLARE_CLASS_CODEGEN(Qosmetics, SingletonContainer, Il2CppObject,
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::QosmeticsFlowCoordinator*, qosmeticsFlowCoordinator);
    DECLARE_INSTANCE_FIELD(Qosmetics::SaberManager*, saberManager);
    DECLARE_INSTANCE_FIELD(Qosmetics::NoteManager*, noteManager);
    DECLARE_INSTANCE_FIELD(Qosmetics::WallManager*, wallManager);
    DECLARE_INSTANCE_FIELD(Qosmetics::ColorManager*, colorManager);

    DECLARE_INSTANCE_FIELD(Qosmetics::UI::SaberSwitcherViewController*, saberSwitcherViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::SaberSettingsViewController*, saberSettingsViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::SaberPreviewViewController*, saberPreviewViewController);
    
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::NoteSwitcherViewController*, noteSwitcherViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::NoteSettingsViewController*, noteSettingsViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::NotePreviewViewController*, notePreviewViewController);

    DECLARE_INSTANCE_FIELD(Qosmetics::UI::WallSwitcherViewController*, wallSwitcherViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::WallSettingsViewController*, wallSettingsViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::WallPreviewViewController*, wallPreviewViewController);

    DECLARE_INSTANCE_FIELD(Qosmetics::UI::QosmeticsViewController*, qosmeticsViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::PatronViewController*, patronViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::FloorLogoViewController*, floorLogoViewController);
    DECLARE_INSTANCE_FIELD(Qosmetics::UI::UserProfileViewController*, userProfileViewController);

    DECLARE_STATIC_METHOD(Qosmetics::SingletonContainer*, get_instance);
    DECLARE_STATIC_METHOD(Qosmetics::UI::QosmeticsFlowCoordinator*, get_qosmeticsFlowCoordinator);
    DECLARE_STATIC_METHOD(Qosmetics::SaberManager*, get_saberManager);
    DECLARE_STATIC_METHOD(Qosmetics::NoteManager*, get_noteManager);
    DECLARE_STATIC_METHOD(Qosmetics::WallManager*, get_wallManager);
    DECLARE_STATIC_METHOD(Qosmetics::ColorManager*, get_colorManager);

    DECLARE_STATIC_METHOD(Qosmetics::UI::SaberSwitcherViewController*, get_saberSwitcherViewController);
    DECLARE_STATIC_METHOD(Qosmetics::UI::SaberSettingsViewController*, get_saberSettingsViewController);
    DECLARE_STATIC_METHOD(Qosmetics::UI::SaberPreviewViewController*, get_saberPreviewViewController);
    
    DECLARE_STATIC_METHOD(Qosmetics::UI::NoteSwitcherViewController*, get_noteSwitcherViewController);
    DECLARE_STATIC_METHOD(Qosmetics::UI::NoteSettingsViewController*, get_noteSettingsViewController);
    DECLARE_STATIC_METHOD(Qosmetics::UI::NotePreviewViewController*, get_notePreviewViewController);

    DECLARE_STATIC_METHOD(Qosmetics::UI::WallSwitcherViewController*, get_wallSwitcherViewController);
    DECLARE_STATIC_METHOD(Qosmetics::UI::WallSettingsViewController*, get_wallSettingsViewController);
    DECLARE_STATIC_METHOD(Qosmetics::UI::WallPreviewViewController*, get_wallPreviewViewController);

    DECLARE_STATIC_METHOD(Qosmetics::UI::QosmeticsViewController*, get_qosmeticsViewController);
    DECLARE_STATIC_METHOD(Qosmetics::UI::PatronViewController*, get_patronViewController);
    DECLARE_STATIC_METHOD(Qosmetics::UI::FloorLogoViewController*, get_floorLogoViewController);
    DECLARE_STATIC_METHOD(Qosmetics::UI::UserProfileViewController*, get_userProfileViewController);

    //MY_DECLARE_STATIC_FIELD(Qosmetics::SingletonContainer*, instance);

    DECLARE_CTOR(ctor);

    public:
        static void Init();
        static void Delete();
    private:
        static inline SingletonContainer* instance = nullptr;
        //static SafePtr<SingletonContainer>* instance;
)