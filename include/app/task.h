#pragma once
#include <iostream>
#include <string>
#include <wx/datetime.h>

class Task {
  public:
    std::string name;
    std::optional<std::string> description;

    int getId() const { return this->id; }
    void setId(int id) { this->id = id; }

    std::string getStart() const {
        return start_dt.Format("%F %R").ToStdString();
    }

    wxDateTime getStartDateTime() const { return start_dt; }

    void setStart(std::string start) { start_dt.ParseDateTime(start); }
    void setStartDateTime(wxDateTime &datetime) { start_dt = datetime; }

    std::string getEnd() const {
        return end_dt.Format("%F %R").ToStdString();
    }
    wxDateTime getEndDateTime() const { return end_dt; }

    void setEnd(std::string end) { end_dt.ParseDateTime(end); }
    void setEndDateTime(wxDateTime &datetime) { end_dt = datetime; }

    bool getCompleted() const { return this->completed; }
    void setCompleted(bool completed) { this->completed = completed; }

    friend std::ostream &operator<<(std::ostream &os, const Task &t) {
        return os << "[" << t.id << "](" << t.name << ", "
                  << t.description.value_or("none") << ", "
                  << t.start_dt.Format() << ", " << t.end_dt.Format()
                  << ", " << t.completed << ")";
    }

  private:
    wxDateTime start_dt = wxDateTime::Now();
    wxDateTime end_dt = start_dt;

    bool completed = false;

    int id = -1;
};
