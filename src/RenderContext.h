#pragma once

#include "JuceHeader.h"

namespace vcp {

struct RenderContext
{
    int keyStart                = 36;   // C2
    int keyEnd                  = 60;   // C4
    int keyStride               = 4;

    String baseName             = "Sample";
    int noteLength              = 3000;
    int tailLength              = 1000;

    bool layerEnabled[4]        { true, false, false, false };
    int layerVelocities[4]      { 127, 96, 64, 32 };

    int loopMode                = 0;
    int loopStart               = 500;
    int loopEnd                 = 2500;
    int crossfadeLength         = 0;

    String instrumentName       = String();
    String outputPath           = String();
    
    ValueTree createValueTree()
    {
        ValueTree versicap ("versicap");
        versicap.setProperty ("keyStart",   keyStart, nullptr)
                .setProperty ("keyEnd",     keyEnd, nullptr)
                .setProperty ("keyStride",  keyStride, nullptr)
                .setProperty ("baseName",   baseName, nullptr)
                .setProperty ("noteLength", noteLength, nullptr)
                .setProperty ("tailLength", tailLength, nullptr)
                .setProperty ("loopMode",   loopMode, nullptr)
                .setProperty ("loopStart",  loopStart, nullptr)
                .setProperty ("crossfadeLength", crossfadeLength, nullptr)
                .setProperty ("instrumentName", instrumentName, nullptr)
                .setProperty ("outputPath", outputPath, nullptr);
        
        auto layers = versicap.getOrCreateChildWithName ("layers", nullptr);
        for (int i = 0; i < 4; ++i)
        {
            ValueTree layer ("layer");
            layer.setProperty ("enabled", layerEnabled [i], nullptr)
                 .setProperty ("velocity", layerVelocities [i], nullptr);
            layers.appendChild (layer, nullptr);
        }

        return versicap;
    }

    MidiMessageSequence* createMidiMessageSequence (const int layer, const double sampleRate) const
    {
        jassert (sampleRate > 0.0);
        jassert (isPositiveAndBelow (layer, 4));
        jassert (keyStride > 0);

        std::unique_ptr<MidiMessageSequence> seq;
        seq.reset (new MidiMessageSequence());

        int key = keyStart;
        int64 frame = 0.0;
        const int64 noteFrames = static_cast<int64> (sampleRate * ((double) noteLength / 1000.0));
        const int64 tailFrames = static_cast<int64> (sampleRate * ((double) tailLength / 1000.0));
        
        while (key <= keyEnd)
        {
            const auto velocity = static_cast<uint8> (layerVelocities [layer]);
            auto noteOn  = MidiMessage::noteOn (1, key, velocity);
            noteOn.setTimeStamp (static_cast<double> (frame));
            frame += noteFrames;
            
            auto noteOff = MidiMessage::noteOff (1, key);
            noteOff.setTimeStamp (static_cast<double> (frame));
            frame += tailFrames;
            
            seq->addEvent (noteOn);
            seq->addEvent (noteOff);
            key += keyStride;
        }
        
        return seq.release();
    }
};

}
