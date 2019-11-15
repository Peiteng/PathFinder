#pragma once

#include "PipelineResourceStorage.hpp"
#include "ResourceDescriptorStorage.hpp"

#include "../Foundation/Name.hpp"
#include "../HardwareAbstractionLayer/ShaderRegister.hpp"

namespace PathFinder
{

    class ResourceProvider
    {
    public:
        ResourceProvider(const PipelineResourceStorage* storage, const ResourceDescriptorStorage* descriptorStorage);
       
        uint32_t GetTextureDescriptorTableIndex(Foundation::Name resourceName) const;
        uint32_t GetExternalTextureDescriptorTableIndex(const HAL::TextureResource* texture, HAL::ShaderRegister registerType);

    private:
        const PipelineResourceStorage* mResourceStorage;
        const ResourceDescriptorStorage* mDescriptorStorage;
    };

}
