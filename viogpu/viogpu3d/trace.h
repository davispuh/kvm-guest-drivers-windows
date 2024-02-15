#pragma once
#include "kdebugprint.h"

// #define DBG 1
#define EVENT_TRACING 1

#ifndef TRACE_LEVEL_INFORMATION
#define TRACE_LEVEL_NONE        0   // Tracing is not on
#define TRACE_LEVEL_FATAL       1   // Abnormal exit or termination
#define TRACE_LEVEL_ERROR       2   // Severe errors that need logging
#define TRACE_LEVEL_WARNING     3   // Warnings such as allocation failure
#define TRACE_LEVEL_INFORMATION 4   // Includes non-error cases(e.g.,Entry-Exit)
#define TRACE_LEVEL_VERBOSE     5   // Detailed traces from intermediate steps
#define TRACE_LEVEL_RESERVED6   6
#define TRACE_LEVEL_RESERVED7   7
#define TRACE_LEVEL_RESERVED8   8
#define TRACE_LEVEL_RESERVED9   9
#endif // TRACE_LEVEL_INFORMATION

#if defined(DBG)
#define PRINT_DEBUG 1
//#define COM_DEBUG 1

    extern int nDebugLevel;
    extern int bBreakAlways;

    void DebugPrintFuncSerial(const char *format, ...);
    void DebugPrintFuncKdPrint(const char *format, ...);
#define DbgExpandArguments(...) __VA_ARGS__
#define DbgPrint(Level, MSG) \
    if (Level <= nDebugLevel) VirtioDebugPrintProc(DbgExpandArguments MSG);
#define VioGpuDbgBreak()\
    if (KD_DEBUGGER_ENABLED && !KD_DEBUGGER_NOT_PRESENT && bBreakAlways) DbgBreakPoint();

#undef EVENT_TRACING // DBG build excludes WPP
#define WPP_INIT_TRACING(driver, regpath)  InitializeDebugPrints(driver, regpath);
#define WPP_CLEANUP(driver)
#else
//#define DbgPrint(level, line) {};

#define WPP_CHECK_FOR_NULL_STRING  // to prevent exceptions due to NULL strings.

// Tracing GUID - {88FF4AF9-B7B4-40FF-94AD-271B4777ED97}

#define WPP_CONTROL_GUIDS                                              \
    WPP_DEFINE_CONTROL_GUID(                                           \
        VioGpu3DTraceGuid, (88FF4AF9,B7B4,40FF,94AD,271B4777ED97),    \
        WPP_DEFINE_BIT(TRACE_ALL)                                      \
        )

#define WPP_FLAG_LEVEL_LOGGER(flag, level)     WPP_LEVEL_LOGGER(flag)

#define WPP_FLAG_LEVEL_ENABLED(flag, level)                            \
    (WPP_LEVEL_ENABLED(flag) &&                                        \
     WPP_CONTROL(WPP_BIT_ ## flag).Level >= level)

#define WPP_LEVEL_FLAGS_LOGGER(lvl,flags)      WPP_LEVEL_LOGGER(flags)

#define WPP_LEVEL_FLAGS_ENABLED(lvl, flags)                            \
    (WPP_LEVEL_ENABLED(flags) && WPP_CONTROL(WPP_BIT_ ## flags).Level >= lvl)

#define VioGpuDbgBreak() {}

// begin_wpp config
// USEPREFIX (DbgPrint, "%!STDPREFIX! %!FUNC!");
// DbgPrint{FLAG = TRACE_ALL}(LEVEL, (MSG, ...));
// end_wpp
//

#ifndef EVENT_TRACING

#define WPP_INIT_TRACING(driver, regpath)
#define WPP_CLEANUP(driver)
#define DbgPrint

#endif

#endif
