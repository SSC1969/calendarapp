#include "gui/main_frame.h"
#include "gui/calender.h"
#include "gui/todo.h"
#include <wx/anybutton.h>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/wx.h>

MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title) {

    createControls();
    setUpSizers();
    bindEventHandlers();
}

void MainFrame::createControls() {
    logger = new wxLogWindow(this, "Log", true, false);
    wxLog::SetActiveTarget(logger);

    wxFont main_font(wxFontInfo(wxSize(0, 18)));

    panel = new wxPanel(this);
    // panel->SetFont(main_font);

    notebook = new wxNotebook(panel, wxID_ANY, wxDefaultPosition,
                              wxDefaultSize, wxNB_LEFT | wxNB_NOPAGETHEME);

    todo_border = new wxPanel(notebook);
    todo_panel = new TodoPanel(todo_border);
    todo_panel->init();

    calender_border = new wxPanel(notebook);
    calender_panel = new CalenderPanel(calender_border);
    calender_panel->init();

    CreateStatusBar();
}

void MainFrame::setUpSizers() {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(notebook, wxSizerFlags(1).Expand());

    wxBoxSizer *todo_sizer = new wxBoxSizer(wxHORIZONTAL);
    todo_sizer->Add(todo_panel, wxSizerFlags(1).Expand().Border());
    todo_border->SetSizer(todo_sizer);

    notebook->AddPage(todo_border, "To-Do");
    wxBoxSizer *calender_sizer = new wxBoxSizer(wxHORIZONTAL);
    calender_sizer->Add(calender_panel, wxSizerFlags(1).Expand().Border());
    calender_border->SetSizer(calender_sizer);

    notebook->AddPage(calender_border, "Calender");

    panel->SetSizer(sizer);
    sizer->SetSizeHints(this);
}

void MainFrame::bindEventHandlers() {}
