#include "gui/widgets/daterangepicker.h"
#include <regex>
#include <string>
#include <wx/calctrl.h>
#include <wx/combo.h>
#include <wx/datetime.h>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/settings.h>
#include <wx/spinbutt.h>
#include <wx/thread.h>
#include <wx/valtext.h>
#include <wx/wx.h>

bool wxCustomDateRangePickerPopup::Create(wxWindow *parent) {
    if (!wxPanel::Create(parent, wxID_ANY, wxDefaultPosition,
                         wxDefaultSize,
                         wxBORDER_RAISED | wxTAB_TRAVERSAL)) {
        return false;
    }

    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));

    // create controls and sizers
    main_sizer = new wxBoxSizer(wxVERTICAL);
    top_sizer = new wxBoxSizer(wxHORIZONTAL);
    bottom_sizer = new wxBoxSizer(wxHORIZONTAL);

    start_text =
        new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                         wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    end_text =
        new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                         wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    end_button = new wxButton(this, wxID_ADD);

    calender =
        new wxCalendarCtrl(this, wxID_ANY, wxDefaultDateTime,
                           wxDefaultPosition, wxDefaultSize, wxNO_BORDER);

    timepicker = new wxCustomTimePicker(this);
    ok_button = new wxButton(this, wxID_OK);

    // bind event handlers
    start_text->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &evt) {
        setSelection("start");
        evt.Skip();
    });
    end_text->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent &evt) {
        setSelection("end");
        evt.Skip();
    });
    end_button->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) {
        setEndDateTime(start_dt + wxTimeSpan(1));
        setSelection("end");
    });

    calender->Bind(wxEVT_CALENDAR_SEL_CHANGED,
                   [this](wxCalendarEvent &evt) {
                       updateSelection();
                       evt.Skip();
                   });
    // TODO
    // time_picker->Bind(wxEVT_CALENDAR_SEL_CHANGED,
    //                [this] { updateSelection(); });

    ok_button->Bind(wxEVT_BUTTON, [this](wxCommandEvent &) { Dismiss(); });

    // place objects in sizers
    wxSizerFlags flags =
        wxSizerFlags(1).Center().Border(wxALL, FromDIP(2));

    top_sizer->Add(start_text, flags);
    top_sizer->Add(end_text, flags);
    top_sizer->Hide(end_text);

    flags = wxSizerFlags(1).Expand().Border(wxALL, FromDIP(2));

    top_sizer->Add(end_button, flags.Proportion(0));

    main_sizer->Add(top_sizer, wxSizerFlags().Expand());
    main_sizer->Add(calender, flags);

    bottom_sizer->Add(timepicker, flags.Proportion(3));
    bottom_sizer->Add(ok_button, flags.Proportion(2));

    main_sizer->Add(bottom_sizer, wxSizerFlags().Expand());

    SetSizerAndFit(main_sizer);

    return true;
}

wxString wxCustomDateRangePickerPopup::GetStringValue() const {
    return wxCustomDateRangePicker::getDisplayDateRangeString(start_dt,
                                                              end_dt);
}
wxSize
wxCustomDateRangePickerPopup::GetAdjustedSize(int WXUNUSED(minWidth),
                                              int WXUNUSED(prefHeight),
                                              int WXUNUSED(maxHeight)) {
    return GetSize();
}

void wxCustomDateRangePickerPopup::OnPopup() {
    wxCustomDateRangePicker *picker = getCustomDateRangePicker();

    if (picker) {
        setStartDateTime(picker->getStartDateTime());
        setEndDateTime(picker->getEndDateTime());
        setSelection("start");
    }
}

void wxCustomDateRangePickerPopup::OnDismiss() {
    wxCustomDateRangePicker *picker = getCustomDateRangePicker();

    if (picker) {
        picker->setStartDateTime(getStartDateTime());
        picker->setEndDateTime(getEndDateTime());
    }
}

