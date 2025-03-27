#pragma once
#include <string>
#include <filesystem>

namespace core
{

class Settings
{
public:
    static Settings& getInstance();

    Settings (const Settings&) = delete;
    Settings& operator= (const Settings&) = delete;

    std::filesystem::path getLogFilePath() const;

    void setLogFilePath (const std::string& path);

private:
    Settings();

private:
    std::filesystem::path m_logPath; 
};
}
