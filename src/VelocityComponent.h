#pragma once

#include "JuceHeader.h"

class VelocityComponent : public Component
{
public:
    VelocityComponent();
    ~VelocityComponent();

    void paint (Graphics& g) override;
    void resized() override;

private:
    OwnedArray<TextButton> toggles;
    OwnedArray<Slider> sliders;
};
