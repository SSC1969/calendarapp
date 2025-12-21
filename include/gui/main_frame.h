#pragma once
#include "gui/calender.h"
#include "gui/todo.h"
#include <wx/wx.h>

class MainFrame : public wxFrame {
  public:
    MainFrame(const wxString &title);

  private:
    // Setup methods
    void createControls();
    void setUpSizers();
    void bindEventHandlers();

    // Event handlers

    // Other private methods

    // Control members
    wxPanel *panel;
    TodoPanel *todo_panel;
    CalenderPanel *calender_panel;
};
