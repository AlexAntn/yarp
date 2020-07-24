/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_OS_LOG_H
#define YARP_OS_LOG_H

#include <yarp/os/api.h>

#include <cstdint>
#include <iosfwd>
#include <mutex>

#if defined(__GNUC__)
#    define __YFUNCTION__ __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#    define __YFUNCTION__ __FUNCSIG__
#elif (__cplusplus <= 199711)
#    define __YFUNCTION__ __func__
#else
#    define __YFUNCTION__ "(unknown function)"
#endif // __GNUC__

// check arguments of the c-style debug functions to make sure that the
// arguments supplied have types appropriate to the format string
// specified, and that the conversions specified in the format string
// make sense. On gcc the warning is enabled by -Wformat.
#if defined(__GNUC__)
#    define YARP_ATTRIBUTE_FORMAT(style, fmt, args) __attribute__((format(printf, (fmt), (args))))
#else
#    define YARP_ATTRIBUTE_FORMAT(style, fmt, args)
#endif


// Forward declarations
namespace yarp {
namespace os {

class LogComponent;
class LogStream;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace impl {
class LogPrivate;
} // namespace impl
#endif // DOXYGEN_SHOULD_SKIP_THIS

class YARP_os_API Log
{
public:
    using Predicate = bool(*)();

    Log(const char* file,
        const unsigned int line,
        const char* func,
        const Predicate pred = nullptr,
        const LogComponent& comp = defaultLogComponent());

    // constructor with customtime
    Log(const char* file,
        const unsigned int line,
        const char* func,
        const double customtime,
        const Predicate pred = nullptr,
        const LogComponent& comp = defaultLogComponent());

    Log();
    virtual ~Log();

    enum LogType : uint8_t
    {
        LogTypeUnknown = 0,
        TraceType,
        DebugType,
        InfoType,
        WarningType,
        ErrorType,
        FatalType,
        LogTypeReserved = 0xFF
    };

    void trace(const char* msg, ...) const YARP_ATTRIBUTE_FORMAT(printf, 2, 3);
    void debug(const char* msg, ...) const YARP_ATTRIBUTE_FORMAT(printf, 2, 3);
    void info(const char* msg, ...) const YARP_ATTRIBUTE_FORMAT(printf, 2, 3);
    void warning(const char* msg, ...) const YARP_ATTRIBUTE_FORMAT(printf, 2, 3);
    void error(const char* msg, ...) const YARP_ATTRIBUTE_FORMAT(printf, 2, 3);
    YARP_NORETURN void fatal(const char* msg, ...) const YARP_ATTRIBUTE_FORMAT(printf, 2, 3);

    LogStream trace() const;
    LogStream debug() const;
    LogStream info() const;
    LogStream warning() const;
    LogStream error() const;
    LogStream fatal() const;

    using LogCallback = void (*)(yarp::os::Log::LogType type,
                                 const char* msg,
                                 const char* file,
                                 const unsigned int line,
                                 const char* func,
                                 double systemtime,
                                 double networktime,
                                 double customtime,
                                 const char* comp_name);

#ifndef YARP_NO_DEPRECATED // Since YARP 3.4
    YARP_DEPRECATED_MSG("Use setPrintCallback instead")
    static void setLogCallback(LogCallback);
#endif // YARP_NO_DEPRECATED

    static void setMinimumPrintLevel(LogType level); //!< Set current minimum print level
    static LogType minimumPrintLevel();              //!< Get current minimum print level
    static LogType defaultMinimumPrintLevel();       //!< Get default minimum print level

    static void setMinimumForwardLevel(LogType level); //!< Set current minimum forward level (it does nothing if forwarding is not enabled)
    static LogType minimumForwardLevel();              //!< Get current minimum forward level (or LogTypeReserved if forwarding is not enabled)
    static LogType defaultMinimumForwardLevel();       //!< Get default minimum forward level (or LogTypeReserved if forwarding is not enabled)

