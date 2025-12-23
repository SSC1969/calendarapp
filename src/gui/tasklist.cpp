#include "gui/tasklist.h"
#include "app/database.h"
#include "app/task.h"
#include <sqlite_orm/sqlite_orm.h>
#include <sstream>
#include <vector>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/gtk/textctrl.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

namespace sql = sqlite_orm;

TaskListPanel::TaskListPanel(wxWindow *parent)
    : wxScrolledWindow(parent, wxID_ANY) {
    createControls();
    setUpSizers();
    bindEventHandlers();

    loadDayTasks(std::chrono::system_clock::now());
}

void TaskListPanel::createControls() {
    SetBackgroundColour(wxColour("purple"));
    SetScrollRate(0, FromDIP(10));

    sizer = new wxBoxSizer(wxVERTICAL);
    placeholder_text = new wxStaticText(this, wxID_ANY, "No tasks.");
    context_menu = new wxMenu();
    context_menu->Append(wxID_ANY, "Test");

    database = CalenderDatabase();
}

void TaskListPanel::setUpSizers() {
    sizer->Add(placeholder_text, wxSizerFlags().Center());

    SetSizerAndFit(sizer);
}

void TaskListPanel::bindEventHandlers() {}

void TaskListPanel::loadTasks(std::vector<Task> tasks) {
    for (Task &task : tasks) {
        TaskPanel *panel = new TaskPanel(this, task);
        panel->Bind(wxEVT_CONTEXT_MENU, &TaskListPanel::onContextMenu,
                    this);
        sizer->Add(panel, this->todo_flags);
    }

    if (tasks.empty()) {
        return;
    }

    placeholder_text->Hide();
    sizer->Layout();
    sizer->SetSizeHints(this);
}

void TaskListPanel::loadDayTasks(
    std::chrono::system_clock::time_point day) {

    std::string date_str = std::format("{:%F}", day);
    std::cout << "Loading tasks for " << date_str << "\n";

    std::vector<Task> tasks = database.storage->get_all<Task>(
        sql::where(date_str == sql::date(&Task::getStart) or
                   (date_str > sql::date(&Task::getStart) and
                    not sql::c(&Task::getCompleted))));

    loadTasks(tasks);
}

void TaskListPanel::onContextMenu(wxContextMenuEvent &evt) {
    wxLogStatus("Context menu opened");

    PopupMenu(context_menu);
}

void TaskListPanel::updateDatabase(Task task) {
    wxLogStatus("Updating database");
    database.storage->update(task);
}

TaskPanel::TaskPanel(TaskListPanel *parent, Task task)
    : wxPanel(parent, wxID_ANY) {
    this->parent = parent;
    createControls();
    setTask(task);

    setUpSizers();
    bindEventHandlers();
}

void TaskPanel::createControls() {
    SetBackgroundColour(wxColour("orange"));

    name = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                          wxDefaultSize, wxTE_PROCESS_ENTER);
    point_panel = new wxPanel(this);
    checkbox = new wxCheckBox(this, wxID_ANY, "");
}

void TaskPanel::setUpSizers() {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(name);
    sizer->Add(point_panel, 1);
    sizer->Add(checkbox, wxSizerFlags().CenterVertical());

    SetSizerAndFit(sizer);
}

void TaskPanel::bindEventHandlers() {
    name->Bind(wxEVT_TEXT_ENTER, &TaskPanel::onNameEntered, this);
    name->Bind(wxEVT_KILL_FOCUS, &TaskPanel::onNameFocusLost, this);
}

void TaskPanel::onNameEntered(wxCommandEvent &evt) { updateTaskName(); }

void TaskPanel::onNameFocusLost(wxFocusEvent &evt) {
    evt.Skip();
    updateTaskName();
}

void TaskPanel::setTask(Task new_task) {
    task = new_task;
    name->SetValue(task.name);

    checkbox->SetValue(task.getCompleted());
}

void TaskPanel::updateTaskName() {
    std::ostringstream ss;
    ss << "Updating task " << task;
    wxLogStatus(ss.str());

    task.name = name->GetValue().ToStdString();
    parent->updateDatabase(task);
}
