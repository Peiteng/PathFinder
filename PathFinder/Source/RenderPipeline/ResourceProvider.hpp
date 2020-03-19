#pragma once

#include "PipelineResourceStorage.hpp"

#include "../Foundation/Name.hpp"
#include "../HardwareAbstractionLayer/ShaderRegister.hpp"

namespace PathFinder
{

    class ResourceProvider
    {
    public:
        ResourceProvider(const PipelineResourceStorage* storage);
       
        uint32_t GetUATextureIndex(Foundation::Name resourceName, uint8_t mipLevel = 0);
        uint32_t GetSRTextureIndex(Foundation::Name resourceName);
        const HAL::Texture::Properties& GetTextureProperties(Foundation::Name resourceName);

    private:
        const PipelineResourceStorage* mResourceStorage;
    };

}
