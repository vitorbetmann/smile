#ifndef LOG_H
#define LOG_H

// --------------------------------------------------
// Data types
// --------------------------------------------------

typedef void (*lgFatalHandler)(void);

// --------------------------------------------------
// Prototypes
// --------------------------------------------------

void lgLog(const char *origin, const char *msg, ...);

void lgSetFatalHandler(lgFatalHandler handler);

#endif
