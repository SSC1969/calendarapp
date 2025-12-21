#include "gui/app_panel.h"
#include <iostream>
#include <wx/wx.h>

void AppPanel::init() {
    createControls();
    setUpSizers();
    bindEventHandlers();
}

void AppPanel::createControls() { std::cout << "Unimplemented!\n"; }

void AppPanel::setUpSizers() { std::cout << "Unimplemented!\n"; }

void AppPanel::bindEventHandlers() { std::cout << "Unimplemented!\n"; }
