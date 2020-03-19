#pragma once

#include "GPUResource.hpp"

#include "../HardwareAbstractionLayer/Texture.hpp"

namespace Memory
{
   
    class Texture : public GPUResource
    {
    public:
        Texture(
            const HAL::Texture::Properties& properties, 
            ResourceStateTracker* stateTracker,
            SegregatedPoolsResourceAllocator* resourceAllocator,
            PoolDescriptorAllocator* descriptorAllocator,
            CopyCommandListProvider* commandListProvider);

        Texture(
            const HAL::Texture::Properties& properties,
            ResourceStateTracker* stateTracker,
            SegregatedPoolsResourceAllocator* resourceAllocator,
            PoolDescriptorAllocator* descriptorAllocator,
            CopyCommandListProvider* commandListProvider,
            const HAL::Device& device,
            const HAL::Heap& mainResourceExplicitHeap,
            uint64_t explicitHeapOffset);

        Texture(
            ResourceStateTracker* stateTracker,
            SegregatedPoolsResourceAllocator* resourceAllocator,
            PoolDescriptorAllocator* descriptorAllocator,
            CopyCommandListProvider* commandListProvider,
            HAL::Texture* existingTexture);

        ~Texture();

        const HAL::RTDescriptor* GetRTDescriptor(uint8_t mipLevel = 0) const;
        const HAL::DSDescriptor* GetDSDescriptor() const;
        const HAL::SRDescriptor* GetSRDescriptor() const;
        const HAL::UADescriptor* GetUADescriptor(uint8_t mipLevel = 0) const;

        const HAL::Texture* HALTexture() const;
        const HAL::Resource* HALResource() const override;

    protected:
        uint64_t ResourceSizeInBytes() const override;
        void ApplyDebugName() override;
        void RecordUploadCommands() override;
        void RecordReadbackCommands() override;
        void ReserveDiscriptorArrays(uint8_t mipCount);

    private:
        SegregatedPoolsResourceAllocator::TexturePtr mTexturePtr;
        HAL::Texture::Properties mProperties;

        mutable PoolDescriptorAllocator::DSDescriptorPtr mDSDescriptor;
        mutable PoolDescriptorAllocator::SRDescriptorPtr mSRDescriptor;
        mutable std::vector<PoolDescriptorAllocator::RTDescriptorPtr> mRTDescriptors;
        mutable std::vector<PoolDescriptorAllocator::UADescriptorPtr> mUADescriptors;

    public:
        inline const auto& Properties() const { return mProperties; }
    };

}

