
#pragma once

#include "gui/MainComponent.h"
#include "gui/MainMenu.h"
#include "Versicap.h"

namespace vcp {

class MainWindow : public DocumentWindow
{
public:
    MainWindow (String name, Versicap& vc) 
        : DocumentWindow (name, Desktop::getInstance().getDefaultLookAndFeel()
                                    .findColour (ResizableWindow::backgroundColourId),
                                        DocumentWindow::allButtons),
            versicap (vc)
    {
        menu.reset (new MainMenu (*this, versicap.getCommandManager()));
        menu->setupMenu();

        setUsingNativeTitleBar (true);
        setBackgroundColour (kv::LookAndFeel_KV1::widgetBackgroundColor.darker());

        addKeyListener (versicap.getCommandManager().getKeyMappings());

        setContentOwned (new MainComponent (vc), true);

        setResizable (true, false);
        constrain.setMinimumSize (870, 460);
        // constrain.setMaximumSize (1280, 340);
        setConstrainer (&constrain);
        centreWithSize (1200, 600);

        if (auto* props = versicap.getSettings().getUserSettings())
        {
            const auto state = props->getValue ("windowPosition", String());
            if (state.isNotEmpty())
                restoreWindowStateFromString (state);
        }
        
        setVisible (true);
    }

    ~MainWindow()
    {
        setConstrainer (nullptr);
        #if ! JUCE_MAC
        setMenuBar (nullptr);
        #endif
        menu.reset();
    }

    void savePersistentData()
    {
        if (auto* props = versicap.getSettings().getUserSettings())
            props->setValue ("windowPosition", getWindowStateAsString());
        if (auto* const comp = dynamic_cast<MainComponent*> (getContentComponent()))
            comp->saveSettings();
    }

    void closeButtonPressed() override
    {
        JUCEApplication::getInstance()->systemRequestedQuit();
    }

    void maximiseButtonPressed() override
    {
        return;
    }

private:
    Versicap& versicap;
    ComponentBoundsConstrainer constrain;
    std::unique_ptr<MainMenu> menu;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

}
