/*
 * Copyright (C) 2006-2020 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_OS_LOGCOMPONENT_H
#define YARP_OS_LOGCOMPONENT_H

#include <yarp/os/api.h>
#include <yarp/os/Log.h>

#include <atomic>

namespace yarp {
namespace os {

class YARP_os_API LogComponent
{
public:
    using LogCallback = yarp::os::Log::LogCallback;
    using LogType = yarp::os::Log::LogType;

    LogComponent(const char* name,
                 LogType minimumPrintLevel = yarp::os::Log::minimumPrintLevel(),
                 LogType minimumForwardLevel = yarp::os::Log::minimumForwardLevel(),
                 LogCallback printCallback = yarp::os::Log::printCallback(),
                 LogCallback forwardCallback = yarp::os::Log::forwardCallback());

    LogComponent(const LogComponent&) = delete;
    LogComponent(LogComponent&&) noexcept = delete;
    LogComponent& operator=(const LogComponent&) = delete;
    LogComponent& operator=(LogComponent&&) noexcept = delete;

    ~LogComponent() = default;

    LogCallback printCallback(yarp::os::Log::LogType t) const;
    LogCallback forwardCallback(yarp::os::Log::LogType t) const;

    constexpr const char* name() const { return m_name; }
    constexpr operator bool() const { return m_name != nullptr; }

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef YARP_NO_DEPRECATED // Since YARP 3.4
    // These methods are currently used to support setVerbose and setQuiet
    // around the code, they should not be used in new code
    void setMinimumPrintLevel(LogType minimumPrintLevel);
    void setMinimumForwardLevel(LogType minimumForwardLevel);
#endif // YARP_NO_DEPRECATED

private:
    const char* m_name { nullptr };

    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_tracePrintEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_debugPrintEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_infoPrintEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_warningPrintEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_errorPrintEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_fatalPrintEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_traceForwardEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_debugForwardEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_infoForwardEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_warningForwardEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_errorForwardEnabled;
    YARP_SUPPRESS_DLL_INTERFACE_WARNING_ARG(std::atomic<bool>) m_fatalForwardEnabled;

    LogCallback m_printCallback { nullptr };
    LogCallback m_forwardCallback { nullptr };

#endif // DOXYGEN_SHOULD_SKIP_THIS

};

#define YARP_DECLARE_LOG_COMPONENT(name) \
    extern const yarp::os::LogComponent& name();

#define YARP_LOG_COMPONENT(name, ...) \
    const yarp::os::LogComponent& name() \
    { \
        static const yarp::os::LogComponent component(__VA_ARGS__); \
        return component; \
    }

#ifndef NDEBUG
#  define yCTrace(component, ...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, nullptr, component()).trace(__VA_ARGS__)
#  define yCTraceOnce(component, ...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_ONCE_CALLBACK, component()).trace(__VA_ARGS__)
#  define yCTraceThreadOnce(component, ...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADONCE_CALLBACK, component()).trace(__VA_ARGS__)
#  define yCTraceThrottle(component, period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THROTTLE_CALLBACK(period), component()).trace(__VA_ARGS__)
#  define yCTraceThreadThrottle(component, period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADTHROTTLE_CALLBACK(period), component()).trace(__VA_ARGS__)
#  define yCTraceCustomTime(customTime, component, ...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, nullptr, component()).trace(__VA_ARGS__)
#  define yCTraceCustomTimeOnce(customTime, component, ...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_ONCE_CALLBACK, component()).trace(__VA_ARGS__)
#  define yCTraceCustomTimeThreadOnce(customTime, component, ...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THREADONCE_CALLBACK, component()).trace(__VA_ARGS__)
#  define yCTraceCustomTimeThrottle(customTime, component, period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THROTTLE_CALLBACK(period), component()).trace(__VA_ARGS__)
#  define yCTraceCustomTimeThreadThrottle(customTime, component, period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THREADTHROTTLE_CALLBACK(period), component()).trace(__VA_ARGS__)
#else
#  define yCTrace(component, ...)                       YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCTraceOnce(component, ...)                   YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCTraceThreadOnce(component, ...)             YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCTraceThrottle(component, period, ...)       YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCTraceThreadThrottle(component, period, ...) YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCTraceCustomTime(customTime, component, ...)                       YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCTraceCustomTimeOnce(customTime, component, ...)                   YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCTraceCustomTimeThreadOnce(customTime, component, ...)             YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCTraceCustomTimeThrottle(customTime, component, period, ...)       YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCTraceCustomTimeThreadThrottle(customTime, component, period, ...) YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#endif

#ifndef YARP_NO_DEBUG_OUTPUT
#  define yCDebug(component, ...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, nullptr, component()).debug(__VA_ARGS__)
#  define yCDebugOnce(component, ...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_ONCE_CALLBACK, component()).debug(__VA_ARGS__)
#  define yCDebugThreadOnce(component, ...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADONCE_CALLBACK, component()).debug(__VA_ARGS__)
#  define yCDebugThrottle(component, period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THROTTLE_CALLBACK(period), component()).debug(__VA_ARGS__)
#  define yCDebugThreadThrottle(component, period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADTHROTTLE_CALLBACK(period), component()).debug(__VA_ARGS__)
#  define yCDebugCustomTime(customTime, component, ...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, nullptr, component()).debug(__VA_ARGS__)
#  define yCDebugCustomTimeOnce(customTime, component, ...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_ONCE_CALLBACK, component()).debug(__VA_ARGS__)
#  define yCDebugCustomTimeThreadOnce(customTime, component, ...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THREADONCE_CALLBACK, component()).debug(__VA_ARGS__)
#  define yCDebugCustomTimeThrottle(customTime, component, period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THROTTLE_CALLBACK(period), component()).debug(__VA_ARGS__)
#  define yCDebugCustomTimeThreadThrottle(customTime, component, period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THREADTHROTTLE_CALLBACK(period), component()).debug(__VA_ARGS__)
#else
#  define yCDebug(component, ...)                       YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCDebugOnce(component, ...)                   YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCDebugThreadOnce(component, ...)             YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCDebugThrottle(component, period, ...)       YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCDebugThreadThrottle(component, period, ...) YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCDebugCustomTime(customTime, component, ...)                       YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCDebugCustomTimeOnce(customTime, component, ...)                   YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCDebugCustomTimeThreadOnce(customTime, component, ...)             YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCDebugCustomTimeThrottle(customTime, component, period, ...)       YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#  define yCDebugCustomTimeThreadThrottle(customTime, component, period, ...) YARP_UNUSED(component); yarp::os::Log::nolog(__VA_ARGS__)
#endif

#define yCInfo(component, ...)                          yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, nullptr, component()).info(__VA_ARGS__)
#define yCInfoOnce(component, ...)                      yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_ONCE_CALLBACK, component()).info(__VA_ARGS__)
#define yCInfoThreadOnce(component, ...)                yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADONCE_CALLBACK, component()).info(__VA_ARGS__)
#define yCInfoThrottle(component, period, ...)          yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THROTTLE_CALLBACK(period), component()).info(__VA_ARGS__)
#define yCInfoThreadThrottle(component, period, ...)    yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADTHROTTLE_CALLBACK(period), component()).info(__VA_ARGS__)
#define yCInfoCustomTime(customTime, component, ...)                          yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, nullptr, component()).info(__VA_ARGS__)
#define yCInfoCustomTimeOnce(customTime, component, ...)                      yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_ONCE_CALLBACK, component()).info(__VA_ARGS__)
#define yCInfoCustomTimeThreadOnce(customTime, component, ...)                yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THREADONCE_CALLBACK, component()).info(__VA_ARGS__)
#define yCInfoCustomTimeThrottle(customTime, component, period, ...)          yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THROTTLE_CALLBACK(period), component()).info(__VA_ARGS__)
#define yCInfoCustomTimeThreadThrottle(customTime, component, period, ...)    yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THREADTHROTTLE_CALLBACK(period), component()).info(__VA_ARGS__)

#define yCWarning(component, ...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, nullptr, component()).warning(__VA_ARGS__)
#define yCWarningOnce(component, ...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_ONCE_CALLBACK, component()).warning(__VA_ARGS__)
#define yCWarningThreadOnce(component, ...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADONCE_CALLBACK, component()).warning(__VA_ARGS__)
#define yCWarningThrottle(component, period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THROTTLE_CALLBACK(period), component()).warning(__VA_ARGS__)
#define yCWarningThreadThrottle(component, period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADTHROTTLE_CALLBACK(period), component()).warning(__VA_ARGS__)
#define yCWarningCustomTime(customTime, component, ...)                       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, nullptr, component()).warning(__VA_ARGS__)
#define yCWarningCustomTimeOnce(customTime, component, ...)                   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_ONCE_CALLBACK, component()).warning(__VA_ARGS__)
#define yCWarningCustomTimeThreadOnce(customTime, component, ...)             yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THREADONCE_CALLBACK, component()).warning(__VA_ARGS__)
#define yCWarningCustomTimeThrottle(customTime, component, period, ...)       yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THROTTLE_CALLBACK(period), component()).warning(__VA_ARGS__)
#define yCWarningCustomTimeThreadThrottle(customTime, component, period, ...) yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THREADTHROTTLE_CALLBACK(period), component()).warning(__VA_ARGS__)

#define yCError(component, ...)                         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, nullptr, component()).error(__VA_ARGS__)
#define yCErrorOnce(component, ...)                     yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_ONCE_CALLBACK, component()).error(__VA_ARGS__)
#define yCErrorThreadOnce(component, ...)               yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADONCE_CALLBACK, component()).error(__VA_ARGS__)
#define yCErrorThrottle(component, period, ...)         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THROTTLE_CALLBACK(period), component()).error(__VA_ARGS__)
#define yCErrorThreadThrottle(component, period, ...)   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, YARP_THREADTHROTTLE_CALLBACK(period), component()).error(__VA_ARGS__)
#define yCErrorCustomTime(customTime, component, ...)                         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, nullptr, component()).error(__VA_ARGS__)
#define yCErrorCustomTimeOnce(customTime, component, ...)                     yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_ONCE_CALLBACK, component()).error(__VA_ARGS__)
#define yCErrorCustomTimeThreadOnce(customTime, component, ...)               yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THREADONCE_CALLBACK, component()).error(__VA_ARGS__)
#define yCErrorCustomTimeThrottle(customTime, component, period, ...)         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THROTTLE_CALLBACK(period), component()).error(__VA_ARGS__)
#define yCErrorCustomTimeThreadThrottle(customTime, component, period, ...)   yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, YARP_THREADTHROTTLE_CALLBACK(period), component()).error(__VA_ARGS__)

#define yCFatal(component, ...)                         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, nullptr, component()).fatal(__VA_ARGS__)
#define yCFatalCustomTime(customTime, component, ...)                         yarp::os::Log(__FILE__, __LINE__, __YFUNCTION__, customTime, nullptr, component()).fatal(__VA_ARGS__)

#ifndef NDEBUG
#  define yCAssert(component, x)                                                       \
    if (!(x)) {                                                                        \
        yCFatal(component, "Assertion failure at %s:%d (%s)", __FILE__, __LINE__, #x); \
    }
#else
#  define yCAssert(component, x) { YARP_UNUSED(component); }
#endif


} // namespace yarp
} // namespace os

#endif // YARP_OS_LOGCOMPONENT_H
