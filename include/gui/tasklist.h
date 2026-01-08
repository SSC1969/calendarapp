#pragma once
#include "app/database.h"
#include "app/task.h"
#include "gui/widgets/daterangepicker.h"
#include <chrono>
#include <wx/datectrl.h>
#include <wx/event.h>
#include <wx/timectrl.h>
#include <wx/wx.h>

enum ContextOptions { coAdd, coEdit, coDelete };

class TaskListPanel : public wxScrolledWindow {
  public:
    TaskListPanel(wxWindow *parent);

    void addTask(Task task);
    void updateDatabase(Task task);
    void deleteTask(int id);

    wxMenu *context_menu;

  private:
    // Setup methods
    void createControls();
    void setUpSizers();
    void bindEventHandlers();

    // Event handlers

    // Other private methods
    void loadTasks(std::vector<Task> tasks);
    void loadDayTasks(std::chrono::system_clock::time_point day);

    // Control members
    wxStaticText *placeholder_text;
    wxBoxSizer *sizer;

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
    void onContextMenu(wxContextMenuEvent &evt);
    void onMenuEvent(wxCommandEvent &evt);

    void onNameTextSelected(wxMouseEvent &evt);
    void onNameEnterPressed(wxCommandEvent &evt);
    void onNameFocusLost(wxFocusEvent &evt);
    void onCheckboxChanged(wxCommandEvent &evt);
    void onTimeChanged(wxCommandEvent &evt);

    // Other private methods
    void updateTaskName();
    void enableNameEdit();

    Task task;
    TaskListPanel *parent;

    // Control members
    wxBoxSizer *sizer;
    wxCheckBox *checkbox;
    wxStaticText *name;
    wxTextCtrl *name_entry;

    wxCustomDateRangePicker *duration;
};
