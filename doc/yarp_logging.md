Logging with YARP {#yarp_logging}
=================

[TOC]

YARP has an internal mechanism to help you debugging your distributed
application.

While you can simply use `printf` or `std::cout` methods to debug your
application locally, if you use the functionalities offered, YARP will collect
and show several additional information, including system and network time,
file, line, thread id, ecc, and eventually forward them to
[yarplogger](@ref yarplogger).

YARP is also able to detect if it is running on a console or inside
[yarprun](@ref yarprun) and if the application output is forwarded to
[yarplogger](@ref yarplogger) (using the `--log` option), and change it's
output accordingly, so that the extra information are forwarded properly.


## Log Levels

YARP defines 6 levels for log.

| Level       | Value | Stream   | Notes                                                         |
|:-----------:|:-----:|----------|---------------------------------------------------------------|
| `[TRACE]`   | 1     | `stdout` | Generates no binary code in release mode.                     |
| `[DEBUG]`   | 2     | `stdout` | Generates no binary code if `YARP_NO_DEBUG_OUTPUT` is defined |
| `[INFO]`    | 3     | `stdout` |                                                               |
| `[WARNING]` | 4     | `stderr` |                                                               |
| `[ERROR]`   | 5     | `stderr` |                                                               |
| `[FATAL]`   | 6     | `stderr` | Aborts the execution and prints a backtrace                   |

For each these levels, YARP offers logging functions.

## Generic Macros

YARP offers some generic macros that do not require any configuration, and will
use the default logging settings.

| Level       | C-Style                          | Stream       |
|:-----------:|----------------------------------|--------------|
| `[TRACE]`   | `yTrace(const char* msg, ...)`   | `yTrace()`   |
| `[DEBUG]`   | `yDebug(const char* msg, ...)`   | `yDebug()`   |
| `[INFO]`    | `yInfo(const char* msg, ...)`    | `yInfo()`    |
| `[WARNING]` | `yWarning(const char* msg, ...)` | `yWarning()` |
| `[ERROR]`   | `yError(const char* msg, ...)`   | `yError()`   |
| `[FATAL]`   | `yFatal(const char* msg, ...)`   | `yFatal()`   |


### C-Style macros

The `yDebug(const char* msg, ...)` macro family is defined in the
`<yarp/os/Log.h>` header.
These macro and work in similar way to the C `printf()` function.
For example:

```{.cpp}
int g = 42;
yDebug("The value of g is %d", g);
```

will print

```
[DEBUG] The value of g is 42
```

Note that the string does not end with `\n`, since YARP will automatically add
it.

### Stream Macros

The `yDebug()` macro family is defined in the `<yarp/os/LogStream.h>` header.
These macros work in similar way to C++ `std::cout`.
For example:

```{.cpp}
int g = 42;
yDebug() << "The value of g is" << g;
```

will print

```
[DEBUG] The value of g is 42
```

Note that there is no need to add a white space between the fields, since these
are added automatically. Also YARP will automatically add a `\n` at the end of
the stream.


## Components

The output of YARP applications tends to be very messy, especially when using
using multiple threads, or several devices at the same time.

Therefore YARP allows to define *Log Components* that can be used to filter the
output and follow the execution flow.

| Level       | Component C-Style                                                     | Component Stream                                |
|:-----------:|-----------------------------------------------------------------------|-------------------------------------------------|
| `[TRACE]`   | `yCTrace(const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCTrace(const yarp::os::Logcomponent& comp)`   |
| `[DEBUG]`   | `yCDebug(const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCDebug(const yarp::os::Logcomponent& comp)`   |
| `[INFO]`    | `yCInfo(const yarp::os::Logcomponent& comp, const char* msg, ...)`    | `yCInfo(const yarp::os::Logcomponent& comp)`    |
| `[WARNING]` | `yCWarning(const yarp::os::Logcomponent& comp, const char* msg, ...)` | `yCWarning(const yarp::os::Logcomponent& comp)` |
| `[ERROR]`   | `yCError(const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCError(const yarp::os::Logcomponent& comp)`   |
| `[FATAL]`   | `yCFatal(const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCFatal(const yarp::os::Logcomponent& comp)`   |


### Defining a Logging Component

In order to define a log component, the `YARP_LOG_COMPONENT` macro (defined in
`<yarp/os/LogComponent.h>`)should be used in a `.cpp` file. For example

```{.cpp}
YARP_LOG_COMPONENT(FOO, "foo.bar")
```

will define a logging component FOO that will be printed as `foo.bar`. In order
to use the component in that file there is no need to do anything else.
If you need to use it in more than one file, you need to forward declare it
somewhere else (in a `.h` file):

```{.cpp}
YARP_DECLARE_LOG_COMPONENT(FOO)
```

### C-Style Macros

The `yDebug(const yarp::os::Logcomponent& comp, const char* msg, ...)` macro
family is defined in the `<yarp/os/LogComponent.h>` header.
These macros work in similar way to the C `printf()` function.
For example (assuming that the `FOO` component is defined as shown before):

```{.cpp}
int g = 42;
yDebug(FOO, "The value of g is %d", g);
```

will print

```
[DEBUG] |foo.bar| The value of g is 42
```

Like for the `yDebug` family, there is no need to add `\n` at the end of the
string.


### Stream Macros

The `yCDebug(const yarp::os::Logcomponent& comp)` macro family is defined in
the `<yarp/os/LogComponent.h>` header.

These macros work in similar way to C++ `std::cout`.
For example (assuming that the `FOO` component is defined as shown before):

```{.cpp}
int g = 42;
yDebug(FOO) << "The value of g is" << g;
```

will print

```
[DEBUG] |foo.bar| The value of g is 42
```

Like for the `yDebug` family, there is no need to add a white space between the
fields, or `\n` at the end of the stream.


## Limited Macros

Sometimes some logging line should be printed only once, or only at most once
in a certain amount of time.
YARP has several utilities to achieve this


### yDebugOnce() Family

These log lines are printed only once in the execution of the program.

| Level       | C-Style                              | Stream           |
|:-----------:|--------------------------------------|------------------|
| `[TRACE]`   | `yTraceOnce(const char* msg, ...)`   | `yTraceOnce()`   |
| `[DEBUG]`   | `yDebugOnce(const char* msg, ...)`   | `yDebugOnce()`   |
| `[INFO]`    | `yInfoOnce(const char* msg, ...)`    | `yInfoOnce()`    |
| `[WARNING]` | `yWarningOnce(const char* msg, ...)` | `yWarningOnce()` |
| `[ERROR]`   | `yErrorOnce(const char* msg, ...)`   | `yErrorOnce()`   |
| `[FATAL]`   | N/A                                  | N/A              |


| Level       | Component C-Style                                                         | Component Stream                                    |
|:-----------:|---------------------------------------------------------------------------|-----------------------------------------------------|
| `[TRACE]`   | `yCTraceOnce(const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCTraceOnce(const yarp::os::Logcomponent& comp)`   |
| `[DEBUG]`   | `yCDebugOnce(const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCDebugOnce(const yarp::os::Logcomponent& comp)`   |
| `[INFO]`    | `yCInfoOnce(const yarp::os::Logcomponent& comp, const char* msg, ...)`    | `yCInfoOnce(const yarp::os::Logcomponent& comp)`    |
| `[WARNING]` | `yCWarningOnce(const yarp::os::Logcomponent& comp, const char* msg, ...)` | `yCWarningOnce(const yarp::os::Logcomponent& comp)` |
| `[ERROR]`   | `yCErrorOnce(const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCErrorOnce(const yarp::os::Logcomponent& comp)`   |
| `[FATAL]`   | N/A                                                                       | N/A                                                 |


### yDebugThreadOnce() Family

These log lines are printed at most once by every thread during the execution of
the program.

| Level       | C-Style                                    | Stream                 |
|:-----------:|--------------------------------------------|------------------------|
| `[TRACE]`   | `yTraceThreadOnce(const char* msg, ...)`   | `yTraceThreadOnce()`   |
| `[DEBUG]`   | `yDebugThreadOnce(const char* msg, ...)`   | `yDebugThreadOnce()`   |
| `[INFO]`    | `yInfoThreadOnce(const char* msg, ...)`    | `yInfoThreadOnce()`    |
| `[WARNING]` | `yWarningThreadOnce(const char* msg, ...)` | `yWarningThreadOnce()` |
| `[ERROR]`   | `yErrorThreadOnce(const char* msg, ...)`   | `yErrorThreadOnce()`   |
| `[FATAL]`   | N/A                                        | N/A                    |


| Level       | Component C-Style                                                               | Component Stream                                          |
|:-----------:|---------------------------------------------------------------------------------|-----------------------------------------------------------|
| `[TRACE]`   | `yCTraceThreadOnce(const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCTraceThreadOnce(const yarp::os::Logcomponent& comp)`   |
| `[DEBUG]`   | `yCDebugThreadOnce(const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCDebugThreadOnce(const yarp::os::Logcomponent& comp)`   |
| `[INFO]`    | `yCInfoThreadOnce(const yarp::os::Logcomponent& comp, const char* msg, ...)`    | `yCInfoThreadOnce(const yarp::os::Logcomponent& comp)`    |
| `[WARNING]` | `yCWarningThreadOnce(const yarp::os::Logcomponent& comp, const char* msg, ...)` | `yCWarningThreadOnce(const yarp::os::Logcomponent& comp)` |
| `[ERROR]`   | `yCErrorThreadOnce(const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCErrorThreadOnce(const yarp::os::Logcomponent& comp)`   |
| `[FATAL]`   | N/A                                                                             | N/A                                                       |


### yDebugThrottle() Family

These log lines are printed at most once every `period` seconds.

| Level       | C-Style                                                 | Stream                            |
|:-----------:|---------------------------------------------------------|-----------------------------------|
| `[TRACE]`   | `yTraceThrottle(double period, const char* msg, ...)`   | `yTraceThrottle(double period)`   |
| `[DEBUG]`   | `yDebugThrottle(double period, const char* msg, ...)`   | `yDebugThrottle(double period)`   |
| `[INFO]`    | `yInfoThrottle(double period, const char* msg, ...)`    | `yInfoThrottle(double period)`    |
| `[WARNING]` | `yWarningThrottle(double period, const char* msg, ...)` | `yWarningThrottle(double period)` |
| `[ERROR]`   | `yErrorThrottle(double period, const char* msg, ...)`   | `yErrorThrottle(double period)`   |
| `[FATAL]`   | N/A                                                     | N/A                               |


| Level       | Component C-Style                                                                            | Component Stream                                                       |
|:-----------:|----------------------------------------------------------------------------------------------|------------------------------------------------------------------------|
| `[TRACE]`   | `yCTraceThrottle(const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCTraceThrottle(const yarp::os::Logcomponent& comp, double period)`   |
| `[DEBUG]`   | `yCDebugThrottle(const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCDebugThrottle(const yarp::os::Logcomponent& comp, double period)`   |
| `[INFO]`    | `yCInfoThrottle(const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`    | `yCInfoThrottle(const yarp::os::Logcomponent& comp, double period)`    |
| `[WARNING]` | `yCWarningThrottle(const yarp::os::Logcomponent& comp, double period, const char* msg, ...)` | `yCWarningThrottle(const yarp::os::Logcomponent& comp, double period)` |
| `[ERROR]`   | `yCErrorThrottle(const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCErrorThrottle(const yarp::os::Logcomponent& comp, double period)`   |
| `[FATAL]`   | N/A                                                                           | N/A                                                     |


### yDebugThreadThrottle() Family

These log lines are printed at most once by every thread every `period` seconds.

| Level       | C-Style                                                       | Stream                                  |
|:-----------:|---------------------------------------------------------------|-----------------------------------------|
| `[TRACE]`   | `yTraceThreadThrottle(double period, const char* msg, ...)`   | `yTraceThreadThrottle(double period)`   |
| `[DEBUG]`   | `yDebugThreadThrottle(double period, const char* msg, ...)`   | `yDebugThreadThrottle(double period)`   |
| `[INFO]`    | `yInfoThreadThrottle(double period, const char* msg, ...)`    | `yInfoThreadThrottle(double period)`    |
| `[WARNING]` | `yWarningThreadThrottle(double period, const char* msg, ...)` | `yWarningThreadThrottle(double period)` |
| `[ERROR]`   | `yErrorThreadThrottle(double period, const char* msg, ...)`   | `yErrorThreadThrottle(double period)`   |
| `[FATAL]`   | N/A                                                           | N/A                                     |


| Level       | Component C-Style                                                                                  | Component Stream                                                             |
|:-----------:|----------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------|
| `[TRACE]`   | `yCTraceThreadThrottle(const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCTraceThreadThrottle(const yarp::os::Logcomponent& comp, double period)`   |
| `[DEBUG]`   | `yCDebugThreadThrottle(const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCDebugThreadThrottle(const yarp::os::Logcomponent& comp, double period)`   |
| `[INFO]`    | `yCInfoThreadThrottle(const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`    | `yCInfoThreadThrottle(const yarp::os::Logcomponent& comp, double period)`    |
| `[WARNING]` | `yCWarningThreadThrottle(const yarp::os::Logcomponent& comp, double period, const char* msg, ...)` | `yCWarningThreadThrottle(const yarp::os::Logcomponent& comp, double period)` |
| `[ERROR]`   | `yCErrorThreadThrottle(const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCErrorThreadThrottle(const yarp::os::Logcomponent& comp, double period)`   |
| `[FATAL]`   | N/A                                                                                                | N/A                                                                          |

## Assert

YARP offers some macros that can help programmers find bugs in their programs,
and stop the execution of the program when some unexpected situation happens.

The macros

| Assert               | Component Assert                                          |
|:--------------------:|:---------------------------------------------------------:|
| `yAssert(condition)` | `yCAssert(const yarp::os::Logcomponent& comp, condition)` |

will check the value of the condition, and throw a fatal error, if the assertion
is not true.

When the code is compiled in release mode, these assertion will just disappear
from the compiled code.

## y*CustomTime family

Users have the possibility of logging information using a custom timestamp, 
including the timestamp in the macro. These macros expand on the previous macros'
by including a customtime term.

### Generic Macros

| Level       | C-Style                                                       | Stream                                  |
|:-----------:|---------------------------------------------------------------|-----------------------------------------|
| `[TRACE]`   | `yTraceCustomTime(double customtime, const char* msg, ...)`   | `yTraceCustomTime(double customtime)`   |
| `[DEBUG]`   | `yDebugCustomTime(double customtime, const char* msg, ...)`   | `yDebugCustomTime(double customtime)`   |
| `[INFO]`    | `yInfoCustomTime(double customtime, const char* msg, ...)`    | `yInfoCustomTime(double customtime)`    |
| `[WARNING]` | `yWarningCustomTime(double customtime, const char* msg, ...)` | `yWarningCustomTime(double customtime)` |
| `[ERROR]`   | `yErrorCustomTime(double customtime, const char* msg, ...)`   | `yErrorCustomTime(double customtime)`   |
| `[FATAL]`   | `yFatalCustomTime(double customtime, const char* msg, ...)`   | `yFatalCustomTime(double customtime)`   |

### Macros with Components

| Level       | Component C-Style                                                                                  | Component Stream                                                             |
|:-----------:|----------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------|
| `[TRACE]`   | `yCTraceCustomTime(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCTraceCustomTime(double customtime, const yarp::os::Logcomponent& comp)`   |
| `[DEBUG]`   | `yCDebugCustomTime(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCDebugCustomTime(double customtime, const yarp::os::Logcomponent& comp)`   |
| `[INFO]`    | `yCInfoCustomTime(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`    | `yCInfoCustomTime(double customtime, const yarp::os::Logcomponent& comp)`    |
| `[WARNING]` | `yCWarningCustomTime(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)` | `yCWarningCustomTime(double customtime, const yarp::os::Logcomponent& comp)` |
| `[ERROR]`   | `yCErrorCustomTime(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCErrorCustomTime(double customtime, const yarp::os::Logcomponent& comp)`   |
| `[FATAL]`   | `yCFatalCustomTime(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCFatalCustomTime(double customtime, const yarp::os::Logcomponent& comp)`   |

### Limited Macros

| Level       | C-Style                                                           | Stream                                      |
|:-----------:|-------------------------------------------------------------------|---------------------------------------------|
| `[TRACE]`   | `yTraceCustomTimeOnce(double customtime, const char* msg, ...)`   | `yTraceCustomTimeOnce(double customtime)`   |
| `[DEBUG]`   | `yDebugCustomTimeOnce(double customtime, const char* msg, ...)`   | `yDebugCustomTimeOnce(double customtime)`   |
| `[INFO]`    | `yInfoCustomTimeOnce(double customtime, const char* msg, ...)`    | `yInfoCustomTimeOnce(double customtime)`    |
| `[WARNING]` | `yWarningCustomTimeOnce(double customtime, const char* msg, ...)` | `yWarningCustomTimeOnce(double customtime)` |
| `[ERROR]`   | `yErrorCustomTimeOnce(double customtime, const char* msg, ...)`   | `yErrorCustomTimeOnce(double customtime)`   |
| `[FATAL]`   | N/A                                                               | N/A                                         |


| Level       | Component C-Style                                                                                      | Component Stream                                                                |
|:-----------:|--------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------|
| `[TRACE]`   | `yCTraceCustomTimeOnce(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCTraceCustomTimeOnce(double customtime, const yarp::os::Logcomponent& comp)`   |
| `[DEBUG]`   | `yCDebugCustomTimeOnce(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCDebugCustomTimeOnce(double customtime, const yarp::os::Logcomponent& comp)`   |
| `[INFO]`    | `yCInfoCustomTimeOnce(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`    | `yCInfoCustomTimeOnce(double customtime, const yarp::os::Logcomponent& comp)`    |
| `[WARNING]` | `yCWarningCustomTimeOnce(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)` | `yCWarningCustomTimeOnce(double customtime, const yarp::os::Logcomponent& comp)` |
| `[ERROR]`   | `yCErrorCustomTimeOnce(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCErrorCustomTimeOnce(double customtime, const yarp::os::Logcomponent& comp)`   |
| `[FATAL]`   | N/A                                                                                                    | N/A                                                                             |

| Level       | C-Style                                                                 | Stream                                            |
|:-----------:|-------------------------------------------------------------------------|---------------------------------------------------|
| `[TRACE]`   | `yTraceCustomTimeThreadOnce(double customtime, const char* msg, ...)`   | `yTraceCustomTimeThreadOnce(double customtime)`   |
| `[DEBUG]`   | `yDebugCustomTimeThreadOnce(double customtime, const char* msg, ...)`   | `yDebugCustomTimeThreadOnce(double customtime)`   |
| `[INFO]`    | `yInfoCustomTimeThreadOnce(double customtime, const char* msg, ...)`    | `yInfoCustomTimeThreadOnce(double customtime)`    |
| `[WARNING]` | `yWarningCustomTimeThreadOnce(double customtime, const char* msg, ...)` | `yWarningCustomTimeThreadOnce(double customtime)` |
| `[ERROR]`   | `yErrorCustomTimeThreadOnce(double customtime, const char* msg, ...)`   | `yErrorCustomTimeThreadOnce(double customtime)`   |
| `[FATAL]`   | N/A                                                                     | N/A                                               |


| Level       | Component C-Style                                                                                            | Component Stream                                                                      |
|:-----------:|--------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------|
| `[TRACE]`   | `yCTraceCustomTimeThreadOnce(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCTraceCustomTimeThreadOnce(double customtime, const yarp::os::Logcomponent& comp)`   |
| `[DEBUG]`   | `yCDebugCustomTimeThreadOnce(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCDebugCustomTimeThreadOnce(double customtime, const yarp::os::Logcomponent& comp)`   |
| `[INFO]`    | `yCInfoCustomTimeThreadOnce(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`    | `yCInfoCustomTimeThreadOnce(double customtime, const yarp::os::Logcomponent& comp)`    |
| `[WARNING]` | `yCWarningCustomTimeThreadOnce(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)` | `yCWarningCustomTimeThreadOnce(double customtime, const yarp::os::Logcomponent& comp)` |
| `[ERROR]`   | `yCErrorCustomTimeThreadOnce(double customtime, const yarp::os::Logcomponent& comp, const char* msg, ...)`   | `yCErrorCustomTimeThreadOnce(double customtime, const yarp::os::Logcomponent& comp)`   |
| `[FATAL]`   | N/A                                                                                                          | N/A                                                                                   |

| Level       | C-Style                                                                              | Stream                                                         |
|:-----------:|--------------------------------------------------------------------------------------|----------------------------------------------------------------|
| `[TRACE]`   | `yTraceCustomTimeThrottle(double customtime, double period, const char* msg, ...)`   | `yTraceCustomTimeThrottle(double customtime, double period)`   |
| `[DEBUG]`   | `yDebugCustomTimeThrottle(double customtime, double period, const char* msg, ...)`   | `yDebugCustomTimeThrottle(double customtime, double period)`   |
| `[INFO]`    | `yInfoCustomTimeThrottle(double customtime, double period, const char* msg, ...)`    | `yInfoCustomTimeThrottle(double customtime, double period)`    |
| `[WARNING]` | `yWarningCustomTimeThrottle(double customtime, double period, const char* msg, ...)` | `yWarningCustomTimeThrottle(double customtime, double period)` |
| `[ERROR]`   | `yErrorCustomTimeThrottle(double customtime, double period, const char* msg, ...)`   | `yErrorCustomTimeThrottle(double customtime, double period)`   |
| `[FATAL]`   | N/A                                                                                  | N/A                                                            |


| Level       | Component C-Style                                                                                                         | Component Stream                                                                                   |
|:-----------:|---------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------
------------------------------------------------|
| `[TRACE]`   | `yCTraceCustomTimeThrottle(double customtime, const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCTraceCustomTimeThrottle(double customtime, const yarp::os::Logcomponent& comp, double period)`   |
| `[DEBUG]`   | `yCDebugCustomTimeThrottle(double customtime, const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCDebugCustomTimeThrottle(double customtime, const yarp::os::Logcomponent& comp, double period)`   |
| `[INFO]`    | `yCInfoCustomTimeThrottle(double customtime, const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`    | `yCInfoCustomTimeThrottle(double customtime, const yarp::os::Logcomponent& comp, double period)`    |
| `[WARNING]` | `yCWarningCustomTimeThrottle(double customtime, const yarp::os::Logcomponent& comp, double period, const char* msg, ...)` | `yCWarningCustomTimeThrottle(double customtime, const yarp::os::Logcomponent& comp, double period)` |
| `[ERROR]`   | `yCErrorCustomTimeThrottle(double customtime, const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCErrorCustomTimeThrottle(double customtime, const yarp::os::Logcomponent& comp, double period)`   |
| `[FATAL]`   | N/A                                                                                                                       | N/A                                                                                                |


| Level       | C-Style                                                                                    | Stream                                                               |
|:-----------:|--------------------------------------------------------------------------------------------|----------------------------------------------------------------------|
| `[TRACE]`   | `yTraceCustomTimeThreadThrottle(double customtime, double period, const char* msg, ...)`   | `yTraceCustomTimeThreadThrottle(double customtime, double period)`   |
| `[DEBUG]`   | `yDebugCustomTimeThreadThrottle(double customtime, double period, const char* msg, ...)`   | `yDebugCustomTimeThreadThrottle(double customtime, double period)`   |
| `[INFO]`    | `yInfoCustomTimeThreadThrottle(double customtime, double period, const char* msg, ...)`    | `yInfoCustomTimeThreadThrottle(double customtime, double period)`    |
| `[WARNING]` | `yWarningCustomTimeThreadThrottle(double customtime, double period, const char* msg, ...)` | `yWarningCustomTimeThreadThrottle(double customtime, double period)` |
| `[ERROR]`   | `yErrorCustomTimeThreadThrottle(double customtime, double period, const char* msg, ...)`   | `yErrorCustomTimeThreadThrottle(double customtime, double period)`   |
| `[FATAL]`   | N/A                                                                                        | N/A                                                                  |


| Level       | Component C-Style                                                                                                               | Component Stream                                                                                         |
|:-----------:|---------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------
------------------------------------------------------|
| `[TRACE]`   | `yCTraceCustomTimeThreadThrottle(double customtime, const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCTraceCustomTimeThreadThrottle(double customtime, const yarp::os::Logcomponent& comp, double period)`   |
| `[DEBUG]`   | `yCDebugCustomTimeThreadThrottle(double customtime, const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCDebugCustomTimeThreadThrottle(double customtime, const yarp::os::Logcomponent& comp, double period)`   |
| `[INFO]`    | `yCInfoCustomTimeThreadThrottle(double customtime, const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`    | `yCInfoCustomTimeThreadThrottle(double customtime, const yarp::os::Logcomponent& comp, double period)`    |
| `[WARNING]` | `yCWarningCustomTimeThreadThrottle(double customtime, const yarp::os::Logcomponent& comp, double period, const char* msg, ...)` | `yCWarningCustomTimeThreadThrottle(double customtime, const yarp::os::Logcomponent& comp, double period)` |
| `[ERROR]`   | `yCErrorCustomTimeThreadThrottle(double customtime, const yarp::os::Logcomponent& comp, double period, const char* msg, ...)`   | `yCErrorCustomTimeThreadThrottle(double customtime, const yarp::os::Logcomponent& comp, double period)`   |
| `[FATAL]`   | N/A                                                                                                                             | N/A                                                                                                      |


## Configuration

The log behaviour can be configured using build flags, environment variables,
and configuration files.

### Build flags

When building a project, a few build option can change the behavior of the
logging system.

When building in "Release mode" (i.e. `NDEBUG` is defined, and the optimizations
are enabled), the `[TRACE]` level does not produce any binary code.
The `[DEBUG]` level behaves in the same way when `YARP_NO_DEBUG_OUTPUT` is
defined.

This happens only for the projects that are built with these options. If you are
building in "Release mode", but you are linking a library that was built in
"Debug mode", your program will still execute some code for each trace line in
the library.

As already mentioned, the `NDEBUG` option also controls whether the `yAssert`s
are enabled or not.

### Environment variables

The default behaviour for the logger can be configured using the some
environment variables.

The `YARP_COLORED_OUTPUT` can be set to `1` to enable colors in the console
output (if supported).

If colors are enabled, the `YARP_COMPACT_OUTPUT` can be used to

If the `YARP_VERBOSE_OUTPUT` is set to `1`, YARP will print several extra
information for each log line.

The `YARP_FORWARD_LOG_ENABLE` can be used to enable the forwarding of the log
to [yarplogger](@ref yarplogger)

By default, YARP prints all log from `[DEBUG]` to `[FATAL]`. If `[DEBUG]` is not
required, it is possible to disable it by setting the `YARP_DEBUG_ENABLE`
environment variable to `0`. It is also possible to enable the `[TRACE]` level
by setting `YARP_TRACE_ENABLE` to `1`.

For `yarp::os` internal logging, the default minimum level is `[INFO]`. It is
possible to changing by using `YARP_QUIET`, to change the minimum level to
`[WARNING]`, and `YARP_VERBOSE`, to change it to `[DEBUG]`.

Finally, the `YARP_DEBUG_LOG_ENABLE` can be enabled to debug the output of the
logger internal component, in case you want to debug the output of your
application.


### Configuration File

FIXME TODO (configuring the log using configuration files is not enabled yet)

## Advanced

### Forwarding output

By default, the logging callback prints the output on the `stdout`/`stderr`.
When a YARP program is started by [yarprun --log](@ref yarprun), this will
forward the output on a YARP port, that can be read by
[yarplogger](@ref yarplogger), and displayed.

This behaviour can be obtained also without [yarprun](@ref yarprun), by
setting the `YARP_FORWARD_LOG_ENABLE` environment variable to `1`.
Please note that, while [yarprun](@ref yarprun) is able to forward all the
output of the application (including `printf` and `std::out` output), this
method will forward only the output using YARP log utilities.

At the moment, not all the information gathered is forwarded. It is possible
to enable them by using the following environment variables:

* `YARP_FORWARD_CODEINFO_ENABLE`
* `YARP_FORWARD_HOSTNAME_ENABLE`
* `YARP_FORWARD_PROCESSINFO_ENABLE`
* `YARP_FORWARD_BACKTRACE_ENABLE`

Please note that `yarp::os` internal logging is never forwarded, since this
could cause recursions that will crash the program.


### Custom Logging functions

The default print callback is smart and can detect when it is running inside
[yarprun](@ref yarprun), and if this is forwarding the output.
It is also possible to change the output format.

If you don't like the behaviour, or need to change it for some specific
application, it is possible to change the callback with a custom one using the
`yarp::os::Log::setPrintCallback()` method.
It's also possible to disable output completely, by setting it to `nullptr`.

If log forwarding is enabled, it is also possible to change the forwarding
callback with a custom one, using the `yarp::os::Log::setForwardCallback()`
method. This allows, for example, to log everything to a different system.

Inside both the print callback and the forward callback, it is possible to call
YARP default callbacks by calling `yarp::os::Log::defaultPrintCallback()(...)`
and `yarp::os::Log::defaultForwardCallback()(...)`


### Log Components

It's possible to change the default behaviour for each specific Log Component by
adding some optional parameter to the `YARP_LOG_COMPONENT` macro.
The full syntax is:

```{.cpp}
YARP_LOG_COMPONENT(component,
                   name,
                   minimumPrintLevel = yarp::os::Log::minimumPrintLevel(),
                   minimumForwardLevel = yarp::os::Log::minimumForwardLevel(),
                   printCallback = yarp::os::Log::printCallback(),
                   forwardCallback = yarp::os::Log::forwardCallback())
```

By changing `minimumPrintLevel` it's possible to decide the minimum level that
is printed using the `printCallback`.

By changing `minimumForwardLevel` it's possible to decide the minimum level that
is forwarded using the `forwardCallback`.

By changing the `printCallback`, the output printed on the command line for this
component can be customized as needed. If this is set to `nullptr`, the log
component will not be printed.

By changing the `forwardCallback`, it is possible to change the callback that
will handle forwarding. If this is set to `nullptr`, the log component will not
be forwarded.


### Providing Support for the yDebug() Stream Operator

In order to provide support for yarp logging to your classes, you need to
overload the stream operator (`operator<<`).

This means that you have to declare the `operator<<` function taking `LogStream`
and your class as parameters, somewhere outside your class:

```{.cpp}
yarp::os::LogStream operator<<(yarp::os::LogStream stream, const MyClass& c)
{
    // stream << ...
    return stream;
}
```

In order to access your private class members inside this function you also have
to declare this function as `friend` inside your class declaration

```{.cpp}
class MyClass
{
    // ...
    friend yarp::os::LogStream operator<<(yarp::os::LogStream stream, const MyClass& c);
}
```

Then you can just log your class in this way:

```{.cpp}
MyClass c
yDebug() << c;
```

This is a full example that shows how to overload and use the stream operator
for a custom class.


```{.cpp}
#include <yarp/os/Network.h>
#include <yarp/os/LogStream.h>

#include <sstream>
#include <iomanip>

class Date
{
    int yr;
    int mo;
    int da;

public:
    Date(int y, int m, int d) : yr(y), mo(m), da(d) {}
    friend yarp::os::LogStream operator<<(yarp::os::LogStream stream, const Date& dt);
};

yarp::os::LogStream operator<<(yarp::os::LogStream stream, const Date& dt)
{
    std::ostringstream oss;
    oss << std::setw(4) << dt.yr << '/' << std::setw(2) << std::setfill('0') << dt.mo << '/' << std::setw(2) << std::setfill('0') << dt.da;
    stream << oss.str();
    return stream;
}

int main()
{
    yarp::os::Network yarp;
    Date today(2020, 06, 17);
    Date tomorrow(2020, 06, 18);
    yDebug() << today << tomorrow;
    return 0;
}
```


## Recommendations

### Log Levels

The user is free to use these levels as he wishes, but we recommend to follow
these guidelines:

* `[TRACE]` - Information useful for the developer. These are normally turned
              off, and might not be compiled in some configurations.
* `[DEBUG]` - Diagnostic information required to debug the application, useful for
              developers, but also for system administrators, etc.
* `[INFO]` - The normal output of the application.
* `[WARNING]` - Anything that can cause troubles, but from which the application
                will recover and continue to run normally.
* `[ERROR]` - An error in the application. The application will continue to run,
              but it might behave in an unexpected way.
* `[FATAL]` - An error that will cause the application to stop.

The `[FATAL]` level, in some cases, can be very dangerous, since it will abort
the execution.

Developers often leave lots of debugging lines commented out in their code
These lines, after a while, tend to become useless, since they are not updated
when the code is changed, variables renamed or removed, and they just clutter up
the code.
A better practice is to remove them if they are not needed, or eventually to use
`[TRACE]` for the lines that are actually useful for the developers.
Most compilers will just cut any `[TRACE]` line out of the produced code when
building in "Release mode", but they will still interpret the line, ensuring
therefore that the debug line is updated together with the code.


### Component Variable Name

Please be aware that `YARP_LOG_COMPONENT(name)` will generate a method `name`
that can generate some namimg conflicts. If the component is used only in a
compilation unit (i.e. in a `.cpp` file), you should consider defining it
inside an anonymous namespace, i.e.

```{.cpp}
namespace {
YARP_LOG_COMPONENT(FOO, "foo.bar")
}
```


### Component Names

The component name is defined as a string.
Any string is valid, nonethleless we recommend to use a hierarchical naming
scheme, using `.` to separate levels.
For example, all components in YARP are defined as `yarp.*`, and components in
`yarp::os` are defined as `yarp.os.*`.


### Logic inside asserts

Asserts are only compiled into code when `NDEBUG` is not defined.
Do not insert code that should always be executed, since this will not be
compiled in release mode. For example this is ok:

```{.cpp}
yarp::os::Port p;
[[maybe_unused]] bool ret = p.open("...");
yAssert(ret);
```

but this is not ok, since the port will not be open in release mode:

```{.cpp}
yarp::os::Port p;
yAssert(p.open("..."));
```

Note that, in the first example, the `[[maybe_unused]]` c++ attribute can be
used to disable the unused variable warning in release mode, since the `ret`
variable is used only inside the `yAssert`, and therefore "unused".


### Custom Print Callback

While it's technically possible to do anything by replacing the print callback,
including forwarding to a different system, it is recommended to avoid any
network operation inside the callback.

YARP internal components use the logging system, but disable the forwarding,
since this could cause recursion and ultimately crash the program.
Using any YARP port inside the custom print callback could have the same
implications.

Using other networking systems (for example to send the output to some other
logging system) inside the print callback is strongly discouraged.
You should use the forward callback for this kind of tasks.

The custom print callback should be used for example to change the format of
the output, or to log the output to a file.


### Understanding the Implications of Log Forwarding

When log forwarding is demanded to [yarprun](@ref yarprun), the output of the
process is simply piped to another process, which will forward it to the logger.
YARP logging system understands that the output is being forwarded, and changes
the output slightly, to match the format expected by yarplogger.
This should not have a big impact on the process.
Also note that [yarprun](@ref yarprun) will forward all the output of the
application, including `printf`, `std::out`, ecc, even though the extra
information will not be attached to these output lines.

On the other hand, enabling the `YARP_FORWARD_LOG_ENABLE` environment variable
will add some extra instructions for every log line, will cause the YARP
application to open an extra port, that will have one or more extra thread,
requires the `Network` to be initialized, etc. This could slow down the
application therefore you should take this into account before enabling it.


### Limited Output

`yDebugOnce()` and the other macros are useful in some cases, but you should
take into consideration that these macros do not come completely for free,
they use a callback mechanism to decide whether the output should be printed or
not, and therefore some extra code is executed every time, even when there is no
output.
Therefore in some cases there might be good reason for using alternatives.


### Formatting Output

The stream version of the debugging macros, is faster to use, and does not
require to know the type of the variable printed, but they offer a very limited
way of formatting the output.
If you require a proper formatting, the c-style macros are recommended.


### Performances

The C-style `yDebug()` macro family are slightly more performant than the stream
version, since the stream version imply objects construction, copies, and
destruction. If you care about the performances of some parts of code, you
should take into consideration the C-style version.

Please note normally the C-style version uses an internal buffer, and therefore
it does not use any dynamic allocation of memory, unless the internal buffer
is not big enough to fit the output.
If you care about performances, you should also ensure that your log output does
not exceed 1024 bytes per log line.