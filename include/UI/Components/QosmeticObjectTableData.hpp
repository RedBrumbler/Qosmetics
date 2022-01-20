#pragma once

#include "Data/Descriptor.hpp"
#include "HMUI/TableCell.hpp"
#include "HMUI/TableView.hpp"
#include "HMUI/TableView_IDataSource.hpp"
#include "UI/Components/DeletionConfirmationModal.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "custom-types/shared/macros.hpp"
#include <functional>

#define GET_FIND_METHOD(mPtr) \
    il2cpp_utils::il2cpp_type_check::MetadataGetter<mPtr>::get()

#define DECLARE_OVERRIDE_METHOD_MATCH(retval, method, mptr, ...) \
    DECLARE_OVERRIDE_METHOD(retval, method, il2cpp_utils::il2cpp_type_check::MetadataGetter<mptr>::get(), __VA_ARGS__)

___DECLARE_TYPE_WRAPPER_INHERITANCE(Qosmetics::Core, QosmeticObjectTableData, Il2CppTypeEnum::IL2CPP_TYPE_CLASS, UnityEngine::MonoBehaviour, "Qosmetics::Core", {classof(HMUI::TableView::IDataSource*)}, 0, nullptr,

                                    DECLARE_INSTANCE_FIELD(Il2CppString*, reuseIdentifier);
                                    DECLARE_INSTANCE_FIELD(float, cellSize);
                                    DECLARE_INSTANCE_FIELD(DeletionConfirmationModal*, deletionConfirmationModal);

                                    DECLARE_CTOR(ctor);

                                    DECLARE_OVERRIDE_METHOD_MATCH(HMUI::TableCell*, CellForIdx, &HMUI::TableView::IDataSource::CellForIdx, HMUI::TableView* tableView, int idx);
                                    DECLARE_OVERRIDE_METHOD_MATCH(float, CellSize, &HMUI::TableView::IDataSource::CellSize);
                                    DECLARE_OVERRIDE_METHOD_MATCH(int, NumberOfCells, &HMUI::TableView::IDataSource::NumberOfCells);

                                    void Refresh();

                                    std::function<HMUI::TableCell*(HMUI::TableView* tableView, int idx)> getCellForIdx = nullptr;
                                    std::function<void()> refresh = nullptr;

                                    private
                                    :

                                    std::vector<Descriptor> objectDescriptors = {};

)