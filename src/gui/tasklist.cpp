#include "gui/tasklist.h"
#include "app/database.h"
#include "app/task.h"
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/wx.h>

TaskListPanel::TaskListPanel(wxWindow *parent)
    : wxScrolled<wxPanel>(parent, wxID_ANY) {
    createControls();
    setUpSizers();
    bindEventHandlers();
}

void TaskListPanel::createControls() {
    SetBackgroundColour(wxColour("purple"));

    SetScrollRate(0, FromDIP(10));

    // Dynamically create tasks later
    wxStaticBoxSizer *sizer = new wxStaticBoxSizer(wxVERTICAL, this);

    wxSizerFlags todo_flags = wxSizerFlags().Expand().DoubleHorzBorder();

    CalenderDatabase database = CalenderDatabase();

    for (int i = 0; i < 15; i++) {
        std::string task_name = std::format("Task {}", i);
        Task new_task = Task(task_name);
        database.addTask(new_task);
        TaskPanel *task_panel = new TaskPanel(sizer->GetStaticBox());
        task_panel->setTask(new_task);
        sizer->Add(task_panel, todo_flags);
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

    checkbox->SetValue(task.isComplete());
}
