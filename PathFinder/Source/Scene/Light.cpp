#include "Light.hpp"

#include "../Foundation/Pi.hpp"

namespace PathFinder
{

    Light::~Light() {}

    void Light::SetGPULightTableIndex(uint32_t index)
    {
        mGPULightTableIndex = index;
    }

    void Light::SetColor(const Foundation::Color& color)
    {
        mColor = color;
    }

    void Light::SetColorTemperature(Kelvin temperature)
    {
        // Convert to color here
    }

    void Light::SetLuminousPower(Lumen luminousPower)
    {
        mLuminousPower = luminousPower;
        mLuminousIntensity = mLuminousPower / mArea;
        mLuminance = mLuminousIntensity / M_PI;

        // Luminance due to a point on a Lambertian emitter, emitted in any direction, 
        // is equal to its total luminous power Phi divided by the emitter area A and the projected solid angle (Pi)
    }

    void Light::SetArea(float area)
    {
        mArea = area;
        // Recalculate due to changes in the area
        SetLuminousPower(mLuminousPower);
    }

}
