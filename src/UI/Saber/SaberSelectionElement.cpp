#include "Config.hpp"
#include "UI/Saber/SaberSelectionElement.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Data/CustomDataType.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/WaitUntil.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "System/Func_1.hpp"

#include "Utils/TextUtils.hpp"
#include "Utils/UnityUtils.hpp"

#include "Data/CreatorCache.hpp"

#include "QosmeticsLogger.hpp"

#include "System/Collections/IEnumerator.hpp"

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace Qosmetics;
using namespace Qosmetics::UI;
using namespace TMPro;

DEFINE_TYPE(Qosmetics::UI::SaberSelectionElement);

#define INFO(value...) QosmeticsLogger::GetContextLogger("Saber Selector").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Saber Selector").error(value)

static Il2CppString* textLayoutName = nullptr;
static Il2CppString* authorName = nullptr;
static Il2CppString* nameName = nullptr;

extern config_t config;

using IEnumerator = System::Collections::IEnumerator;

static void SetupName(VerticalLayoutGroup* layout, std::string name)
{
    if (TextUtils::shouldRainbow(name)) name = TextUtils::rainbowify(name);
    TextMeshProUGUI* text = CreateText(layout->get_transform(), name);
    if (!nameName) nameName = il2cpp_utils::createcsstr("Name", il2cpp_utils::StringType::Manual);
    text->get_gameObject()->set_name(nameName);
    text->get_gameObject()->AddComponent<LayoutElement*>()->set_preferredWidth(45.0f);
}

static void SetupAuthor(VerticalLayoutGroup* layout, std::string author)
{
    Color color = CreatorCache::GetCreatorColor(author);
    if (TextUtils::shouldRainbow(color)) author = TextUtils::rainbowify(author);
    TextMeshProUGUI* text = CreateText(layout->get_transform(), author);
    if (!authorName) authorName = il2cpp_utils::createcsstr("Author", il2cpp_utils::StringType::Manual);
    text->get_gameObject()->set_name(authorName);
    text->set_color(color);
    text->set_fontSize(text->get_fontSize() * 0.8f);
    text->get_gameObject()->AddComponent<LayoutElement*>()->set_preferredWidth(45.0f);
}

static void SetupSelect(HorizontalLayoutGroup* layout, SaberSelectionElement* self)
{
    Button* deleteButton = CreateUIButton(layout->get_transform(), "<color=#88ff88>select</color>", [self]{
        self->Select();
    });
}

static void SetupDelete(HorizontalLayoutGroup* layout, SaberSelectionElement* self)
{
    Button* deleteButton = CreateUIButton(layout->get_transform(), "<color=#ff8888>delete</color>", [self]{
        self->Delete();
    });
}

static void SetupDescription(HorizontalLayoutGroup* layout, std::string description)
{
    AddHoverHint(layout->get_gameObject(), description);
}

namespace Qosmetics::UI
{
    void SaberSelectionElement::Init(SaberManager* saberManager, SaberPreviewViewController* previewViewController)
    {
        modelManager = saberManager;
        this->previewViewController = previewViewController;
        if (!textLayoutName) textLayoutName = il2cpp_utils::createcsstr("TextLayout", il2cpp_utils::StringType::Manual);
    }

    void SaberSelectionElement::Select()
    {
        std::string saberName = descriptor->GetFileName();
        previewViewController->ShowLoading();
        modelManager->SetActiveSaber(saberName, true);
        this->UpdateData();
    }

    void SaberSelectionElement::Delete()
    {
        SaberItem& item = modelManager->get_item();
        // if selected was the current, set to default
        if (item.get_descriptor().GetFileName() == descriptor->GetFileName()) modelManager->SetDefault();
        Object::Destroy(get_gameObject());
    }
    
    void SaberSelectionElement::UpdateData()
    {
        INFO("Updating selector data");
        SaberItem& item = modelManager->get_item();
        Descriptor& descriptor = item.get_descriptor();

        Transform* textGroup = get_transform()->Find(textLayoutName);
        Transform* author = textGroup->Find(authorName);
        Transform* name = textGroup->Find(nameName);

        TextMeshProUGUI* nameText = name->get_gameObject()->GetComponent<TextMeshProUGUI*>();
        
        std::string nameString = descriptor.get_name();
        if (TextUtils::shouldRainbow(nameString)) nameString = TextUtils::rainbowify(nameString);
        nameText->set_text(il2cpp_utils::createcsstr("<i>" + nameString + "</i>"));

        std::string authorName = descriptor.get_author();
        Color color = CreatorCache::GetCreatorColor(authorName);
        if (TextUtils::shouldRainbow(color)) authorName = TextUtils::rainbowify(authorName);

        TextMeshProUGUI* authorText = author->get_gameObject()->GetComponent<TextMeshProUGUI*>();
        authorText->set_text(il2cpp_utils::createcsstr("<i>" + authorName + "</i>"));
        authorText->set_color(color);
        
        HoverHint* hoverHint = GetComponent<HoverHint*>();
        hoverHint->set_text(il2cpp_utils::createcsstr(descriptor.get_description()));

        previewViewController->UpdatePreview(true);
        config.lastActiveSaber = descriptor.GetFileName();
        SaveConfig();
    }

    void SaberSelectionElement::SetDescriptor(Descriptor* descriptor)
    {
        this->descriptor = descriptor;
        SetupButtons();
    }

    void SaberSelectionElement::SetupButtons()
    {
        StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(ButtonSetupRoutine())));
    }

    custom_types::Helpers::Coroutine SaberSelectionElement::ButtonSetupRoutine()
    {
        VerticalLayoutGroup* textGroup;
        HorizontalLayoutGroup* layout = GetComponent<HorizontalLayoutGroup*>();
        std::string name;

        UnityUtils::GetAddComponent<Backgroundable*>(this->get_gameObject())->ApplyBackgroundWithAlpha(il2cpp_utils::createcsstr("round-rect-panel"), 0.5f);
        co_yield nullptr;

        textGroup = CreateVerticalLayoutGroup(this->get_transform());
        textGroup->get_gameObject()->set_name(textLayoutName);
        co_yield nullptr;

        name = this->descriptor->get_name();
        if (name == "") name = this->descriptor->GetFileName(true);
        SetupName(textGroup, name);
        co_yield nullptr;

        SetupAuthor(textGroup, this->descriptor->get_author());
        co_yield nullptr;

        SetupSelect(layout, this);
        co_yield nullptr;

        SetupDelete(layout, this);
        co_yield nullptr;

        SetupDescription(layout, this->descriptor->get_description());
        co_return;
    }

    Descriptor& SaberSelectionElement::get_descriptor()
    {
        CRASH_UNLESS(descriptor);
        return *descriptor;
    }
}