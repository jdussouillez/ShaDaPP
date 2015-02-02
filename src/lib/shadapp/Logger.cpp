#include <ctime>
#include <iostream>
#include <typeinfo>
#include <sstream>

#include <shadapp/Logger.h>

namespace shadapp {

    Logger::Level Logger::level = Logger::Level::OFF;
    bool Logger::colors = false;
    std::map<Logger::Level, Logger::Color> Logger::levelColors = {
        {Logger::Level::DEBUG, Logger::Color::NORMAL},
        {Logger::Level::INFO, Logger::Color::BLUE},
        {Logger::Level::SUCCESS, Logger::Color::GREEN},
        {Logger::Level::WARN, Logger::Color::YELLOW},
        {Logger::Level::ERROR, Logger::Color::RED},
        {Logger::Level::FATAL, Logger::Color::RED},
    };

    void Logger::setLevel(Level level) {
        Logger::level = level;
    }

    void Logger::enableColors(bool colors) {
        Logger::colors = colors;
    }

    void Logger::log(Level level, std::string s, std::exception* ex) {
        if (level < Logger::level) {
            return;
        }
        time_t t;
        struct tm* timeInfo;
        char timeStr[20];
        time(&t);
        timeInfo = localtime(&t);
        strftime(timeStr, sizeof (timeStr), "%m/%d/%Y %H:%M:%S", timeInfo);
        std::ostringstream stream;
        stream << "[" << timeStr << "][" << level2String(level) << "]";
        if (ex != nullptr) {
            stream << "[EXCEPTION(" << ex->what() << ")]";
        }
        stream << " " << s << std::endl;
        std::cerr << (colors ? color(level, stream.str()) : stream.str());
    }
    
    void Logger::log(Level level, const char* format, va_list arglist) {
        if (level < Logger::level) {
            return;
        }
        time_t t;
        struct tm* timeInfo;
        char timeStr[20];
        time(&t);
        timeInfo = localtime(&t);
        strftime(timeStr, sizeof (timeStr), "%m/%d/%Y %H:%M:%S", timeInfo);
        std::ostringstream stream;
        stream << "[" << timeStr << "][" << level2String(level) << "]";
        char buf[256];
        vsnprintf(buf, 256, format, arglist);
        stream << " " << buf << std::endl;
        std::cerr << (colors ? color(level, stream.str()) : stream.str());
    }

    std::string Logger::level2String(Level level) {
        switch (level) {
            case Level::DEBUG:
                return "DEBUG";
            case Level::INFO:
                return "INFO";
            case Level::SUCCESS:
                return "SUCCESS";
            case Level::WARN:
                return "WARN";
            case Level::ERROR:
                return "ERROR";
            case Level::FATAL:
                return "FATAL";
            default:
                return "UNKNOWN";
        }
    }

    std::string Logger::color2String(Color color) {
        if (color == Color::NORMAL) {
            return "\e[m";
        }
        std::ostringstream stream;
        stream << "\e[3" << (short) color << "m";
        return stream.str();
    }

    std::string Logger::color(Level level, std::string s) {
        return color2String(levelColors[level]) + s + color2String(Color::NORMAL);
    }

    void Logger::debug(std::string s) {
        Logger::log(Level::DEBUG, s);
    }

    void Logger::debug(const char* format, ...) {
        std::va_list arglist;
        va_start(arglist, format);
        Logger::log(Level::DEBUG, format, arglist);
        va_end(arglist);
    }

    void Logger::info(std::string s) {
        Logger::log(Level::INFO, s);
    }
    
    void Logger::info(const char* format, ...) {
        std::va_list arglist;
        va_start(arglist, format);
        Logger::log(Level::INFO, format, arglist);
        va_end(arglist);
    }

    void Logger::success(std::string s) {
        Logger::log(Level::SUCCESS, s);
    }
    
    void Logger::success(const char* format, ...) {
        std::va_list arglist;
        va_start(arglist, format);
        Logger::log(Level::SUCCESS, format, arglist);
        va_end(arglist);
    }

    void Logger::warn(std::string s, std::exception* ex) {
        Logger::log(Level::WARN, s, ex);
    }
    
    void Logger::warn(const char* format, ...) {
        std::va_list arglist;
        va_start(arglist, format);
        Logger::log(Level::WARN, format, arglist);
        va_end(arglist);
    }

    void Logger::error(std::string s, std::exception* ex) {
        Logger::log(Level::ERROR, s, ex);
    }
    
    void Logger::error(const char* format, ...) {
        std::va_list arglist;
        va_start(arglist, format);
        Logger::log(Level::ERROR, format, arglist);
        va_end(arglist);
    }

    void Logger::fatal(std::string s, std::exception* ex) {
        Logger::log(Level::FATAL, s + " | Aborting...", ex);
        abort();
    }
    
    void Logger::fatal(const char* format, ...) {
        std::va_list arglist;
        va_start(arglist, format);
        Logger::log(Level::FATAL, format, arglist);
        va_end(arglist);
        abort();
    }
}