wxDateTime wxCustomDateRangePickerPopup::getStartDateTime() const {
    wxCHECK(IsCreated(), wxInvalidDateTime);

    return start_dt;
}
wxDateTime wxCustomDateRangePickerPopup::getEndDateTime() const {
    wxCHECK(IsCreated(), wxInvalidDateTime);

    return end_dt;
}
wxDateTime wxCustomDateRangePickerPopup::getWidgetDateTime() const {
    wxCHECK(IsCreated(), wxInvalidDateTime);

    wxDateTime date, time;

    date = calender->GetDate();
    wxCHECK(date.IsValid(), wxInvalidDateTime);

    // TODO
    time = timepicker->GetTime();
    wxCHECK(time.IsValid(), wxInvalidDateTime);

    return wxDateTime(date.GetDay(), date.GetMonth(), date.GetYear(),
                      time.GetHour(), time.GetMinute());
}

wxCustomDateRangePicker *
wxCustomDateRangePickerPopup::getCustomDateRangePicker() {
    wxCHECK(IsCreated(), NULL);

    return dynamic_cast<wxCustomDateRangePicker *>(GetComboCtrl());
}

void wxCustomDateRangePickerPopup::setStartDateTime(
    const wxDateTime &date_time) {
    wxCHECK_RET(IsCreated(), "Call Create() before setting the datetime!");
    wxLogMessage("Updating popup start datetime");

    start_dt = date_time;
    start_text->SetLabel(
        wxCustomDateRangePicker::getDisplayDateTimeString(start_dt));

    refreshLayout();
}

void wxCustomDateRangePickerPopup::setEndDateTime(
    const wxDateTime &date_time) {
    wxCHECK_RET(IsCreated(), "Call Create() before setting the datetime!");
    wxLogMessage("Updating popup end datetime");

    end_dt = date_time;
    end_text->SetLabel(
        wxCustomDateRangePicker::getDisplayDateTimeString(end_dt));

    refreshLayout();
}
void wxCustomDateRangePickerPopup::setWidgetDateTime(
    const wxDateTime &date_time) {
    wxCHECK_RET(IsCreated(), "Call Create() before setting the datetime!");

    calender->SetDate(date_time);
    timepicker->SetTime(date_time);
}

