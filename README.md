logx
====

logx is a lightweight loggin-library, which makes extensive use of the new C++11 features such as variadic templates, type_traits and std::thread.

Usage
-----

Logging is done, using the function `log`.

	logx::log("Hello world!");

Arguments can be passed to the function after the actual message and are referenced by `$` followed by the index of the argument starting with 1:

	int a = 3;
	int b = 4;
	logx::log("a + b = $1 + $2 = $3",
				a, b, a + b);

Using C++11's new variadic templates, it is not necessary to provide the type of the arguments within the log message! If you like the the C++ streaming style, you can also use

    logx::log() << "a + b = " << a << " + " << b << " = " << a + b;

Tags
----

Instead of providing speacial facilities for log categories or log levels, logx uses a far more generic approach: __tags__. Every tag is simply a class deriving from `logx::tag`. They are then handled by the backend specially (see next section).

    namespace tag = logx::tags;
    tag::cat category("main");
    
    ...
    
    logx::log("Hello World", category, tag::info);
    
Sinks
-----

After `logx::log` was called the log message and it's arguments will be passed to the backend of logx. There it will be processed and then passed to every registered __sink__. In logx a sink is simply a function taking a *sink_message*, which is a processed log message. The sink is responsible for writing the message somewhere; for example to the console:

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
    
