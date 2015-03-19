# Levels of the API #

The lowest level of the API is the C API, which is declared in the header files in [ppapi/c](http://code.google.com/p/ppapi/source/browse/trunk/c). This represents the lowest level binary interface between a plugin and the browser.

For convenience, a simple C++ wrapper is provided in [ppapi/cpp](http://code.google.com/p/ppapi/source/browse/trunk/cpp) for managing many of the basic tasks of writing a plugin. Most plugin authors will want to use this wrapper if you're using C++. It is written strictly on top of the C API and all the source code will be contained in your plugin. The C++ wrapper is not a binary interface to the browser.

# Interfaces #

The basic concept of PPAPI is the _interface._ In C, this is just a collection of function pointers associated with some similar concept. Interfaces are identified by a name, and should remain stable over time. If additions are necessary in the future, they can be added by creating a "Interface2" with the additions.

This scheme allows the browser and the plugin to implement different capabilities and versions of interfaces over time. For example, a plugin may want clipboard functionality provided on a `Clipboard` interface. If the browser supports working with the clipboard, it will return a pointer to the associated function pointers when queried with the interface name. If it doesn't support such operations, it will return NULL and the plugin will have to degrade. Likewise, a plugin may prefer a newer function on a `Clipboard2` interface. When run under an older browser, only `Clipboard` might be available and the plugin will have to make do.

Both the plugin and the browser expose a `void* GetInterface(const char* name)` function for the other to call to access functionality.

# Naming #

The prefix "PP" is used to help prevent naming collisions, which stands for "Pepper Plugin." Common structs have a `PP_` prefix, for example, `PP_Var` for representing a JavaScript variable, or `PP_Rect` for describing a rectangle.

Interfaces are named according to who implements them. Interfaces implemented by the browser are prefixed by "PPB" where "B" stands for _browser_. Interfaces implemented by the plugin are prefixed by "PPP" where "P" stands for _plugin_. Examples include the `PPB_Core` interface which is a collection of core interfaces implemented by the browser and used by the plugin, and the `PPP_Class` interface which the plugin implements to provide an object accessible to JavaScript in the browser.

In some cases, there might be both a browser and a plugin interface for the same concept. For example, `PPP_Instance` represents the plugin functions the browser calls related to a certain instance. This interface is how things like mouse click events are delivered to the plugin. On the other side, the plugin will call `PPB_Instance` which the browser implements to allow the plugin to manipulate its instance.

# Loading and identifying a plugin #

A plugin is identified by a `PP_Module` (in the C API) or `pp::Module` (in the C++ API). This corresponds to one time a plugin library is loaded into a browser. There might be multiple plugins of different types loaded at the same time.

A `PP_Instance` (in the C API) or `pp::Instance` (in the C++ API) identifies one instance of a plugin, which is one time the page specifies `<embed>`. There may be several instances of a plugin on a page. In this case, there will be one "module" and several "instances" within that module. If a plugin is used across several pages, it is up to the browser whether it loads one global module with many instances, or separate modules for each page. In Google Chrome, for example, there will be one separate module for each renderer process, which may in turn correspond to one tab, or multiple tabs, depending on the [process model](http://www.chromium.org/developers/design-documents/process-models) and how the tabs were opened.

# PP\_Var #

A `PP_Var` (in the C API) or `pp::Var` (in the C++ API) represents an object for binding to JavaScript or the DOM. It can represent boolean types, numeric values, strings, and generic JavaScript-type objects. It can be most convenient to think of a var as a JavaScript variable, which doesn't have a predefined type associated with it.

Vars are used extensively for interacting with the DOM and for passing generic values around. See [interfacing with JavaScript](InterfacingWithJavaScript.md) for more information on these types.

# PP\_Resource #

While a `PP_Var` or `pp::Var` represents a JavaScript object or value, a resource (`PP_Resource` or `pp::Resource`) represents an object managed exclusively within the plugin layer. Examples include audio devices, images, and drawing contexts. The resource value is an opaque handle assigned by the browser that identifies the object.

A resource is reference counted much like a `PP_Var`. The browser will keep the object alive so long as the reference count is nonzero. When the reference count reaches zero, it will be deleted.

### Typical PP\_Resource usage ###

Typically, a resource of one type is managed using one interface. A typical example of a resource interface is `ImageData`, which provides:

  * A `Create` function to create an image resource. The returned resources automatically has a reference count of 1. The plugin is responsible for eventually calling `PPB_Core.ReleaseResource` to release this reference. On error, the creation function will return a resource with a 0 `id`, which is an invalid resource handle.

  * An `IsImageData` function to querying the type of a resource. The browser internally maintains a type for each resource, although the `PP_Resource` struct does not carry an inherent type. In normal usage, a plugin will know the type of its resources since it will have created them, but in some cases, such as for debugging purposes, a plugin will want to check the type.

  * Then the interface will contain functions specifically for working with resources of that type. These functions will generally fail if provided a `PP_Resource` identifying a non-`ImageData` object.