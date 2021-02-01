#include "Saber/SaberItem.hpp"

using namespace UnityEngine;

namespace Qosmetics
{
    void SaberItem::ConfigCallback(TextAsset* textAsset)
    {
        if (!textAsset)
        {
            return;
        }
    }

    void SaberItem::DescriptorCallback(TextAsset* textAsset)
    {
        if (!textAsset)
        {
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