#pragma once
#include "app/task.h"
#include <wx/wx.h>

class TaskListPanel : public wxScrolledWindow {
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
    wxStaticText *placeholder_text;
};

class TaskPanel : public wxPanel {
  public:
    TaskPanel(wxWindow *parent, Task task);
    void setTask(Task new_task);

  private:
    // Setup methods
    void createControls();
    void setUpSizers();
    void bindEventHandlers();

    // Event handlers

    // Other private methods

    Task task;

    // Control members
    wxStaticText *name;
    wxPanel *point_panel;

    wxCheckBox *checkbox;
};
