#include "gui/tasklist.h"
#include "app/database.h"
#include "app/task.h"
#include "gui/widgets/daterangepicker.h"
#include <iostream>
#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <wx/datectrl.h>
#include <wx/datetime.h>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/gtk/stattext.h>
#include <wx/gtk/textctrl.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/timectrl.h>
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
    SetScrollRate(0, FromDIP(10));

    sizer = new wxBoxSizer(wxVERTICAL);
    placeholder_text = new wxStaticText(this, wxID_ANY, "No tasks.");
    context_menu = new wxMenu();
    context_menu->Append(coAdd, "&Create Task");
    context_menu->Append(coEdit, "&Edit Task");
    context_menu->Append(coDelete, "&Delete Task");

    database = CalenderDatabase();
}

void TaskListPanel::setUpSizers() {
    sizer->Add(placeholder_text, wxSizerFlags().Center());

    SetSizer(sizer);
}

void TaskListPanel::bindEventHandlers() {}

void TaskListPanel::loadTasks(std::vector<Task> tasks) {
    for (Task &task : tasks) {
        TaskPanel *panel = new TaskPanel(this, task);
        sizer->Add(panel, this->todo_flags);
    }

    if (tasks.empty()) {
        return;
    }

    placeholder_text->Hide();
    Layout();
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

void TaskListPanel::addTask(Task task) {
    wxLogStatus("Adding task");
    auto inserted_id = database.storage->insert(task);
    task.setId(inserted_id);

    loadTasks({task});
}

void TaskListPanel::updateDatabase(Task task) {
    wxLogStatus("Updating database");
    database.storage->update(task);
}

// Deletes a task when passed it's wxWidgets ID, which should be
// wxID_HIGHEST + the id of that panel's task
void TaskListPanel::deleteTask(int id) {
    // Remove task from database
    database.storage->remove<Task>(id - wxID_HIGHEST);

    // Remove task panel from window and recalculate size
    if (wxWindow *task_panel = FindWindowById(id, this)) {
        task_panel->Destroy();
    }
    Layout();
}

TaskPanel::TaskPanel(TaskListPanel *parent, Task task)
    : wxPanel(parent, (wxID_HIGHEST + task.getId())) {
    this->parent = parent;
    createControls();
    setTask(task);

    setUpSizers();
    bindEventHandlers();
}

void TaskPanel::createControls() {
    checkbox = new wxCheckBox(this, wxID_ANY, "");
    name = new wxStaticText(this, wxID_ANY, wxEmptyString);
    name_entry =
        new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                       wxDefaultSize, wxTE_PROCESS_ENTER);

    duration = new wxCustomDateRangePicker(this);
}

void TaskPanel::setUpSizers() {
    sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(checkbox, wxSizerFlags().Center().Border(wxRIGHT));
    sizer->Add(name, wxSizerFlags(1).Center());
    sizer->Add(name_entry, wxSizerFlags(1).Center());

    sizer->Add(duration, wxSizerFlags().Center());

    sizer->Hide(name_entry);
    SetSizer(sizer);
    Layout();
}

void TaskPanel::bindEventHandlers() {
    Bind(wxEVT_CONTEXT_MENU, &TaskPanel::onContextMenu, this);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &TaskPanel::onMenuEvent, this);

    name->Bind(wxEVT_LEFT_DOWN, &TaskPanel::onNameTextSelected, this);
    name_entry->Bind(wxEVT_TEXT_ENTER, &TaskPanel::onNameEnterPressed,
                     this);
    name_entry->Bind(wxEVT_KILL_FOCUS, &TaskPanel::onNameFocusLost, this);
    checkbox->Bind(wxEVT_CHECKBOX, &TaskPanel::onCheckboxChanged, this);
    duration->Bind(wxEVT_COMBOBOX_CLOSEUP, &TaskPanel::onTimeChanged,
                   this);
}

void TaskPanel::onNameTextSelected(wxMouseEvent &evt) {
    evt.Skip();
    enableNameEdit();
}

void TaskPanel::onNameEnterPressed(wxCommandEvent &evt) {
    updateTaskName();
}

void TaskPanel::onNameFocusLost(wxFocusEvent &evt) {
    evt.Skip();
    updateTaskName();
}

void TaskPanel::onCheckboxChanged(wxCommandEvent &evt) {
    wxLogStatus("Updating task completeness");

    task.setCompleted(checkbox->GetValue());
    parent->updateDatabase(task);
}

void TaskPanel::onTimeChanged(wxCommandEvent &evt) {
    wxLogStatus("Updating task date to %s", duration->GetValue());

    // Set both start and end date
    wxDateTime s_dt = duration->getStartDateTime();
    task.setStartDateTime(s_dt);

    wxDateTime e_dt = duration->getEndDateTime();

    task.setEndDateTime(e_dt);
    parent->updateDatabase(task);

    Layout();
}

void TaskPanel::setTask(Task new_task) {
    task = new_task;
    name->SetLabel(task.name);
    name_entry->SetValue(task.name);

    duration->setStartDateTime(task.getStartDateTime());
    duration->setEndDateTime(task.getEndDateTime());

    checkbox->SetValue(task.getCompleted());
}

void TaskPanel::updateTaskName() {
    wxLogStatus("Updating task name");

    std::string new_name = name_entry->GetValue().ToStdString();
    name->SetLabel(new_name);
    sizer->Hide(name_entry);
    sizer->Show(name);
    Layout();

    task.name = new_name;
    parent->updateDatabase(task);
}

void TaskPanel::enableNameEdit() {
    sizer->Hide(name);
    sizer->Show(name_entry);
    Layout();
}

void TaskPanel::onContextMenu(wxContextMenuEvent &evt) {
    wxLogStatus("Context menu opened");

    PopupMenu(parent->context_menu);
}

void TaskPanel::onMenuEvent(wxCommandEvent &evt) {
    switch (evt.GetId()) {
    case coAdd:
        // TODO: Pop up task creation dialog
        break;
    case coEdit:
        // TODO: Pop up task panel after it's been implemented
        break;
    case coDelete:
        // Get the ID of the panel we opened the menu on
        parent->deleteTask(GetId());
        break;
    default:
        break;
    }
}
