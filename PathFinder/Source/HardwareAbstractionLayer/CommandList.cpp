#include "CommandList.hpp"
#include "Utils.h"

namespace HAL
{

    CommandList::CommandList(const Device& device, const CommandAllocator& allocator, D3D12_COMMAND_LIST_TYPE type)
    {
        ThrowIfFailed(device.D3DPtr()->CreateCommandList(0, type, allocator.D3DPtr(), nullptr, IID_PPV_ARGS(&mList)));
    }

    CommandList::~CommandList() {}

    void CommandList::Reset(const CommandAllocator& allocator)
    {
       ThrowIfFailed(mList->Reset(allocator.D3DPtr(), nullptr));
    }

    void CommandList::Close()
    {
       ThrowIfFailed(mList->Close());
    }



    void CopyCommandListBase::TransitionResourceState(const ResourceTransitionBarrier& barrier)
    {
        mList->ResourceBarrier(1, &barrier.D3DBarrier());
    }

    void CopyCommandListBase::CopyResource(const Resource& source, Resource& destination)
    {
        mList->CopyResource(destination.D3DPtr(), source.D3DPtr());
    }

    void CopyCommandListBase::CopyTextureRegion(
        const TextureResource& source, TextureResource& destination,
        uint16_t sourceSubresource, uint16_t destinationSubresource, 
        const glm::ivec3& sourceOrigin, const glm::ivec3& destinationOrigin,
        const Geometry::Dimensions& regionDimensions)
    {
        D3D12_TEXTURE_COPY_LOCATION srcLocation{};
        D3D12_TEXTURE_COPY_LOCATION dstLocation{};

        srcLocation.pResource = source.D3DPtr();
        srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        srcLocation.SubresourceIndex = sourceSubresource;

        dstLocation.pResource = destination.D3DPtr();
        dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        dstLocation.SubresourceIndex = destinationSubresource;

        D3D12_BOX srcBox{};
        //srcBox.

        //mList->CopyTextureRegion(&dstLocation, destinationRectOrigin.x, destinationRectOrigin.y, destinationRectOrigin.z, &srcLocation, )
    }



    /* void ComputeCommandListBase::SetComputeRootConstantBuffer(const TypelessBufferResource& cbResource, uint32_t rootParameterIndex)
    { 
        mList->SetComputeRootConstantBufferView(rootParameterIndex, cbResource.D3DPtr()->GetGPUVirtualAddress());
    }

    void ComputeCommandListBase::SetComputeRootConstantBuffer(const ColorBufferResource& cbResource, uint32_t rootParameterIndex)
    {
        mList->SetComputeRootConstantBufferView(rootParameterIndex, cbResource.D3DPtr()->GetGPUVirtualAddress());
    }*/

    void ComputeCommandListBase::SetComputeRootShaderResource(const TypelessTextureResource& resource, uint32_t rootParameterIndex)
    {
        mList->SetComputeRootShaderResourceView(rootParameterIndex, resource.D3DPtr()->GetGPUVirtualAddress());
    }

    void ComputeCommandListBase::SetComputeRootShaderResource(const ColorTextureResource& resource, uint32_t rootParameterIndex)
    {
        mList->SetComputeRootShaderResourceView(rootParameterIndex, resource.D3DPtr()->GetGPUVirtualAddress());
    }

    void ComputeCommandListBase::SetComputeRootShaderResource(const DepthStencilTextureResource& resource, uint32_t rootParameterIndex)
    {
        mList->SetComputeRootShaderResourceView(rootParameterIndex, resource.D3DPtr()->GetGPUVirtualAddress());
    }

    void ComputeCommandListBase::SetComputeRootUnorderedAccessResource(const TypelessTextureResource& resource, uint32_t rootParameterIndex)
    {
        mList->SetComputeRootUnorderedAccessView(rootParameterIndex, resource.D3DPtr()->GetGPUVirtualAddress());
    }

    void ComputeCommandListBase::SetComputeRootUnorderedAccessResource(const ColorTextureResource& resource, uint32_t rootParameterIndex) 
    {
        mList->SetComputeRootUnorderedAccessView(rootParameterIndex, resource.D3DPtr()->GetGPUVirtualAddress());
    }

    void ComputeCommandListBase::SetComputeRootUnorderedAccessResource(const DepthStencilTextureResource& resource, uint32_t rootParameterIndex) 
    { 
        mList->SetComputeRootUnorderedAccessView(rootParameterIndex, resource.D3DPtr()->GetGPUVirtualAddress());
    }

    void ComputeCommandListBase::SetDescriptorHeap(const DescriptorHeap& heap)
    {
        auto ptr = heap.D3DPtr();
        mList->SetDescriptorHeaps(1, (ID3D12DescriptorHeap* const*)&ptr);
    }

