#include "gui/todo.h"
#include "app/task.h"
#include "gui/tasklist.h"
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/gtk/font.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/wx.h>

void TodoPanel::createControls() {
    wxFont title_font(wxFontInfo(wxSize(0, 24)).Bold());

    headline = new wxStaticText(this, wxID_ANY, "To-Do:");
    headline->SetFont(title_font);

    task_input =
        new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                       wxDefaultSize, wxTE_PROCESS_ENTER);

    create_task_button = new wxButton(this, wxID_ADD);
    task_panel = new TaskListPanel(this);
}

void TodoPanel::setUpSizers() {
    // Main vertical sizer containing all elements of the todo section
    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

    // Create a new horizontal BoxSizer to hold the input field and button
    wxBoxSizer *input_sizer = new wxBoxSizer(wxHORIZONTAL);
    input_sizer->Add(task_input, wxSizerFlags(1).Border(wxRIGHT));
    input_sizer->Add(create_task_button, wxSizerFlags(0).CenterVertical());

    // Add elements to the main sizer in the correct order
    main_sizer->Add(headline, wxSizerFlags().CenterHorizontal().Border());
    main_sizer->Add(input_sizer, wxSizerFlags().Expand().Border(wxBOTTOM));
    main_sizer->Add(task_panel, wxSizerFlags(1).Expand());

    // Make the TodoPanel use the main_sizer to control it's contents
    SetSizer(main_sizer);
}

void TodoPanel::bindEventHandlers() {
    create_task_button->Bind(wxEVT_BUTTON,
                             &TodoPanel::onCreateTaskButtonPressed, this);
    task_input->Bind(wxEVT_TEXT_ENTER, &TodoPanel::onInputEnterPressed,
                     this);
}

void TodoPanel::onCreateTaskButtonPressed(wxCommandEvent &evt) {
    addTaskFromInput();
}

void TodoPanel::onInputEnterPressed(wxCommandEvent &evt) {
    addTaskFromInput();
}

void TodoPanel::addTaskFromInput() {
    if (task_input->IsEmpty()) {
        return;
    }
    Task new_task;
    new_task.name = task_input->GetValue().ToStdString();
    task_input->Clear();

    task_panel->addTask(new_task);
}