void wxCustomDateRangePickerPopup::setSelection(std::string selection) {
    if (this->selection == selection) {
        return;
    }

    wxLogMessage("Popup switching selection to %s", selection);

    if (selection == "start") {
        this->selection = selection;
        setWidgetDateTime(getStartDateTime());

        // unlimit date range
        calender->SetDateRange();

        // highlight selected text
        start_text->SetBackgroundColour(
            wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
        end_text->SetBackgroundColour(GetBackgroundColour());
    } else if (selection == "end") {
        this->selection = selection;
        setWidgetDateTime(getEndDateTime());

        // limit date range
        calender->SetDateRange(getStartDateTime() -
                               wxDateSpan(0, 0, 0, 1));

        // highlight selected text
        end_text->SetBackgroundColour(
            wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
        start_text->SetBackgroundColour(GetBackgroundColour());
    }
}

void wxCustomDateRangePickerPopup::updateSelection() {
    if (selection == "start") {
        setStartDateTime(getWidgetDateTime());
    } else if (selection == "end") {
        setEndDateTime(getWidgetDateTime());
    }
}

void wxCustomDateRangePickerPopup::refreshLayout() {
    wxLogMessage("Updating popup layout");

    if (start_dt.IsEqualTo(end_dt)) {
        top_sizer->Hide(end_text);
        top_sizer->Show(end_button);
    } else {
        top_sizer->Hide(end_button);
        top_sizer->Show(end_text);
    }

    // update layout
    top_sizer->Layout();
    main_sizer->Fit(this);

    // This line is required for the popup box to resize propery when
    // it's components change size
    GetParent()->SetSize(GetSize());
}

// wxCustomDateRangePicker definitions

wxCustomDateRangePicker::wxCustomDateRangePicker(wxWindow *parent)
    : wxComboCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition,
                  wxDefaultSize, wxCB_READONLY) {
    UseAltPopupWindow();
    popup = new wxCustomDateRangePickerPopup();
    SetPopupControl(popup);
}

void wxCustomDateRangePicker::setStartDateTime(
    const wxDateTime &date_time) {
    wxLogMessage("Updating range picker start datetime");
    start_dt = date_time;
    updateText();
}
void wxCustomDateRangePicker::setEndDateTime(const wxDateTime &date_time) {
    wxLogMessage("Updating range picker end datetime");
    end_dt = date_time;
    updateText();
}

void wxCustomDateRangePicker::updateText() {
    const wxString duration_str =
        wxCustomDateRangePicker::getDisplayDateRangeString(start_dt,
                                                           end_dt);

    SetValue(duration_str);
    SetMinClientSize(
        GetSizeFromText(wxString::Format(" %s ", duration_str)));
}

// class methods for uniform datetime/range to string formatting
wxString wxCustomDateRangePicker::getDisplayDateTimeString(
    const wxDateTime &date_time) {
    wxCHECK(date_time.IsValid(), wxString(_("Invalid date and/or time")));

    // default formatting is e.g. Jan 2, 12:05PM
    std::string fmt = "%b %e, %I:%M%p";

    bool is_different_year =
        date_time.GetYear() != wxDateTime::Now().GetYear();

    // display the year if it isn't this year
    if (is_different_year) {
        // e.g. Jan 2, 2025, 12:05PM
        fmt = "%b %e, %G, %I:%M%p";
    }

    return date_time.Format(fmt);
}
wxString
wxCustomDateRangePicker::getDisplayDateRangeString(const wxDateTime &start,
                                                   const wxDateTime &end) {
    wxCHECK(start.IsValid(),
            wxString(_("Invalid start date and/or time")));

    wxString start_str =
        wxCustomDateRangePicker::getDisplayDateTimeString(start);

    // if the task doesn't have a set length, just return the start string
    if (start.IsEqualTo(end) || !end.IsValid()) {
        return start_str;
    }

    wxString end_str =
        wxCustomDateRangePicker::getDisplayDateTimeString(end);

    // if the datetimes have different years, display the years
    if (start.GetYear() != end.GetYear()) {
        std::string fmt = "%b %e, %G, %I:%M%p";
        start_str = start.Format(fmt);
        end_str = end.Format(fmt);
    }

    // if the datetimes have the same date, only display one date and a
    // time range
    if (start.IsSameDate(end)) {
        end_str = end.Format("%I:%M%p");
    }

    return start_str + " - " + end_str;
}

wxCustomTimePicker::wxCustomTimePicker(wxWindow *parent)
    : wxPanel(parent) {

    wxTextValidator validator;
    validator.SetCharIncludes("1234567890:pPaAmM");

    textbox = new wxTextCtrl(this, wxID_ANY, "00:00PM", wxDefaultPosition,
                             wxDefaultSize, 0, validator);
    textbox->SetMaxLength(7);

    button = new wxSpinButton(this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(textbox);
    sizer->Add(button);

    SetSizerAndFit(sizer);
}

wxDateTime wxCustomTimePicker::GetTime() const { return time; }

void wxCustomTimePicker::SetTime(const wxDateTime &date_time) {
    time = date_time;
    UpdateText();
}

void wxCustomTimePicker::UpdateText() {
    wxString str = time.Format("%I:%M%p");

    textbox->SetValue(str);
}

bool wxTimeValidator::Validate(wxWindow *WXUNUSED(parent)) {
    wxASSERT(GetWindow()->IsKindOf(CLASSINFO(wxTextCtrl)));

    wxTextCtrl *textbox = (wxTextCtrl *)GetWindow();
    std::string str = textbox->GetValue().ToStdString();
    std::regex reg("/^(((0[1-9])|(1[0-2]))|[1-9]):([0-5]\\d)([paPA]\\.?["
                   "mM]\\.?)$/gm");

    return true;
}
