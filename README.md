logx
====

logx is a lightweight loggin-library, which makes extensive use of the new C++11 features like
variadic templates, type_traits, std::thread, ...

It supports some predefined logging-destinations (like terminal or file) called sinks, as well as the possibility to 
write custom sinks. One or more of those sinks can then be registered. Log messages are send via a simple command:

	logx::log("Hello world!");

Variables can be used as following arguments:

	int a = 3;
	int b = 4;
	logx::log("a + b = $1 + $2 = $3",
				a, b, a + b);

Using C++11's new variadic templates, it is not necessary to provide the type of the arguments within the log message,
making the code much more type safe.
