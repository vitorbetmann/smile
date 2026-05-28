// Includes ————————————————————————————————————————————————————————————————————————————————————————

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Log.h"
#include "LogInternal.h"

#include "internal/Common/Common.h"

// Prototypes ——————————————————————————————————————————————————————————————————————————————————————

static int lgPrivateLog(lgInternalLevel lvl, const char *origin, const char *msg, ...);

static int lgPrivateLogV(lgInternalLevel lvl, const char *ori, const char *msg, va_list args);

static bool lgPrivateIsLevelEnabled(lgInternalLevel lvl);

static void lgPrivateGetColorAndPrefix(lgInternalLevel lvl, const char **color,
                                       const char **prefix);

static void lgPrivateFatalHandler(void);

// Variables ———————————————————————————————————————————————————————————————————————————————————————

// Constant

static const char SMILE_CYAN[] = "\033[36m";
static const char SMILE_YELLOW[] = "\033[33m";
static const char SMILE_RED[] = "\033[31m";
static const char SMILE_PURPLE[] = "\033[0;35m";
static const char SMILE_GREEN[] = "\033[32m";
static const char SMILE_WHITE[] = "\033[0m";

static const char LOG_TIME_FMT[] = "%H:%M:%S";

static const int LOG_TIME_BUFFER_LEN = 32;

// Mutable

static lgFatalHandler fatalHandler = lgPrivateFatalHandler;

// Functions - Public ——————————————————————————————————————————————————————————————————————————————

int lgLog(const char *msg, ...)
{
    if (!msg)
    {
        return RES_NULL_ARG;
    }

    va_list args;
    va_start(args, msg);
    int result = lgPrivateLogV(USER, "User", msg, args);
    va_end(args);
    return result;
}

int lgSetFatal(const lgFatalHandler handler)
{
    if (!handler)
    {
        fatalHandler = lgPrivateFatalHandler;
    }

    fatalHandler = handler;
    return RES_OK;
}

// Functions - Internal ————————————————————————————————————————————————————————————————————————————

int lgInternalLog(const lgInternalLevel lvl, const char *ori, const char *cse, const char *caller,
                  const char *csq)
{
    if (!ori || !cse || !caller || !csq)
    {
        return RES_NULL_ARG;
    }

    return lgPrivateLog(lvl, ori, "%s. '%s' %s.", cse, caller, csq);
}

int lgInternalLogWithArg(const lgInternalLevel lvl, const char *ori, const char *cse,
                         const char *arg, const char *caller, const char *csq)
{
    if (!ori || !cse || !arg || !caller || !csq)
    {
        return RES_NULL_ARG;
    }

    return lgPrivateLog(lvl, ori, "%s: %s. '%s' %s.", cse, arg, caller, csq);
}

// Functions - Private —————————————————————————————————————————————————————————————————————————————

static int lgPrivateLog(const lgInternalLevel lvl, const char *origin, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    int result = lgPrivateLogV(lvl, origin, msg, args);
    va_end(args);
    return result;
}

static int lgPrivateLogV(lgInternalLevel lvl, const char *ori, const char *msg, va_list args)
{
    if (!lgPrivateIsLevelEnabled(lvl))
    {
        return RES_OK;
    }

    const char *color = nullptr;
    const char *prefix = nullptr;
    lgPrivateGetColorAndPrefix(lvl, &color, &prefix);

    char timeBuf[LOG_TIME_BUFFER_LEN] = "00:00:00";
    const time_t epochTime = time(nullptr);
    struct tm localTime = {0};
    bool hasLocalTime = false;

    if (epochTime != (time_t)-1)
    {
#ifdef _WIN32
        hasLocalTime = localtime_s(&localTime, &epochTime) == 0;
#else
        hasLocalTime = localtime_r(&epochTime, &localTime) != nullptr;
#endif
    }
    if (!hasLocalTime)
    {
        if (lvl == FATAL)
        {
            fatalHandler();
        }
        return RES_TIME_FAIL;
    }

    if (strftime(timeBuf, sizeof(timeBuf), LOG_TIME_FMT, &localTime) == 0)
    {
        if (lvl == FATAL)
        {
            fatalHandler();
        }
        return RES_TIME_FAIL;
    }

    int prefixStatus = fprintf(stderr, "%s%s [%s %s] - ", color, timeBuf, ori, prefix);
    int messageStatus = vfprintf(stderr, msg, args);
    int suffixStatus = fprintf(stderr, "%s\n", SMILE_WHITE); // Reset color
    int flushStatus = 0;
    if (lvl == ERROR || lvl == FATAL)
    {
        flushStatus = fflush(stderr);
    }

    if (prefixStatus < 0 || messageStatus < 0 || suffixStatus < 0 ||
        flushStatus == EOF)
    {
        if (lvl == FATAL)
        {
            fatalHandler();
        }
        return RES_WRITE_FAIL;
    }

    if (lvl == FATAL)
    {
        fatalHandler();
    }
    return RES_OK;
}

static bool lgPrivateIsLevelEnabled(const lgInternalLevel lvl)
{
    switch (lvl)
    {
    case INFO:
#ifdef SMILE_INFO
        return true;
#else
        return false;
#endif
    case WARN:
#ifdef SMILE_WARN
        return true;
#else
        return false;
#endif
    default:
        return true;
    }
}

static void lgPrivateGetColorAndPrefix(const lgInternalLevel lvl, const char **color,
                                       const char **prefix)
{
    switch (lvl)
    {
    case USER:
        *color = SMILE_GREEN;
        *prefix = "LOG";
        return;
    case INFO:
        *color = SMILE_CYAN;
        *prefix = "INFO";
        return;
    case WARN:
        *color = SMILE_YELLOW;
        *prefix = "WARNING";
        return;
    case ERROR:
        *color = SMILE_RED;
        *prefix = "ERROR";
        return;
    case FATAL:
        *color = SMILE_PURPLE;
        *prefix = "FATAL";
        return;
    default:
        *color = SMILE_WHITE;
        *prefix = "\"You Shouldn't Be Seeing This!\"";
    }
}

static void lgPrivateFatalHandler(void)
{
    exit(EXIT_FAILURE);
}
