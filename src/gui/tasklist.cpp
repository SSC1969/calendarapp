#include "gui/tasklist.h"
#include "app/database.h"
#include "app/task.h"
#include <sqlite_orm/sqlite_orm.h>
#include <vector>
#include <wx/sizer.h>
#include <wx/string.h>
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

    database = CalenderDatabase();
}

void TaskListPanel::setUpSizers() {
    sizer->Add(placeholder_text, wxSizerFlags().Center());

    SetSizerAndFit(sizer);
}

void TaskListPanel::bindEventHandlers() {}

void TaskListPanel::loadDayTasks(
    std::chrono::system_clock::time_point day) {

    std::string date_str = std::format("{:%F}", day);

    std::vector<Task> tasks = database.storage->get_all<Task>(
        sql::where(date_str == sql::c(&Task::getStart) or
                   (date_str > sql::c(&Task::getStart) and
                    not sql::c(&Task::getCompleted))));

    for (Task &task : tasks) {
        TaskPanel *panel = new TaskPanel(this, task);
        sizer->Add(panel, this->todo_flags);
    }

    placeholder_text->Hide();
    sizer->Layout();
    sizer->SetSizeHints(this);
}

TaskPanel::TaskPanel(wxWindow *parent, Task task)
    : wxPanel(parent, wxID_ANY) {
    createControls();
    setUpSizers();
    bindEventHandlers();

    setTask(task);
}

void TaskPanel::createControls() {
    name = new wxStaticText(this, wxID_ANY, "noname");
    point_panel = new wxPanel(this);

    checkbox = new wxCheckBox(this, wxID_ANY, "");
}

void TaskPanel::setUpSizers() {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(name);
    sizer->Add(point_panel, 1);
    sizer->Add(checkbox);

    SetSizerAndFit(sizer);
}

void TaskPanel::bindEventHandlers() {}

void TaskPanel::setTask(Task new_task) {
    task = new_task;
    name->SetLabel(task.name);

    checkbox->SetValue(task.getCompleted());
}
