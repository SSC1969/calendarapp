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
}

void TaskListPanel::createControls() {
    SetBackgroundColour(wxColour("purple"));

    SetScrollRate(0, FromDIP(10));

    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    wxSizerFlags todo_flags = wxSizerFlags().Expand().DoubleHorzBorder();

    CalenderDatabase database = CalenderDatabase();

    std::string date_str =
        std::format("{:%F}", std::chrono::system_clock::now());

    std::vector<Task> tasks = database.storage->get_all<Task>(
        sql::where(date_str == sql::c(&Task::getStart)));

    for (Task &task : tasks) {
        TaskPanel *panel = new TaskPanel(this, task);
        sizer->Add(panel, todo_flags);
    }

    placeholder_text = new wxStaticText(this, wxID_ANY, "No tasks.");
    placeholder_text->Hide();

    sizer->Add(placeholder_text, wxSizerFlags().Center());

    if (tasks.empty()) {
        placeholder_text->Show();
    }

    SetSizerAndFit(sizer);
}

void TaskListPanel::setUpSizers() {}

void TaskListPanel::bindEventHandlers() {}

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
