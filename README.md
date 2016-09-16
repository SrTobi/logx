logx
====

logx is a lightweight loggin-library, which makes extensive use of the new C++11 features such as variadic templates, type_traits and std::thread.

Usage
-----

Logging is done, using the function `log`.

    logx::log() << "Hello world!" << logx::end;

logx uses the C++ streaming style, you know from std::cout:

    logx::log() << "a + b = " << a << " + " << b << " = " << a + b << logx::end;

Tags
----

Instead of providing speacial facilities for log categories or log levels, logx uses a far more generic approach: __tags__.
Every tag is simply a class deriving from `logx::tag`. They can then handled by the backend specially (see next section).

    namespace tag = logx::tags;
    tag::cat category("main");
    
    ...
    
    // log message has the severity 'info' and the category 'main'
    // logxSOURCE adds context dependent source information (i.e. line + source file)
    logx::log() << "Hello World" << logx::end[tag::info, category, logxSOURCE]);
    
Sinks
-----

After `logx::log` was called the log message and it's arguments will be passed to the backend of logx.
There, it will be processed and then passed to every registered __sink__.
In logx a sink is simply a function taking a *sink_message*, which is a processed log message.
The sink is responsible for writing the message somewhere; for example to the console:

    // a very simple sink
    void write_to_cout(const logx::sink_message& msg)
    {
        std::wcout << msg.msg() << std::endl;
    }

    ...

    // init the the core and register the sink
    logx::core& core = logx::core::get_core();
    core.init();
    core.add_sink(&write_to_cout);

There are also some predefined sinks such as a terminal-sink, a file-sink and a xml-sink.

Using Logger
------------

A logger can be used to bundle tags:

    // create a logger (i.e. in a class) with the category "worker-1"
	logx::logger<logx::tags::cat> workerLogger { "worker-1" };

    // create a derived logger for a specific job
    auto jobLogger = workerLogger.with(logx::tags::job("job[12382]"));

    ...

    // use the logger instead of 'logx::log()'
    jobLogger << "Job started..." << logx::end;

Predefined macros
-----------------

Use log macros to bundle context dependent tags like tags::source, tags::thread and tags::time.
Macros also make it more easy to use tag::log_level.

    logxINFO("Start app...")[logx::tags::cat("app")];
    logxWARN("$ENV{" << envVar << "} no set!");
    logxERR("Failed to do something!");

    // use with logger
    appLogger << logxINFO("test...");