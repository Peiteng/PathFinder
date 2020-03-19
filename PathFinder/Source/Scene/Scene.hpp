#pragma once

#include "Mesh.hpp"
#include "MeshInstance.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "GTTonemappingParameters.hpp"
#include "BloomParameters.hpp"
#include "ResourceLoader.hpp"
#include "FlatLight.hpp"
#include "SphericalLight.hpp"

#include "../Memory/GPUResourceProducer.hpp"

#include <functional>
#include <list>
#include <memory>

namespace PathFinder 
{

    class Scene 
    {
    public:
        using FlatLightIt = std::list<FlatLight>::iterator;
        using SphericalLightIt = std::list<SphericalLight>::iterator;

        Mesh& AddMesh(Mesh&& mesh);
        MeshInstance& AddMeshInstance(MeshInstance&& instance);
        Material& AddMaterial(Material&& material);
        FlatLightIt EmplaceFlatLight(FlatLight::Type type);
        SphericalLightIt EmplaceSphericalLight();

    private:
        std::list<Mesh> mMeshes;
        std::list<MeshInstance> mMeshInstances;
        std::list<Material> mMaterials;
        std::list<FlatLight> mFlatLights;
        std::list<SphericalLight> mSphericalLights;

        Camera mCamera;
        GTTonemappingParameterss mTonemappingParams;
        BloomParameters mBloomParameters;

    public:
        inline Camera& MainCamera() { return mCamera; }
        inline const Camera& MainCamera() const { return mCamera; }
        inline const auto& Meshes() const { return mMeshes; }
        inline const auto& MeshInstances() const { return mMeshInstances; }
        inline const auto& Materials() const { return mMaterials; }
        inline const auto& FlatLights() const { return mFlatLights; }
        inline const auto& SphericalLights() const { return mSphericalLights; }
        inline const auto& TonemappingParams() const { return mTonemappingParams; }
        inline const auto& BloomParams() const { return mBloomParameters; }

        inline auto& Meshes() { return mMeshes; }
        inline auto& MeshInstances() { return mMeshInstances; }
        inline auto& Materials() { return mMaterials; }
        inline auto& FlatLights() { return mFlatLights; }
        inline auto& SphericalLights() { return mSphericalLights; }
        inline auto& TonemappingParams() { return mTonemappingParams; }
    };

}
