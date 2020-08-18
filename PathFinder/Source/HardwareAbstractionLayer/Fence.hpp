#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <cstdint>

#include "GraphicAPIObject.hpp"
#include "Device.hpp"

namespace HAL
{
    class Fence : public GraphicAPIObject
    {
    public:
        Fence(const Device& device);

        uint64_t IncrementExpectedValue();
        bool IsCompleted() const;
        void StallCurrentThreadUntilCompletion(uint8_t allowedSimultaneousFramesCount = 1);
    
    private:
        void SetCompletionEventHandle(HANDLE handle);

        Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
        uint64_t mExpectedValue = 0;
    
    public:
        inline ID3D12Fence* D3DFence() const { return mFence.Get(); }
        inline uint64_t ExpectedValue() const { return mExpectedValue; }
        inline uint64_t CompletedValue() const { return mFence->GetCompletedValue(); }
    };
}