    static void setPrintCallback(LogCallback); //!< Set current print callback
    static LogCallback printCallback();        //!< Get current print callback
    static LogCallback defaultPrintCallback(); //!< Get default print callback

    static void setForwardCallback(LogCallback); //!< Set current forward callback (it does nothing if forwarding is not enabled)
    static LogCallback forwardCallback();        //!< Get current forward callback (or nullptr if forwarding is not enabled)
    static LogCallback defaultForwardCallback(); //!< Get default forward callback (or nullptr if forwarding is not enabled)


#ifndef DOXYGEN_SHOULD_SKIP_THIS
    static void nolog(const char* msg, ...) {}
    struct NoLog
    {
        template <typename T>
        NoLog& operator<<(const T&)
        {
            return *this;
        }
    };
    static NoLog nolog() { return NoLog(); }

private:
    yarp::os::impl::LogPrivate* const mPriv;

    friend class yarp::os::LogStream;

    // This callback is called by LogStream
    static void do_log(yarp::os::Log::LogType type,
                       const char* msg,
                       const char* file,
                       const unsigned int line,
                       const char* func,
                       double systemtime,
                       double networktime,
                       double customtime,
                       const LogComponent& comp_name);

    // This component is used for yDebug-family output, and is called by LogStream
    static const LogComponent& defaultLogComponent();

    // This component is used for internal debug output, and is called by LogStream
    static const LogComponent& logInternalComponent();
#endif // DOXYGEN_SHOULD_SKIP_THIS
}; // class Log

} // namespace os
} // namespace yarp




#define YARP_ONCE_CALLBACK                               \
    [](){                                                \
        static std::atomic_flag flag = ATOMIC_FLAG_INIT; \
        return !flag.test_and_set();                     \
    }

#define YARP_THREADONCE_CALLBACK                               \
    [](){                                                      \
        thread_local std::atomic_flag flag = ATOMIC_FLAG_INIT; \
        return !flag.test_and_set();                           \
    }

#define YARP_THROTTLE_CALLBACK(period)                   \
    [](){                                                \
        static double last = -period;                    \
        static std::mutex mutex;                         \
        std::lock_guard<std::mutex> lock(mutex);         \
        double now = yarp::os::SystemClock::nowSystem(); \
        if (now >= last + period) {                      \
            last = now;                                  \
            return true;                                 \
        }                                                \
        return false;                                    \
    }

#define YARP_THREADTHROTTLE_CALLBACK(period)             \
    [](){                                                \
        thread_local double last = -period;              \
        double now = yarp::os::SystemClock::nowSystem(); \
        if (now >= last + period) {                      \
            last = now;                                  \
            return true;                                 \
        }                                                \
        return false;                                    \
    }



#ifndef NDEBUG
#  define yTrace(...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__).trace(__VA_ARGS__)
#  define yTraceOnce(...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_ONCE_CALLBACK).trace(__VA_ARGS__)
#  define yTraceThreadOnce(...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADONCE_CALLBACK).trace(__VA_ARGS__)
#  define yTraceThrottle(period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THROTTLE_CALLBACK(period)).trace(__VA_ARGS__)
#  define yTraceThreadThrottle(period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADTHROTTLE_CALLBACK(period)).trace(__VA_ARGS__)
#  define yTraceCustomTime(customtime, ...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime).trace(__VA_ARGS__)
#  define yTraceCustomTimeOnce(customtime, ...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_ONCE_CALLBACK).trace(__VA_ARGS__)
#  define yTraceCustomTimeThreadOnce(customtime, ...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THREADONCE_CALLBACK).trace(__VA_ARGS__)
#  define yTraceCustomTimeThrottle(customtime, period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THROTTLE_CALLBACK(period)).trace(__VA_ARGS__)
#  define yTraceCustomTimeThreadThrottle(customtime, period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THREADTHROTTLE_CALLBACK(period)).trace(__VA_ARGS__)
#else
#  define yTrace(...)                       yarp::os::Log::nolog(__VA_ARGS__)
#  define yTraceOnce(...)                   yarp::os::Log::nolog(__VA_ARGS__)
#  define yTraceThreadOnce(...)             yarp::os::Log::nolog(__VA_ARGS__)
#  define yTraceThrottle(period, ...)       yarp::os::Log::nolog(__VA_ARGS__)
#  define yTraceThreadThrottle(period, ...) yarp::os::Log::nolog(__VA_ARGS__)
#  define yTraceCustomTime(customtime, ...)                       yarp::os::Log::nolog(__VA_ARGS__)
#  define yTraceCustomTimeOnce(customtime, ...)                   yarp::os::Log::nolog(__VA_ARGS__)
#  define yTraceCustomTimeThreadOnce(customtime, ...)             yarp::os::Log::nolog(__VA_ARGS__)
#  define yTraceCustomTimeThrottle(customtime, period, ...)       yarp::os::Log::nolog(__VA_ARGS__)
#  define yTraceCustomTimeThreadThrottle(customtime, period, ...) yarp::os::Log::nolog(__VA_ARGS__)
#endif