    void ComputeCommandListBase::SetPipelineState(const ComputePipelineState& state)
    {
        mList->SetPipelineState(state.D3DCompiledState());
    }

    void ComputeCommandListBase::SetComputeRootSignature(const RootSignature& signature)
    {
        mList->SetComputeRootSignature(signature.D3DSignature());
    }



    void DirectCommandListBase::SetViewport(const Viewport& viewport)
    {
        auto d3dViewport = viewport.D3DViewport();
        mList->RSSetViewports(1, &d3dViewport);
        D3D12_RECT scissorRect{ viewport.X, viewport.Y, viewport.Width, viewport.Height };
        mList->RSSetScissorRects(1, &scissorRect);
    }

    void DirectCommandListBase::SetRenderTarget(const RTDescriptor& rtDescriptor, std::optional<const DSDescriptor> depthStencilDescriptor)
    {
        const D3D12_CPU_DESCRIPTOR_HANDLE* dsHandle = depthStencilDescriptor.has_value() ? &depthStencilDescriptor->CPUHandle() : nullptr;
        mList->OMSetRenderTargets(1, &rtDescriptor.CPUHandle(), false, dsHandle);
    }

    void DirectCommandListBase::ClearRenderTarget(const RTDescriptor& rtDescriptor, const Foundation::Color& color)
    {
        mList->ClearRenderTargetView(rtDescriptor.CPUHandle(), color.Ptr(), 0, nullptr);
    }

    void DirectCommandListBase::CleadDepthStencil(const DSDescriptor& dsDescriptor, float depthValue)
    {
        mList->ClearDepthStencilView(dsDescriptor.CPUHandle(), D3D12_CLEAR_FLAG_DEPTH, depthValue, 0, 0, nullptr);
    }

    void DirectCommandListBase::SetFence(const Fence& fence)
    {
        
    }

    void DirectCommandListBase::SetVertexBuffer(const VertexBufferDescriptor& descriptor)
    {
        mList->IASetVertexBuffers(0, 1, &descriptor.D3DDescriptor());
    }
   
    void DirectCommandListBase::SetIndexBuffer(const IndexBufferDescriptor& descriptor)
    {
        mList->IASetIndexBuffer(&descriptor.D3DDescriptor());
    }

    void DirectCommandListBase::SetPrimitiveTopology(PrimitiveTopology topology)
    {
        mList->IASetPrimitiveTopology(D3DPrimitiveTopology(topology));
    }

    void DirectCommandListBase::SetPipelineState(const PipelineState& state)
    {
        mList->SetPipelineState(state.D3DCompiledState());
    }

    void DirectCommandListBase::SetGraphicsRootSignature(const RootSignature& signature)
    {
        mList->SetGraphicsRootSignature(signature.D3DSignature());
    }
    


    CopyCommandList::CopyCommandList(const Device& device, const CopyCommandAllocator& allocator)
        : CopyCommandListBase(device, allocator, D3D12_COMMAND_LIST_TYPE_COPY) {}



    ComputeCommandList::ComputeCommandList(const Device& device, const ComputeCommandAllocator& allocator)
        : ComputeCommandListBase(device, allocator, D3D12_COMMAND_LIST_TYPE_COMPUTE) {}



    BundleCommandList::BundleCommandList(const Device& device, const BundleCommandAllocator& allocator)
        : DirectCommandListBase(device, allocator, D3D12_COMMAND_LIST_TYPE_BUNDLE) {}



    DirectCommandList::DirectCommandList(const Device& device, const DirectCommandAllocator& allocator)
        : DirectCommandListBase(device, allocator, D3D12_COMMAND_LIST_TYPE_DIRECT) {}

    void DirectCommandList::ExecuteBundle(const BundleCommandList& bundle)
    {

    }

    void DirectCommandList::Draw(uint32_t vertexCount, uint32_t vertexStart)
    {
        mList->DrawInstanced(vertexCount, 1, vertexStart, 0);
    }

    void DirectCommandList::DrawInstanced(uint32_t vertexCount, uint32_t vertexStart, uint32_t instanceCount)
    {
        mList->DrawInstanced(vertexCount, instanceCount, vertexStart, 1);
    }

    void DirectCommandList::DrawIndexed(uint32_t vertexStart, uint32_t indexCount, uint32_t indexStart)
    {
        mList->DrawIndexedInstanced(indexCount, 1, indexStart, vertexStart, 0);
    }

    void DirectCommandList::DrawIndexedInstanced(uint32_t vertexStart, uint32_t indexCount, uint32_t indexStart, uint32_t instanceCount)
    {
        mList->DrawIndexedInstanced(indexCount, instanceCount, indexStart, vertexStart, 1);
    }

}