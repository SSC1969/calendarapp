#include "gui/todo.h"
#include "gui/tasklist.h"
#include <wx/gdicmn.h>
#include <wx/gtk/font.h>
#include <wx/sizer.h>
#include <wx/wx.h>

void TodoPanel::createControls() {
    wxFont headline_font(wxFontInfo(wxSize(0, 24)).Bold());
    SetBackgroundColour(wxColour("blue"));

    headline = new wxStaticText(this, wxID_ANY, "To-do:");
    headline->SetFont(headline_font);

    task_input = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition,
                                wxDefaultSize, wxTE_PROCESS_ENTER);

    create_task_button =
        new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxDefaultSize,
                     wxBU_AUTODRAW);

    task_panel = new TaskListPanel(this);
}

void TodoPanel::setUpSizers() {
    // Main vertical sizer containing all elements of the todo section
    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

    // Create a new horizontal BoxSizer to hold the input field and button
    wxBoxSizer *input_sizer = new wxBoxSizer(wxHORIZONTAL);
    input_sizer->Add(task_input, 4);
    input_sizer->Add(create_task_button, wxSizerFlags().CenterVertical());

    // Add elements to the main sizer in the correct order
    main_sizer->Add(headline, wxSizerFlags().CenterHorizontal());
    main_sizer->Add(input_sizer, wxSizerFlags().Expand());
    main_sizer->Add(task_panel, wxSizerFlags().Expand());

    // Make the TodoPanel use the main_sizer to control it's contents
    SetSizerAndFit(main_sizer);
}

void TodoPanel::bindEventHandlers() {}
