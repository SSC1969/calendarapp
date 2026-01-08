#pragma once
#include <string>
#include <wx/calctrl.h>
#include <wx/combo.h>
#include <wx/datetime.h>
#include <wx/spinbutt.h>
#include <wx/wx.h>

// Code heavily based on
// https://forums.wxwidgets.org/viewtopic.php?p=186665#p186665 Thanks, PB!
class wxCustomDateRangePicker;
class wxCustomTimePicker;

class wxCustomDateRangePickerPopup : public wxPanel, public wxComboPopup {
  public:
    bool Create(wxWindow *parent) wxOVERRIDE;

    wxWindow *GetControl() wxOVERRIDE { return this; }
    wxString GetStringValue() const wxOVERRIDE;
    wxSize GetAdjustedSize(int minWidth, int prefHeight,
                           int maxHeight) wxOVERRIDE;

    void OnPopup() wxOVERRIDE;
    void OnDismiss() wxOVERRIDE;

    wxDateTime getStartDateTime() const;
    wxDateTime getEndDateTime() const;

  private:
    // controls and sizers
    wxBoxSizer *main_sizer;
    wxBoxSizer *top_sizer;
    wxBoxSizer *bottom_sizer;

    wxStaticText *start_text;
    wxStaticText *end_text;
    wxButton *end_button;

    wxCalendarCtrl *calender;

    wxCustomTimePicker *timepicker;
    wxButton *ok_button;

    // private members
    wxDateTime start_dt;
    wxDateTime end_dt;
    std::string selection;

    wxDateTime getWidgetDateTime() const;

    wxCustomDateRangePicker *getCustomDateRangePicker();

    void setStartDateTime(const wxDateTime &date_time);
    void setEndDateTime(const wxDateTime &date_time);
    void setWidgetDateTime(const wxDateTime &date_time);

    void setSelection(std::string selection);
    void updateSelection();
    void refreshLayout();
};

class wxCustomDateRangePicker : public wxComboCtrl {
  public:
    wxCustomDateRangePicker(wxWindow *parent);

    wxDateTime getStartDateTime() const { return start_dt; }
    wxDateTime getEndDateTime() const { return end_dt; }

    void setStartDateTime(const wxDateTime &date_time);
    void setEndDateTime(const wxDateTime &date_time);

    void updateText();

    // class methods for uniform datetime/range to string formatting
    static wxString getDisplayDateTimeString(const wxDateTime &date_time);
    static wxString getDisplayDateRangeString(const wxDateTime &start,
                                              const wxDateTime &end);

  private:
    wxCustomDateRangePickerPopup *popup;

    wxDateTime start_dt;
    wxDateTime end_dt;
};

class wxCustomTimePicker : public wxPanel {
  public:
    wxCustomTimePicker(wxWindow *parent);

    wxDateTime GetTime() const;
    void SetTime(const wxDateTime &date_time);

  private:
    wxTextCtrl *textbox;
    wxSpinButton *button;

    wxDateTime time;

    void UpdateText();
};

class wxTimeValidator : public wxTextValidator {
  public:
    wxTimeValidator();

    virtual bool Validate(wxWindow *WXUNUSED(parent)) wxOVERRIDE;
};
