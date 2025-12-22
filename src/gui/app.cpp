#include "gui/app.h"
#include "gui/main_frame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    MainFrame *mainFrame = new MainFrame("Calender");
    mainFrame->SetClientSize(800, 600);
    mainFrame->Center();
    mainFrame->Show();

    SetAppName("calenderapp");
    SetAppDisplayName("Calender App");
    return true;
}