#ifndef YARP_NO_DEBUG_OUTPUT
#  define yDebug(...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__).debug(__VA_ARGS__)
#  define yDebugOnce(...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_ONCE_CALLBACK).debug(__VA_ARGS__)
#  define yDebugThreadOnce(...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADONCE_CALLBACK).debug(__VA_ARGS__)
#  define yDebugThrottle(period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THROTTLE_CALLBACK(period)).debug(__VA_ARGS__)
#  define yDebugThreadThrottle(period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADTHROTTLE_CALLBACK(period)).debug(__VA_ARGS__)
#  define yDebugCustomTime(customtime, ...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime).debug(__VA_ARGS__)
#  define yDebugCustomTimeOnce(customtime, ...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_ONCE_CALLBACK).debug(__VA_ARGS__)
#  define yDebugCustomTimeThreadOnce(customtime, ...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THREADONCE_CALLBACK).debug(__VA_ARGS__)
#  define yDebugCustomTimeThrottle(customtime, period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THROTTLE_CALLBACK(period)).debug(__VA_ARGS__)
#  define yDebugCustomTimeThreadThrottle(customtime, period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THREADTHROTTLE_CALLBACK(period)).debug(__VA_ARGS__)
#else
#  define yDebug(...)                       yarp::os::Log::nolog(__VA_ARGS__)
#  define yDebugOnce(...)                   yarp::os::Log::nolog(__VA_ARGS__)
#  define yDebugThreadOnce(...)             yarp::os::Log::nolog(__VA_ARGS__)
#  define yDebugThrottle(period, ...)       yarp::os::Log::nolog(__VA_ARGS__)
#  define yDebugThreadThrottle(period, ...) yarp::os::Log::nolog(__VA_ARGS__)
#  define yDebugCustomTime(customtime, ...)                       yarp::os::Log::nolog(__VA_ARGS__)
#  define yDebugCustomTimeOnce(customtime, ...)                   yarp::os::Log::nolog(__VA_ARGS__)
#  define yDebugCustomTimeThreadOnce(customtime, ...)             yarp::os::Log::nolog(__VA_ARGS__)
#  define yDebugCustomTimeThrottle(customtime, period, ...)       yarp::os::Log::nolog(__VA_ARGS__)
#  define yDebugCustomTimeThreadThrottle(customtime, period, ...) yarp::os::Log::nolog(__VA_ARGS__)
#endif

