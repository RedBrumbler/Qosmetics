#pragma once

#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"

class GameCoreSceneStart
{
    private:
        static inline std::vector<void (*)()> gameCoreFuncs;
    public:
    static inline void AddFunc(void(*func)())
    {
        gameCoreFuncs.push_back(func);
    }

    static inline void ExecuteCallbacks()
    {
        for (auto& func : gameCoreFuncs) func();
    }
};

#ifndef ADD_GAMECORE_START
#define ADD_GAMECORE_START(fun)\
struct GameCore_##fun_ { \
    GameCore_##fun_() { \
        GameCoreSceneStart::AddFunc(fun); \
    } \
}; \
static GameCore_##fun_ Auto_Add_ ##fun_;
#endif