#pragma once
#include <vector>
#include <string>
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace Qosmetics
{
    class PatreonStorage
    {
        public:
            static bool get_loaded()
            {
                return loaded;
            }

            static const std::vector<std::string>& get_enthusiastic()
            {
                return enthusiastic;
            }

            static const std::vector<std::string>& get_amazing()
            {
                return amazing;
            }

            static const std::vector<std::string>& get_legendary()
            {
                return legendary;
            }

            static const std::vector<std::string>& get_paypal()
            {
                return paypal;
            }

            static void setloaded()
            {
                loaded = true;
            }

            static void Download();

            static bool get_atLeastOne()
            {
                if (paypal.size()) return true;
                if (enthusiastic.size()) return true;
                if (amazing.size()) return true;
                if (legendary.size()) return true;
                return false;
            }
        private:
            static inline bool loaded = false;
            static inline std::vector<std::string> enthusiastic = {};
            static inline std::vector<std::string> amazing = {};
            static inline std::vector<std::string> legendary = {};
            static inline std::vector<std::string> paypal = {};
    
            static void GetEnthusiastic(rapidjson::Value& val);
            static void GetAmazing(rapidjson::Value& val);
            static void GetLegendary(rapidjson::Value& val);
            static void GetPaypal(rapidjson::Value& val);

    
    };
}