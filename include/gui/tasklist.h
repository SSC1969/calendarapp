#pragma once
#include "app/database.h"
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
    void loadDayTasks(std::chrono::system_clock::time_point day);

    // Control members
    wxStaticText *placeholder_text;
    wxBoxSizer *sizer;
    CalenderDatabase database;

    wxSizerFlags todo_flags = wxSizerFlags().Expand().DoubleHorzBorder();
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
