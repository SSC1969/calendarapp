#include "app/database.h"
#include "app/constants.h"
#include <filesystem>
#include <memory>
#include <string>
#include <wx/stdpaths.h>
#include <wx/wx.h>

CalenderDatabase::CalenderDatabase() {
    std::string data_path =
        wxStandardPaths::Get().GetUserDataDir().ToStdString();
    std::string db_path = data_path + "/" + constants::DB_NAME;

    // Verify that the data path exists
    if (!std::filesystem::exists(data_path)) {
        std::filesystem::create_directory(data_path);
    }

    this->storage = std::make_unique<Storage>(initStorage(db_path));
    this->storage->sync_schema();
}
