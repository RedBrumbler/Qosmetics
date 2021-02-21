#include "Containers/FlowCoordinatorContainer.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "UI/General/QosmeticsFlowCoordinator.hpp"

DEFINE_CLASS(Qosmetics::FlowCoordinatorContainer);

void Qosmetics::FlowCoordinatorContainer::ctor(Zenject::DiContainer* dicontainer)
{
    //container = dicontainer;
    static auto name = il2cpp_utils::createcsstr("QosmeticsFlowCoordinator", il2cpp_utils::StringType::Manual);
    flowCoordinatorGO = UnityEngine::GameObject::New_ctor(name);
    dicontainer->InstantiateComponent<Qosmetics::UI::QosmeticsFlowCoordinator*>(flowCoordinatorGO);
}