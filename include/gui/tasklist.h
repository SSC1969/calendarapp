#pragma once
#include "task.h"
#include <wx/wx.h>

class TaskListPanel : public wxScrolled<wxPanel> {
  public:
    TaskListPanel(wxWindow *parent);

  private:
    // Setup methods
    void createControls();
    void setUpSizers();
    void bindEventHandlers();

    // Event handlers

    // Other private methods

    // Control members
};

class TaskPanel : public wxPanel {
  public:
    TaskPanel(wxWindow *parent, Task task = Task(""));
    void setTask(Task new_task);

  private:
    // Setup methods
    void createControls();
    void setUpSizers();
    void bindEventHandlers();

    // Event handlers

    // Other private methods

    Task task = Task("undefined");

    // Control members
    wxStaticText *name;
    wxPanel *point_panel;

    wxCheckBox *checkbox;
};
