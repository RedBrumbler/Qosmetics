#include "Types/Saber/SaberItem.hpp"
#include "QosmeticsLogger.hpp"

#define INFO(value...) QosmeticsLogger::GetContextLogger("Saber Item").info(value)
#define ERROR(value...) QosmeticsLogger::GetContextLogger("Saber Item").error(value)

using namespace UnityEngine;

namespace Qosmetics
{
    void SaberItem::ConfigCallback(TextAsset* textAsset)
    {
        if (!textAsset)
        {
            ERROR("Config was nullptr! did someone fuck with the exporter, or is the read broken? regardless keeping Default config");
            return;
        }

        Il2CppString* descriptorstring = textAsset->get_text();
        
        std::string json = to_utf8(csstrtostr(descriptorstring));

        rapidjson::Document d;
        d.Parse(json.c_str());

        config = SaberConfig(d);
    }

    void SaberItem::DescriptorCallback(TextAsset* textAsset)
    {
        if (!textAsset)
        {
            ERROR("Descriptor was nullptr! did someone fuck with the exporter, or is the read broken? regardless setting legacy descriptor");
            Descriptor temp = Descriptor("---", descriptor.GetFileName(true), "Legacy Saber", descriptor.get_filePath());
            descriptor.CopyFrom(temp);
            return;
        }

        Il2CppString* descriptorstring = textAsset->get_text();
        
        std::string json = to_utf8(csstrtostr(descriptorstring));

        rapidjson::Document d;
        d.Parse(json.c_str());

        Descriptor temp = Descriptor(d, descriptor.get_filePath());
        descriptor.CopyFrom(temp);
    }
}