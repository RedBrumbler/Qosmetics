#pragma once
#include "Data/Descriptor.hpp"
#include <string>
#include "static-defines.hpp"

namespace Qosmetics
{
    class DescriptorCache
    {
        public:
            static void AddToSaberCache(Descriptor* descriptor);
            static void AddToNoteCache(Descriptor* descriptor);
            static void AddToWallCache(Descriptor* descriptor);
            
            static DescriptorCache& GetCache()
            {
                CreateCacheIfNeeded();
                return *cache;
            }

            static Descriptor* GetDescriptor(std::string fileName, qosmeticsType type);

            static std::vector<Descriptor*>& GetSaberDescriptors()
            {
                return cache->saberDescriptors;
            }

            static std::vector<Descriptor*>& GetNoteDescriptors()
            {
                return cache->noteDescriptors;
            }

            static std::vector<Descriptor*>& GetWallDescriptors()
            {
                return cache->wallDescriptors;
            }

            static bool Load();

            static void Write();
        private:
            rapidjson::Document cacheDocument = rapidjson::Document();
            static inline DescriptorCache* cache = nullptr; 

            static inline Descriptor* invalid = new Descriptor(qosmeticsType::invalid);

            static inline std::vector<Descriptor*> saberDescriptors = {};
            static inline std::vector<Descriptor*> noteDescriptors = {};
            static inline std::vector<Descriptor*> wallDescriptors = {};

            void AddDescriptorToValue(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator, Descriptor* descriptor);
            void AddDescriptorVectorToCache(std::vector<Descriptor*>& descriptors, rapidjson::Document::AllocatorType& allocator, std::string name);
            void LoadDescriptorsFromCacheIntoVector(std::string cacheName, std::vector<Descriptor*>& cacheVector);
            Descriptor* LoadDescriptorFromValue(const rapidjson::Value& value);
            void WriteDocument();

            void static CreateCacheIfNeeded(bool alsoLoad = true)
            {
                if (!cache) 
                {
                    cache = new DescriptorCache();
                    if (alsoLoad) cache->Load();
                }
            }
    };
}