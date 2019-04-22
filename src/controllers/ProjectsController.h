#pragma once

#include "controllers/Controller.h"

namespace vcp {

class ProjectDocument;

class ProjectsController : public Controller,
                           private ChangeListener
{
public:
    ProjectsController (Versicap& vc)
        : Controller (vc) { }
    ~ProjectsController() = default;

    String getName() const override { return "Projects"; }
    
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo&) override;
    bool perform (const ApplicationCommandTarget::InvocationInfo&) override;

    void initialize() override;
    void shutdown() override;

private:
    std::unique_ptr<ProjectDocument> document;
    void save (bool saveAs = false);
    void changeListenerCallback (ChangeBroadcaster*) override;
};

}
