#include "PlaygroundRenderPass.hpp"

namespace PathFinder
{

    PlaygroundRenderPass::PlaygroundRenderPass()
        : RenderPass("Playground") {}

    void PlaygroundRenderPass::SetupPipelineStates(IShaderManager* shaderManager, IPipelineStateManager* psoManager)
    {
        auto pso = psoManager->CloneDefaultGraphicsState();
        pso.SetShaders(shaderManager->LoadShaders("Playground.hlsl", "Playground.hlsl")); 
        pso.SetInputAssemblerLayout(InputAssemblerLayoutForVertexLayout(VertexLayout::Layout1P1N1UV1T1BT));
        pso.SetDepthStencilFormat(HAL::ResourceFormat::Depth24_Float_Stencil8_Unsigned);
        pso.SetRenderTargetFormats(HAL::ResourceFormat::Color::RGBA8_Usigned_Norm);
        pso.SetPrimitiveTopology(HAL::PrimitiveTopology::TriangleList);
        psoManager->StoreGraphicsState(PSONames::GBuffer, pso);
    }

    void PlaygroundRenderPass::ScheduleResources(IResourceScheduler* scheduler)
    {
        scheduler->WillRenderToDepthStencil(ResourceNames::MainDepthStencil);
    }

    void PlaygroundRenderPass::Render(RenderContext* context)
    {
        context->GraphicsDevice()->ApplyPipelineState(PSONames::GBuffer);
        context->GraphicsDevice()->SetBackBufferAsRenderTarget(ResourceNames::MainDepthStencil);
        context->GraphicsDevice()->ClearBackBuffer(Foundation::Color::Green());
        context->GraphicsDevice()->UseVertexBufferOfLayout(VertexLayout::Layout1P1N1UV1T1BT);

        context->World()->IterateMeshInstances([&](const MeshInstance& instance)
        {
            context->World()->IterateSubMeshes(*instance.AssosiatedMesh(), [&](const SubMesh& subMesh)
            {
                context->GraphicsDevice()->Draw(subMesh.LocationInVertexStorage());
            });
        });
    }

}
