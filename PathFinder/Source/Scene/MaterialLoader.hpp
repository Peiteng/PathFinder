#pragma once

#include "Material.hpp"
#include "TextureLoader.hpp"

#include "../RenderPipeline/AssetResourceStorage.hpp"
#include "../HardwareAbstractionLayer/Buffer.hpp"

#include <filesystem>
#include <string>
#include <optional>

namespace PathFinder 
{

    class MaterialLoader
    {
    public:
        inline static const Geometry::Dimensions UncompressedDistanceFieldSize{ 128, 128, 64 };

        MaterialLoader(const std::filesystem::path& fileRoot, const HAL::Device* device,);

        Material LoadMaterial(
            const std::string& albedoMapRelativePath,
            const std::string& normalMapRelativePath,
            const std::string& roughnessMapRelativePath,
            const std::string& metalnessMapRelativePath,
            std::optional<std::string> displacementMapRelativePath = std::nullopt,
            std::optional<std::string> distanceMapRelativePath = std::nullopt,
            std::optional<std::string> AOMapRelativePath = std::nullopt);

        void SerializePostprocessedTextures();

    private:
        struct SerializationData
        {
            std::string DistanceMapReltivePath;
            const HAL::Buffer* DistanceAtlasIndirectionBuffer;
            const HAL::Buffer* DistanceAtlasBuffer;
            const HAL::Buffer* DistanceAtlasCounterBuffer;
        };

        const HAL::Device* mDevice;
        AssetResourceStorage* mAssetStorage;
        TextureLoader mTextureLoader;
    };

}
