#include "gui/main_frame.h"
#include "gui/calender.h"
#include "gui/todo.h"
#include <wx/sizer.h>
#include <wx/wx.h>

MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title) {

    createControls();
    setUpSizers();
    bindEventHandlers();
}

void MainFrame::createControls() {
    SetBackgroundColour(wxColour("green"));
    wxFont main_font(wxFontInfo(wxSize(0, 16)));

    panel = new wxPanel(this);
    panel->SetFont(main_font);

    todo_panel = new TodoPanel(panel);
    todo_panel->init();
    calender_panel = new CalenderPanel(panel);
    calender_panel->init();
}

void MainFrame::setUpSizers() {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(todo_panel,
               wxSizerFlags().DoubleBorder(wxALL).Expand().Proportion(1));
    sizer->Add(calender_panel,
               wxSizerFlags().DoubleBorder(wxALL).Expand().Proportion(3));

    panel->SetSizer(sizer);
    sizer->SetSizeHints(this);
}

void MainFrame::bindEventHandlers() {}
