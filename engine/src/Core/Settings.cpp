#include <Core/Settings.h>

namespace core {
Settings::Settings():
    m_logPath ("./log.txt")
{}

Settings& Settings::getInstance()
{
    static Settings settings;
    return settings;
}

std::filesystem::path Settings::getLogFilePath() const
{
    return m_logPath;
}

void Settings::setLogFilePath (const std::string& path)
{
    m_logPath = std::filesystem::path (path);
}

} // namespace core
