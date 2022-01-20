#include "UI/Components/QosmeticObjectTableData.hpp"
#include "UI/Components/QosmeticObjectTableCell.hpp"

DEFINE_TYPE(Qosmetics::Core, QosmeticObjectTableData);

namespace Qosmetics::Core
{
    void QosmeticObjectTableData::ctor()
    {
        INVOKE_CTOR();
        reuseIdentifier = il2cpp_utils::newcsstr("QosmeticObjectCellList");
        cellSize = 12.0f;
    }

    float QosmeticObjectTableData::CellSize()
    {
        return cellSize;
    }

    int QosmeticObjectTableData::NumberOfCells()
    {
        return objectDescriptors.size();
    }

    HMUI::TableCell*
    QosmeticObjectTableData::CellForIdx(
        HMUI::TableView* tableView, int idx)
    {
        if (getCellForIdx(tableView, idx)) return getCellForIdx(tableView, idx);

        auto tableCell = reinterpret_cast<QosmeticObjectTableCell*>(tableView->DequeueReusableCellForIdentifier(reuseIdentifier));

        if (!tableCell)
        {
            tableCell = QosmeticObjectTableCell::CreateNewCell();
            tableCell->set_interactable(false);
        }

        tableCell->tableData = this;
        tableCell->deletionConfirmationModal = deletionConfirmationModal;
        tableCell->set_reuseIdentifier(reuseIdentifier);
        tableCell->SetDescriptor(objectDescriptors[idx]);

        return tableCell;
    }

    void QosmeticObjectTableData::Refresh()
    {
        if (refresh) refresh();
    }
}
