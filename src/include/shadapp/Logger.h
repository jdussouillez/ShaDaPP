#ifndef LOGGER_H
#define LOGGER_H

#include <cstdarg>
#include <cstdlib>
#include <map>
#include <string>

namespace shadapp {

    class Logger {
    public:

        enum class Level : short {
            ALL = 0,
            DEBUG,
            INFO,
            SUCCESS,
            WARN,
            ERROR,
            FATAL,
            OFF
        };

        enum class Color : short {
            NORMAL = -1,
            BLACK,
            RED,
            GREEN,
            YELLOW,
            BLUE,
            MAGENTA,
            CYAN,
            WHITE
        };

        static void setLevel(Level level);
        static void enableColors(bool colors);

        static void debug(std::string s);
        static void debug(const char* format, ...);
        
        static void info(std::string s);
        static void info(const char* format, ...);
        
        static void success(std::string s);
        static void success(const char* format, ...);
        
        static void warn(std::string s, std::exception* ex = nullptr);
        static void warn(const char* format, ...);
        
        static void error(std::string s, std::exception* ex = nullptr);
        static void error(const char* format, ...);
        
        static void fatal(std::string s, std::exception* ex = nullptr);
        static void fatal(const char* format, ...);

    private:
        static Level level;
        static bool colors;
        static std::map<Level, Color> levelColors;

        static void log(Level level, std::string s, std::exception* ex = nullptr);
        static void log(Level level, const char* format, va_list arglist);
        static std::string level2String(Level level);
        static std::string color2String(Color color);
        static std::string color(Level level, std::string s);
    };
}

#endif	// LOGGER_H
