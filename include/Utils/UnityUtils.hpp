#pragma once
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp" 
#include "beatsaber-hook/shared/utils/typedefs.h"

// NOTE TO SELF, DON'T BE AN IDIOT AND MAKE namespace A VARIABLE NAME, THAT'S WHY IT'S nameSpace WITH A CAPITAL S

class UnityUtils
{
    public:
    /// @brief returns the result of running GetComponent on gameobject with the given type, nameSpace defaults to UnityEngine
    /// @param gameObject object to run on
    /// @param type
    template <class Type>
    static Type GetComponent(UnityEngine::GameObject* gameObject, std::string type)
    {
        return *il2cpp_utils::RunMethod<Type>((Il2CppObject*)gameObject, "GetComponent", TypeFromString(type));
    };

    /// @brief returns the result of running GetComponent on gameobject with the given type
    /// @param gameObject object to run on
    /// @param nameSpace
    /// @param type
    template <class Type>
    static Type GetComponent(UnityEngine::GameObject* gameObject, std::string nameSpace, std::string type)
    {
        return *il2cpp_utils::RunMethod<Type>((Il2CppObject*)gameObject, "GetComponent", TypeFromString(nameSpace, type));
    };

    /// @brief returns the result of running GetComponent on gameobject with the given type, nameSpace defaults to UnityEngine
    /// @param transform object to run on
    /// @param type
    template <class Type>
    static Type GetComponent(UnityEngine::Transform* transform, std::string type)
    {
        UnityEngine::GameObject* object = transform->get_gameObject();
        return *il2cpp_utils::RunMethod<Type>((Il2CppObject*)object, "GetComponent", TypeFromString(type));
    };

    /// @brief returns the result of running GetComponent on gameobject with the given type
    /// @param transform object to run on
    /// @param nameSpace
    /// @param type
    template <class Type>
    static Type GetComponent(UnityEngine::Transform* transform, std::string nameSpace, std::string type)
    {
        UnityEngine::GameObject* object = transform->get_gameObject();
        return *il2cpp_utils::RunMethod<Type>((Il2CppObject*)object, "GetComponent", TypeFromString(nameSpace, type));
    };

    /// @brief gets systemtype out of 1 input string, this version defaults to UnityEngine
    /// @param type
    static Il2CppReflectionType* TypeFromString(std::string type)
    {
        return CRASH_UNLESS(il2cpp_utils::GetSystemType("UnityEngine", type));
    };

    /// @brief gets systemtype out of 2 input strings, this just saves the crash unless macro from needing to be typed all the time
    /// @param nameSpace
    /// @param type
    static Il2CppReflectionType* TypeFromString(std::string nameSpace, std::string type)
    {
        return CRASH_UNLESS(il2cpp_utils::GetSystemType(nameSpace, type));
    };
    
    /// @brief returns last object of the specified type as the given type in the template arguments
    template <class Type>
    static Type GetLastObjectOfType(Il2CppClass *klass) 
    {
        Il2CppReflectionType* klassType = il2cpp_utils::GetSystemType(klass);
        Array<Type>* objects = CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Type>*>(il2cpp_utils::GetClassFromName("UnityEngine", "Resources"), "FindObjectsOfTypeAll", klassType));
        if(objects == nullptr || objects->Length() == 0) return nullptr;
        return objects->values[objects->Length() -1];
    };

    /// @brief returns first object of the specified type as the given type in the template arguments
    template <class Type>
    static Type GetFirstObjectOfType() 
    {
        Il2CppReflectionType* klassType = il2cpp_utils::GetSystemType(classof(Type));
        Array<Type>* objects = CRASH_UNLESS(il2cpp_utils::RunMethod<Array<Type>*>(il2cpp_utils::GetClassFromName("UnityEngine", "Resources"), "FindObjectsOfTypeAll", klassType));
        if(objects == nullptr || objects->Length() == 0) return nullptr;
        return objects->values[0];
    };
};