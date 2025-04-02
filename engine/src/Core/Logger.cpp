#include <Core/Logger.h>

#include <Core/Engine.h>
#include <Core/Settings.h>

#include <chrono>
#include <format>
#include <print>

namespace core {

namespace
{
constexpr std::string_view ConsoleResetColor = "\033[0m";
constexpr std::string_view ConsoleGreen      = "\033[32m";
constexpr std::string_view ConsoleCyan       = "\033[36m";
constexpr std::string_view ConsoleYellow     = "\033[33m";
constexpr std::string_view ConsoleRed        = "\033[31m";
}

Logger::Logger():
    m_targets(TargetAll),
    m_level(LevelAll)
{
    if (m_targets & TargetFile) {
        std::lock_guard<std::mutex> lock (m_mutex);
        openLogFile();
    }
}

void Logger::openLogFile()
{
    if (!m_logFile.is_open()) {
        const auto logPath = Settings::getInstance().getLogFilePath();
        if (auto parentDir = logPath.parent_path(); !parentDir.empty() && !std::filesystem::exists(parentDir)) {
            std::filesystem::create_directories(parentDir);
        }
        if (std::filesystem::exists(logPath)) {
            std::filesystem::remove(logPath);
        }
        m_logFile.open(logPath, std::ios::app);
        if (!m_logFile) {
            std::print (stderr, "Failed to opent log file: {}\n", logPath.string());
        }
    }
}

void Logger::closeLogFile()
{
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void Logger::setOutputTargets (OutputTarget mask)
{
    std::lock_guard<std::mutex> lock (m_mutex);
    bool was_file_enabled     = m_targets & TargetFile;
    bool will_file_be_enabled = mask & TargetFile;

    if (!was_file_enabled && will_file_be_enabled) {
        openLogFile();
    }
    else if (was_file_enabled && !will_file_be_enabled) {
        closeLogFile();
    }

    m_targets = mask;
}

void Logger::setLogLevel (LogLevel mask)
{
    std::lock_guard<std::mutex> lock (m_mutex);
    m_level = mask;
}

void Logger::debug (const std::string& message)
{
    if (m_level & LevelDebug) {
        log ("DEBUG", ConsoleGreen, message);
    }
}

void Logger::info (const std::string& message)
{
    if (m_level & LevelInfo) {
        log ("INFO", ConsoleCyan, message);
    }
}

void Logger::warning (const std::string& message)
{
    if (m_level & LevelWarning) {
        log ("WARNING", ConsoleYellow, message);
    }
}

void Logger::error (const std::string& message)
{
    if (m_level & LevelError) {
        log ("ERROR", ConsoleRed, message);
    }
}

void Logger::log (std::string messagePrefix, std::string_view consoleColor, const std::string& message)
{
    std::lock_guard<std::mutex> lock (m_mutex);

    auto time = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());

    if (m_targets & TargetConsole) {
        std::print ("[{:%F %T}] {}[{}]{}: {}\n", time, consoleColor, messagePrefix, ConsoleResetColor, message);
    }
    if (m_targets & TargetFile) {
        m_logFile << std::format ("[{:%F %T}] [{}]: {}\n", time, messagePrefix, message) << std::flush;
    }
}


}

void core::log::info (const std::string& message)
{
    Engine::getInstance().logger.info (message);
}

void core::log::warning (const std::string& message)
{
    Engine::getInstance().logger.warning (message);
}

void core::log::error (const std::string& message)
{
    Engine::getInstance().logger.error (message);
}


