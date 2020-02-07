#include "../Foundation/StringUtils.hpp"

namespace PathFinder
{

    template <class CommandListT, class CommandQueueT>
    AsyncComputeDevice<CommandListT, CommandQueueT>::AsyncComputeDevice(
        const HAL::Device& device,
        const HAL::CBSRUADescriptorHeap* universalGPUDescriptorHeap,
        Memory::PoolCommandListAllocator* commandListAllocator,
        PipelineResourceStorage* resourceStorage, 
        PipelineStateManager* pipelineStateManager,
        const RenderSurfaceDescription& defaultRenderSurface)
        :
        mCommandQueue{ device },
        mUniversalGPUDescriptorHeap{ universalGPUDescriptorHeap },
        mCommandListAllocator{ commandListAllocator },
        mResourceStorage{ resourceStorage },
        mPipelineStateManager{ pipelineStateManager },
        mDefaultRenderSurface{ defaultRenderSurface }
    {
        mCommandQueue.SetDebugName("Async Compute Device Command Queue");
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::BindBuffer(
        Foundation::Name resourceName, uint16_t shaderRegister, uint16_t registerSpace, HAL::ShaderRegister registerType)
    {
        const BufferPipelineResource* resource = mResourceStorage->GetPipelineBufferResource(resourceName);
        assert_format(resource, "Buffer ' ", resourceName.ToString(), "' doesn't exist");

        BindExternalBuffer(*resource->Resource, shaderRegister, registerSpace, registerType);
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::BindExternalBuffer(
        const Memory::Buffer& buffer, uint16_t shaderRegister, uint16_t registerSpace, HAL::ShaderRegister registerType)
    {
        if (mAppliedComputeState || mAppliedRayTracingState)
        {
            auto index = mAppliedComputeState->GetRootSignature()->GetParameterIndex({ shaderRegister, registerSpace, registerType });

            assert_format(index, "Root signature parameter doesn't exist");

            // Will be changed if I'll see render passes that require a lot of buffers. 
            assert_format(!index->IsIndirect, "Descriptor tables for buffers are not supported. Bind buffers directly instead.");

            switch (registerType)
            {
            case HAL::ShaderRegister::ShaderResource: mCommandList->SetComputeRootShaderResource(*buffer.HALBuffer(), index->IndexInSignature); break;
            case HAL::ShaderRegister::ConstantBuffer: mCommandList->SetComputeRootConstantBuffer(*buffer.HALBuffer(), index->IndexInSignature); break;
            case HAL::ShaderRegister::UnorderedAccess: mCommandList->SetComputeRootUnorderedAccessResource(*buffer.HALBuffer(), index->IndexInSignature); break;
            case HAL::ShaderRegister::Sampler:
                assert_format(false, "Incompatible register type");
            }

            return;
        }
        else {
            assert_format(false, "No pipeline state applied");
        }
    }

    template <class CommandListT, class CommandQueueT>
    template <class T>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::SetRootConstants(
        const T& constants, uint16_t shaderRegister, uint16_t registerSpace)
    {
        auto index = mAppliedComputeState->GetRootSignature()->GetParameterIndex({shaderRegister, registerSpace,  HAL::ShaderRegister::ConstantBuffer });
        assert_format(index, "Root signature parameter doesn't exist");
        mCommandList->SetComputeRootConstants(constants, index->IndexInSignature);
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::ApplyCommonComputeResourceBindings()
    {
        mCommandList->SetDescriptorHeap(*mUniversalGPUDescriptorHeap);

        // Look at PipelineStateManager for base root signature parameter ordering
        /*mCommandList->SetComputeRootConstantBuffer(mResourceStorage->GlobalRootConstantsBuffer(), 0);
        mCommandList->SetComputeRootConstantBuffer(mResourceStorage->PerFrameRootConstantsBuffer(), 1);*/

        HAL::DescriptorAddress SRRangeAddress = mUniversalGPUDescriptorHeap->RangeStartGPUAddress(HAL::CBSRUADescriptorHeap::Range::ShaderResource);
        HAL::DescriptorAddress UARangeAddress = mUniversalGPUDescriptorHeap->RangeStartGPUAddress(HAL::CBSRUADescriptorHeap::Range::UnorderedAccess);

        // Alias different registers to one GPU address
        mCommandList->SetComputeRootDescriptorTable(SRRangeAddress, 3);
        mCommandList->SetComputeRootDescriptorTable(SRRangeAddress, 4);
        mCommandList->SetComputeRootDescriptorTable(SRRangeAddress, 5);
        mCommandList->SetComputeRootDescriptorTable(SRRangeAddress, 6);
        mCommandList->SetComputeRootDescriptorTable(SRRangeAddress, 7);

        mCommandList->SetComputeRootDescriptorTable(UARangeAddress, 8);
        mCommandList->SetComputeRootDescriptorTable(UARangeAddress, 9);
        mCommandList->SetComputeRootDescriptorTable(UARangeAddress, 10);
        mCommandList->SetComputeRootDescriptorTable(UARangeAddress, 11);
        mCommandList->SetComputeRootDescriptorTable(UARangeAddress, 12);
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::BindCurrentPassBuffersCompute()
    {
       /* if (auto buffer = mResourceStorage->RootConstantBufferForCurrentPass())
        {
            mCommandList->SetComputeRootConstantBuffer(*buffer, 2);
        }

        mCommandList->SetComputeRootUnorderedAccessResource(*mResourceStorage->DebugBufferForCurrentPass(), 13);*/
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::BeginFrame(uint64_t newFrameNumber)
    {
        mCommandList = mCommandListAllocator->AllocateCommandList<CommandListT>();
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::EndFrame(uint64_t completedFrameNumber)
    {
        
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::ResetCommandList()
    {
        mCommandList->Reset();
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::ExecuteCommands(const HAL::Fence* fenceToWaitFor, const HAL::Fence* fenceToSignal)
    {
        if (fenceToWaitFor)
        {
            mCommandQueue.WaitFence(*fenceToWaitFor);
        }

        mCommandList->Close();
        mCommandQueue.ExecuteCommandList(*mCommandList);

        if (fenceToSignal)
        {
            mCommandQueue.SignalFence(*fenceToSignal);
        }        
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
    {
        mCommandList->Dispatch(groupCountX, groupCountY, groupCountZ);
        mCommandList->InsertBarriers(mResourceStorage->UnorderedAccessBarriersForCurrentPass());
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::ApplyPipelineState(Foundation::Name psoName)
    {
        const PipelineStateManager::PipelineStateVariant* state = mPipelineStateManager->GetPipelineState(psoName);
        assert_format(state, "Pipeline state doesn't exist");
        assert_format(!std::holds_alternative<HAL::GraphicsPipelineState>(*state), "Trying to apply graphics pipeline state to compute device");

        if (std::holds_alternative<HAL::ComputePipelineState>(*state))
        {
            ApplyStateIfNeeded(&std::get<HAL::ComputePipelineState>(*state));
        }
        else if (std::holds_alternative<HAL::RayTracingPipelineState>(*state))
        {
            ApplyStateIfNeeded(&std::get<HAL::RayTracingPipelineState>(*state));
        } 
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::ApplyStateIfNeeded(const HAL::ComputePipelineState* state)
    {
        bool computeStateApplied = mAppliedComputeState != nullptr;

        // State is already applied
        //
        if (computeStateApplied && mAppliedComputeState == state) return;

        mCommandList->SetPipelineState(*state);

        // Skip setting common bindings when workload type and root signature are not going to change
        //
        if (computeStateApplied && state->GetRootSignature() == mAppliedComputeRootSignature)
        {
            BindCurrentPassBuffersCompute();
            return;
        }

        mCommandList->SetComputeRootSignature(*state->GetRootSignature());
        ApplyCommonComputeResourceBindings();
        BindCurrentPassBuffersCompute();

        mAppliedComputeRootSignature = state->GetRootSignature();
        mAppliedComputeState = state;
        mAppliedRayTracingState = nullptr;
    }

    template <class CommandListT, class CommandQueueT>
    void AsyncComputeDevice<CommandListT, CommandQueueT>::ApplyStateIfNeeded(const HAL::RayTracingPipelineState* state)
    {
        bool rayTracingStateApplied = mAppliedRayTracingState != nullptr;

        // State is already applied
        //
        if (rayTracingStateApplied && mAppliedRayTracingState == state) return;

        // Set RT state
        //mCommandList->SetPipelineState(*pso);

        // Skip setting common bindings when workload type and root signature are not going to change
        //
        if (rayTracingStateApplied && state->GetGlobalRootSignature() == mAppliedComputeRootSignature)
        {
            BindCurrentPassBuffersCompute();
            return;
        }

        mCommandList->SetComputeRootSignature(*state->GetGlobalRootSignature());
        ApplyCommonComputeResourceBindings();
        BindCurrentPassBuffersCompute();

        mAppliedComputeRootSignature = state->GetGlobalRootSignature();
        mAppliedRayTracingState = state;
        mAppliedComputeState = nullptr;
    }

}

