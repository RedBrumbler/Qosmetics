#pragma once

#include "Data/Descriptor.hpp"
#include "HMUI/HoverHint.hpp"
#include "HMUI/TableCell.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UI/Components/DeletionConfirmationModal.hpp"
#include "UI/Components/QosmeticObjectTableData.hpp"
#include "custom-types/shared/macros.hpp"
#include <functional>

DECLARE_CLASS_CODEGEN(Qosmetics::Core, QosmeticObjectTableCell, HMUI::TableCell,

                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, name);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, sub);
                      DECLARE_INSTANCE_FIELD(HMUI::HoverHint*, hover);

                      DECLARE_INSTANCE_FIELD(DeletionConfirmationModal*, deletionConfirmationModal);
                      DECLARE_INSTANCE_FIELD(QosmeticObjectTableData*, tableData);
                      public
                      :

                      static Qosmetics::Core::QosmeticObjectTableCell * CreateNewCell();
                      void Setup();

                      void Select();
                      void Delete();
                      void AttemptDelete();

                      void SetDescriptor(Descriptor descriptor);
                      void set_name(std::u16string_view name);
                      void set_sub(std::u16string_view sub);
                      void set_hover(std::u16string_view hover);

                      std::function<void(QosmeticObjectTableCell*)> onSelect;
                      std::function<void(QosmeticObjectTableCell*)> onDelete;
                      Descriptor descriptor;

)