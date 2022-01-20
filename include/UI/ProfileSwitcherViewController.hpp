#pragma once

#include "HMUI/ViewController.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"
#include "questui/shared/CustomTypes/Components/List/CustomListTableData.hpp"
#include <string>
#include <vector>

#define DECLARE_OVERRIDE_METHOD_MATCH(retval, name, mptr, ...) \
    DECLARE_OVERRIDE_METHOD(retval, name, il2cpp_utils::il2cpp_type_check::MetadataGetter<mptr>::get(), __VA_ARGS__)

DECLARE_CLASS_CODEGEN(Qosmetics::Core, ProfileSwitcherViewController, HMUI::ViewController,
                      DECLARE_INSTANCE_FIELD(QuestUI::CustomListTableData*, userList);

                      DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

                      void AddUser(std::string_view user);
                      void RefreshUsers();
                      void SelectUser(int idx);

)