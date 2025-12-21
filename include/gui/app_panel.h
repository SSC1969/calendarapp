#pragma once
#include <wx/wx.h>

class AppPanel : public wxPanel {
  public:
    AppPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY) {};

    void init();

  private:
    // Setup methods
    virtual void createControls();
    virtual void setUpSizers();
    virtual void bindEventHandlers();

    // Event handlers

    // Other private methods

    // Control members
};
