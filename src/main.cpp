#include "UI/GameplaySetupView.hpp"
#include "custom-types/shared/register.hpp"
#include "hooks.hpp"
#include "logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "questui/shared/QuestUI.hpp"

#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ImageView.hpp"

ModInfo modInfo = {ID, VERSION};

extern "C" void setup(ModInfo& info)
{
    info = modInfo;
}

extern "C" void load()
{
    il2cpp_functions::Class_Init(classof(HMUI::ImageView*));
    il2cpp_functions::Class_Init(classof(HMUI::CurvedTextMeshPro*));

    Hooks::InstallHooks(Qosmetics::Core::Logging::getLogger());
    custom_types::Register::AutoRegister();

    QuestUI::Register::RegisterGameplaySetupMenu<Qosmetics::Core::GameplaySetupView*>(modInfo, "Qosmetics");
}