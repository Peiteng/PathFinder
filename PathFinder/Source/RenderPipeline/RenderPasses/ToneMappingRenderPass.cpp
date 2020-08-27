#include "ToneMappingRenderPass.hpp"

#include "../Foundation/Gaussian.hpp"

namespace PathFinder
{

    ToneMappingRenderPass::ToneMappingRenderPass()
        : RenderPass("ToneMapping") {}

    void ToneMappingRenderPass::SetupPipelineStates(PipelineStateCreator* stateCreator, RootSignatureCreator* rootSignatureCreator)
    {
        stateCreator->CreateComputeState(PSONames::ToneMapping, [](ComputeStateProxy& state)
        {
            state.ComputeShaderFileName = "ToneMapping.hlsl";
        });
    }

    void ToneMappingRenderPass::ScheduleResources(ResourceScheduler* scheduler)
    {
        //scheduler->ReadTexture(ResourceNames::BloomCompositionOutput);
        scheduler->NewTexture(ResourceNames::ToneMappingOutput);
        scheduler->ReadTexture(ResourceNames::StochasticShadowedShadingOutput);
        scheduler->ReadTexture(ResourceNames::StochasticShadowedShadingDenoisedStabilized);
        scheduler->ReadTexture(ResourceNames::StochasticShadingGradientFiltered);
        //scheduler->ReadTexture(ResourceNames::StochasticShadingGradient);
    }
     
    void ToneMappingRenderPass::Render(RenderContext<RenderPassContentMediator>* context)
    {
        context->GetCommandRecorder()->ApplyPipelineState(PSONames::ToneMapping);

        ToneMappingCBContent cbContent{};
        cbContent.InputTexIdx = context->GetResourceProvider()->GetSRTextureIndex(ResourceNames::StochasticShadingGradientFiltered);
        //cbContent._Pad0 = context->GetResourceProvider()->GetSRTextureIndex(ResourceNames::StochasticShadingGradient);
        //cbContent._Pad1 = context->GetResourceProvider()->GetSRTextureIndex(ResourceNames::StochasticShadingGradientNormFactor, 10);
        cbContent.OutputTexIdx = context->GetResourceProvider()->GetUATextureIndex(ResourceNames::ToneMappingOutput);
        cbContent.TonemappingParams = context->GetContent()->GetScene()->TonemappingParams();

        context->GetConstantsUpdater()->UpdateRootConstantBuffer(cbContent);

        auto dimensions = context->GetDefaultRenderSurfaceDesc().DispatchDimensionsForGroupSize(16, 16);
        context->GetCommandRecorder()->Dispatch(dimensions.x, dimensions.y);
    }

}
