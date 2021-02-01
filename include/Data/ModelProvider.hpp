#pragma once

namespace Qosmetics
{
    class QosmeticItem;

    class ModelProvider
    {
        public:
            ModelProvider() {};
            ModelProvider(QosmeticItem* item) 
            {
                this->item = item;
            };

            void LoadBundle(bool alsoLoadAssets = false);
            void LoadAssets();
        private:
            QosmeticItem* item;
    };
}