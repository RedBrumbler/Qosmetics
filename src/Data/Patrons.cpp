#include "Data/Patrons.hpp"
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "logging.hpp"

void get_string_vector(rapidjson::Document& doc, std::string_view identifier, std::vector<std::string>& out)
{
    auto memberItr = doc.FindMember(identifier.data());
    if (memberItr != doc.MemberEnd())
    {
        for (auto& name : memberItr->value.GetArray())
        {
            INFO("Found %s", name.GetString());
            out.emplace_back(name.GetString());
        }
    }
    else
    {
        ERROR("Couldn't find member %s in doc", identifier.data());
    }
}

namespace Qosmetics::Core
{
    Patrons Patrons::Parse(std::string json)
    {
        rapidjson::Document doc;
        doc.Parse(json);
        auto result = Patrons();
        if (doc.GetParseError() == 0)
        {
            get_string_vector(doc, "enthusiastic", result.enthusiastic);
            get_string_vector(doc, "amazing", result.amazing);
            get_string_vector(doc, "legendary", result.legendary);
            get_string_vector(doc, "paypal", result.paypal);
        }
        else
        {
            ERROR("Doc parse Error %d, data:\n%s", doc.GetParseError(), json.c_str());
        }

        return result;
    }

    bool Patrons::any()
    {
        // if any is over 0, return true
        return enthusiastic.size() > 0 || amazing.size() > 0 || legendary.size() > 0 || paypal.size() > 0;
    }
}