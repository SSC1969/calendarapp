#pragma once
#include "gui/app_panel.h"
#include "gui/tasklist.h"
#include <wx/wx.h>

class TodoPanel : public AppPanel {
    using AppPanel::AppPanel;

  private:
    // Setup methods
    void createControls() override;
    void setUpSizers() override;
    void bindEventHandlers() override;

    // Event handlers

    // Other private methods

    // Control members
    wxStaticText *headline;
    wxTextCtrl *task_input;
    wxButton *create_task_button;
    TaskListPanel *task_panel;
};
