#include "UI/Components/QosmeticObjectTableCell.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "HMUI/Touchable.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

DEFINE_TYPE(Qosmetics::Core, QosmeticObjectTableCell);

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI::BeatSaberUI;

#define SetHorizontalFitMode(obj, mode)                                               \
    auto obj##SizeFitter = obj->get_gameObject()->GetComponent<ContentSizeFitter*>(); \
    if (!obj##SizeFitter)                                                             \
        obj##SizeFitter = obj->get_gameObject()->AddComponent<ContentSizeFitter*>();  \
    obj##SizeFitter->set_horizontalFit(mode)

VerticalLayoutGroup* CreateHost(Transform* parent, Vector2 anchoredPos,
                                Vector2 size)
{
    VerticalLayoutGroup* group = CreateVerticalLayoutGroup(parent);
    group->get_rectTransform()->set_anchoredPosition(anchoredPos);

    LayoutElement* elem = group->GetComponent<LayoutElement*>();
    elem->set_preferredHeight(size.y);
    elem->set_preferredWidth(size.x);

    ContentSizeFitter* fitter = group->GetComponent<ContentSizeFitter*>();
    fitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
    fitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
    return group;
}

namespace Qosmetics::Core
{
    Qosmetics::Core::QosmeticObjectTableCell* QosmeticObjectTableCell::CreateNewCell()
    {
        static auto playerTableCellStr = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("GlobalLeaderboardTableCell");
        auto cellGO = UnityEngine::GameObject::New_ctor();
        auto playerCell = cellGO->AddComponent<QosmeticObjectTableCell*>();
        cellGO->set_name(playerTableCellStr);
        playerCell->Setup();
        return playerCell;
    }

    void QosmeticObjectTableCell::Setup()
    {
        get_gameObject()->AddComponent<HMUI::Touchable*>();
        set_interactable(false);

        auto cellHorizontal = CreateHorizontalLayoutGroup(get_transform());

        auto layout = cellHorizontal->get_gameObject()->GetComponent<LayoutElement*>();
        layout->set_preferredHeight(12.0f);
        layout->set_preferredWidth(200.0f);

        auto t = get_transform();

        auto textVertical = CreateVerticalLayoutGroup(cellHorizontal->get_transform());
        name = CreateText(textVertical->get_transform(), "---", {0, 0}, {0, 0});
        sub = CreateText(textVertical->get_transform(), "---", {0, 0}, {0, 0});

        auto paddingHorizontal = CreateHorizontalLayoutGroup(cellHorizontal->get_transform());
        SetHorizontalFitMode(paddingHorizontal, ContentSizeFitter::FitMode::Unconstrained);

        auto buttonHorizontal = CreateHorizontalLayoutGroup(cellHorizontal->get_transform());

        auto selectBtn = CreateUIButton(buttonHorizontal->get_transform(), "Select", Vector2(0, 0), Vector2(0, 0), std::bind(&QosmeticObjectTableCell::Select, this));
        auto deleteBtn = CreateUIButton(buttonHorizontal->get_transform(), "Delete", Vector2(0, 0), Vector2(0, 0), std::bind(&QosmeticObjectTableCell::AttemptDelete, this));

        hover = AddHoverHint(get_gameObject(), "---");
    }

    void QosmeticObjectTableCell::Select()
    {
        if (onSelect) onSelect(this);
    }

    void QosmeticObjectTableCell::AttemptDelete()
    {
        deletionConfirmationModal->Show(this);
    }

    void QosmeticObjectTableCell::Delete()
    {
        if (onDelete) onDelete(this);
    }

    void QosmeticObjectTableCell::SetDescriptor(Descriptor descriptor)
    {
        this->descriptor = descriptor;
        set_name(to_utf16(descriptor.get_name()));
        set_sub(to_utf16(descriptor.get_author()));
        set_hover(to_utf16(descriptor.get_description()));
    }

    void QosmeticObjectTableCell::set_name(std::u16string_view name)
    {
        this->name->set_text(il2cpp_utils::newcsstr(name));
    }

    void QosmeticObjectTableCell::set_sub(std::u16string_view sub)
    {
        this->sub->set_text(il2cpp_utils::newcsstr(sub));
    }

    void QosmeticObjectTableCell::set_hover(std::u16string_view hover)
    {
        this->hover->set_text(il2cpp_utils::newcsstr(hover));
    }

}