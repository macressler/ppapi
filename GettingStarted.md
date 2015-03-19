# Getting started using the C API #

You must implement the following three functions:
  * `PPP_InitializeModule` which is called on startup and returns 0 on success.
  * `PPP_ShutdownModule` which is called on shutdown.
  * `PPP_GetInterface` which is called when a new plugin instance is created for a page.
which are declared in [ppp.h](http://code.google.com/p/ppapi/source/browse/trunk/c/ppp.h).

An example "no op" implementation of these three functions in C is in [examples/stub/stub.c](http://code.google.com/p/ppapi/source/browse/trunk/examples/stub/stub.c) in the source repository.

You must compile these functions such that the names are exported from the shared library to call. For Microsoft VC and GCC the `PP_EXPORT` macro used in the declarations should be sufficient.

# Getting started using the C++ API #

Using the C++ wrapper, there is slightly more to the stub program, but it does more. You must implement these things:
  * `pp::CreateModule` which is the factory function for your plugin's specific `pp::Module` implementation.
  * A specialization of `pp::Module` that implements `CreateInstance`, which is in turn the factory function for your plugin's specific `pp::Instance` implementation.
  * A specialization of `pp::Instance` which represents one plugin on a web page.

An example "no op" implementation of these concepts is in [examples/stub/stub.cc](http://code.google.com/p/ppapi/source/browse/trunk/examples/stub/stub.cc) in the source repository.

# Next steps #

Once you get a simple plugin loading, you may want to explore:

  * [2D painting](2DPaintingModel.md)
  * [Interfacing with JavaScript](InterfacingWithJavaScript.md)