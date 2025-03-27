#pragma once
#include <concepts>
#include <fstream>
#include <mutex>

namespace core {

class Logger
{
public:
    enum OutputTarget : uint8_t
    {
        TargetNone    = 0,
        TargetConsole = 1 << 0,
        TargetFile    = 1 << 1,
        TargetAll     = TargetConsole | TargetFile,
    };

    enum LogLevel : uint8_t
    {
        LevelNone    = 0,
        LevelDebug   = 1 << 0,
        LevelInfo    = 1 << 1,
        LevelWarning = 1 << 2,
        LevelError   = 1 << 3,
        LevelAll     = LevelDebug | LevelInfo | LevelWarning | LevelError,
    };

    Logger();

    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

    ~Logger() = default;

    void setOutputTargets(OutputTarget mask);
    void setLogLevel(LogLevel mask);

    void debug (const std::string& message);
    void info (const std::string& message);
    void warning (const std::string& message);
    void error (const std::string& message);

private:
    void openLogFile();
    void closeLogFile();
    void log (std::string messagePrefix, std::string_view consoleColor, const std::string& message);

private:
    OutputTarget       m_targets;
    LogLevel           m_level;
    std::ofstream      m_logFile;
    mutable std::mutex m_mutex;
};

namespace log {

void info(const std::string& message);
void warning(const std::string& message);
void error(const std::string& message);

}

} // namespace debug
