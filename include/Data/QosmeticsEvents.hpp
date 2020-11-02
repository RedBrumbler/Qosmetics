#pragma once
#include <string>
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Animator.hpp"
namespace Qosmetics
{
    class EventManager;
    
    class QosmeticsEvent
    {
        public:
            typedef enum _EffectType
            {
                GameObject = 0,
                Bool = 1,
                Trigger = 2
            } EffectType;

            typedef enum _Event
            {
                OnSlice,
                OnCombo,
                OnEveryNCombo,
                OnComboBreak,
                MultiplierUp,
                SaberStartColliding,
                SaberStopColliding,
                OnLevelStart,
                OnLevelFail,
                OnLevelEnded,
                OnLeftLightOn,
                OnLeftLightOff,
                OnRightLightOn,
                OnRightLightOff
            } Event;

            EffectType effect;
            Event type;

            int integer;
            bool boolean;
            std::string nameToSet;

            std::string objectPath;

            virtual void RunEvent(UnityEngine::GameObject* object);

            UnityEngine::Animator* animator = nullptr;

        private:
    };

    class SliceEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class ComboEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class EveryNEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class ComboBreakEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class MultiplierUpEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class SaberStartCollidingEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class SaberStopCollidingEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class LevelStartEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class LevelFailedEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class LevelEndedEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class LeftLightOnEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class LeftLightOffEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class RightLightOnEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };

    class RightLightOffEvent : QosmeticsEvent
    {
        public:
            void RunEvent(UnityEngine::GameObject* object);
    };
}