#define yInfo(...)                          yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__).info(__VA_ARGS__)
#define yInfoOnce(...)                      yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_ONCE_CALLBACK).info(__VA_ARGS__)
#define yInfoThreadOnce(...)                yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADONCE_CALLBACK).info(__VA_ARGS__)
#define yInfoThrottle(period, ...)          yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THROTTLE_CALLBACK(period)).info(__VA_ARGS__)
#define yInfoThreadThrottle(period, ...)    yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADTHROTTLE_CALLBACK(period)).info(__VA_ARGS__)
#define yInfoCustomTime(customtime, ...)                          yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime).info(__VA_ARGS__)
#define yInfoCustomTimeOnce(customtime, ...)                      yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_ONCE_CALLBACK).info(__VA_ARGS__)
#define yInfoCustomTimeThreadOnce(customtime, ...)                yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THREADONCE_CALLBACK).info(__VA_ARGS__)
#define yInfoCustomTimeThrottle(customtime, period, ...)          yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THROTTLE_CALLBACK(period)).info(__VA_ARGS__)
#define yInfoCustomTimeThreadThrottle(customtime, period, ...)    yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THREADTHROTTLE_CALLBACK(period)).info(__VA_ARGS__)

#define yWarning(...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__).warning(__VA_ARGS__)
#define yWarningOnce(...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_ONCE_CALLBACK).warning(__VA_ARGS__)
#define yWarningThreadOnce(...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADONCE_CALLBACK).warning(__VA_ARGS__)
#define yWarningThrottle(period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THROTTLE_CALLBACK(period)).warning(__VA_ARGS__)
#define yWarningThreadThrottle(period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADTHROTTLE_CALLBACK(period)).warning(__VA_ARGS__)
#define yWarningCustomTime(customtime, ...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime).warning(__VA_ARGS__)
#define yWarningCustomTimeOnce(customtime, ...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_ONCE_CALLBACK).warning(__VA_ARGS__)
#define yWarningCustomTimeThreadOnce(customtime, ...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THREADONCE_CALLBACK).warning(__VA_ARGS__)
#define yWarningCustomTimeThrottle(customtime, period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THROTTLE_CALLBACK(period)).warning(__VA_ARGS__)
#define yWarningCustomTimeThreadThrottle(customtime, period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THREADTHROTTLE_CALLBACK(period)).warning(__VA_ARGS__)

#define yError(...)                         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__).error(__VA_ARGS__)
#define yErrorOnce(...)                     yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_ONCE_CALLBACK).error(__VA_ARGS__)
#define yErrorThreadOnce(...)               yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADONCE_CALLBACK).error(__VA_ARGS__)
#define yErrorThrottle(period, ...)         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THROTTLE_CALLBACK(period)).error(__VA_ARGS__)
#define yErrorThreadThrottle(period, ...)   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADTHROTTLE_CALLBACK(period)).error(__VA_ARGS__)
#define yErrorCustomTime(customtime, ...)                         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime).error(__VA_ARGS__)
#define yErrorCustomTimeOnce(customtime, ...)                     yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_ONCE_CALLBACK).error(__VA_ARGS__)
#define yErrorCustomTimeThreadOnce(customtime, ...)               yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THREADONCE_CALLBACK).error(__VA_ARGS__)
#define yErrorCustomTimeThrottle(customtime, period, ...)         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THROTTLE_CALLBACK(period)).error(__VA_ARGS__)
#define yErrorCustomTimeThreadThrottle(customtime, period, ...)   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime, YARP_THREADTHROTTLE_CALLBACK(period)).error(__VA_ARGS__)

#define yFatal(...)                         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__).fatal(__VA_ARGS__)
#define yFatalCustomTime(customtime, ...)                         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customtime).fatal(__VA_ARGS__)





#ifndef NDEBUG
#  define yAssert(x)                                                       \
    if (!(x)) {                                                            \
        yFatal("Assertion failure at %s:%d (%s)", __FILE__, __LINE__, #x); \
    }
#else
#  define yAssert(x)
#endif

#define YARP_FIXME_NOTIMPLEMENTED(what) yWarning("FIXME: %s not yet implemented", what);


/**
 * Low level function for printing a stack trace, if implemented (ACE or gcc/Linux).
 */
YARP_os_API void yarp_print_trace(FILE* out, const char* file, unsigned int line);


#endif // YARP_OS_LOG_H
