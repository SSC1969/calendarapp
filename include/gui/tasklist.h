#pragma once
#include "app/database.h"
#include "app/task.h"
#include <wx/event.h>
#include <wx/wx.h>

class TaskListPanel : public wxScrolledWindow {
  public:
    TaskListPanel(wxWindow *parent);

    void updateDatabase(Task task);

  private:
    // Setup methods
    void createControls();
    void setUpSizers();
    void bindEventHandlers();

    // Event handlers
    void onContextMenu(wxContextMenuEvent &evt);

    // Other private methods
    void loadTasks(std::vector<Task> tasks);
    void loadDayTasks(std::chrono::system_clock::time_point day);

    // Control members
    wxStaticText *placeholder_text;
    wxBoxSizer *sizer;
    wxMenu *context_menu;

    CalenderDatabase database;
    wxSizerFlags todo_flags = wxSizerFlags().Expand();
};

class TaskPanel : public wxPanel {
  public:
    TaskPanel(TaskListPanel *parent, Task task);
    void setTask(Task new_task);

  private:
    // Setup methods
    void createControls();
    void setUpSizers();
    void bindEventHandlers();

    // Event handlers
    void onNameEntered(wxCommandEvent &evt);
    void onNameFocusLost(wxFocusEvent &evt);

    // Other private methods
    void updateTaskName();

    Task task;
    TaskListPanel *parent;

    // Control members
    wxTextCtrl *name;
    wxPanel *point_panel;

    wxCheckBox *checkbox;
};
