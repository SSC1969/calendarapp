#pragma once
#include "gui/app_panel.h"
#include <wx/wx.h>

class CalenderPanel : public AppPanel {
    using AppPanel::AppPanel;

  private:
    // Setup methods
    void createControls() override;
    void setUpSizers() override;
    void bindEventHandlers() override;

    // Event handlers

    // Other private methods

    // Control members
};
