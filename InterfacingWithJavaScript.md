# This page is deprecated #

Scripting is no longer supported for plugins. Please see http://src.chromium.org/viewvc/chrome/trunk/src/ppapi/c/ppb_messaging.h?revision=92312&view=markup for how to communicate between a plugin and a page.



&lt;hr&gt;



#summary How to interface back and forth between a plugin and `JavaScript`.

# Overview #

A `PP_Var` (in the C API) or a `pp::Var` (in the C++ API) represents an object for binding to JavaScript or the DOM. It can represent boolean types, numeric values, strings, and generic JavaScript-type objects. It can be most convenient to think of a var as a JavaScript variable, which doesn't have a predefined type associated with it.

### Var lifetimes ###
A var containing a boolean or numeric type holds its own copy of the boolean or numeric data internally.  A var containing a string or an object is reference counted, with the object itself residing inside the browser. When the reference count reaches 0, the string or object will be freed. A C plugin uses the functions `PPB_Var.AddRef` and `PPB_Var.Release` to take and release references.

It is legal to call `AddRef` and `Release` on any type of var. But since boolean and numeric types are "inline" inside the var, there is no extra memory management required.

**Important:** JavaScript objects can have references to other JavaScript objects. Keeping a reference to one object can in turn keep many other objects alive. For example, a plugin might have a reference to a simple callback object. But that object may have a reference to an object in the DOM. Keeping the reference to the callback object alive can in turn cause a leak of the entire DOM of the page.

In the C++ API, the `pp::Var` object acts like a smart pointer. The reference count will be automatically maintained for you, and the object or string will be alive as long as there is a `pp::Var` referencing it. Copying a `pp::Var` will create another reference to the same underlying object.

# Accessing a Var #

## Numbers ##

JavaScript does not distinguish between floating point numbers and integers. In practice, JavaScript libraries implement types as an optimization, but particular behavior is not guaranteed, and the type (double vs. int) may depend on the sequence of operations used to compute the value.

As a result, a plugin should not assume that a particular numeric value is an integer or a double, but should be prepared to handle results in either type and convert as necessary. The C++ wrapper attempts to make this handling a little easier by automatically converting between integers and doubles and providing an `is_number` call that you can use to check if the type is either an integer or a double.

## Strings ##

A string is a sequence of UTF-8 characters. It is reference counted as described above. A C plugin can create a new string using `PPB_Var.VarFromUtf8` and get the string associated with a var using `PPB_Var.VartoUtf8`. A C++ plugin can just use the string constructor of `pp::Var`.

After creating a string, your input can be discarded, the string will be copied by the browser into an internal object. To modify it, you need to assign a new string.

_Warning:_ Since a `pp::Var` is a reference to a reference counted object in the browser, copying a `pp::Var` representing a string corresponds to just taking another reference to the same underlying string. In particular, it is _not_ copy-on-write and assigning the value to one reference will update the values of all references to the same string.

```
pp::Var a = "hello";
pp::Var b = a;        // b references a's "hello".
b = "goodbye";        // BOTH b and a are now "goodbye".
```

Because of this, plus the additional overhead of creating a `pp::Var` and the fact that a var can represent many other types, it is generally recommended that you use your own string class (such as `std::string`), and only use `pp::Var` when interfacing with the plugin API.

## Objects ##

Objects in variants are references to JavaScript-accessible objects. You can use the `PPB_Var` interface to call functions and manipulate properties on the object. In C++, the `pp::Var` interface contains all the manipulation functions. These functions are valid when the var type is `PP_VarType_Object`. If the var is any other type, the functions will fail and throw an exception.

### Error handling ###

Any method on an object might fail. In JavaScript, this is indicated by an exception. Exceptions are passed through the object interface as output parameters to each function. If you are not interested in exception information, you can pass `NULL` (in C++ this argument has a default value of `NULL` so you can omit it if desired).

When you pass the address of a var to receive the exception, you must initialize it to a "void" type (the default `pp::Var()` constructor does this for you). If there was an exception after an operation, the var's type will no longer be null.

All methods will check the exception argument, if given to see if it is of non-void type. If it is non-void, they will immediately return without doing anything. This behavior allows you to perform a sequence of calls without doing individual checks for each calls, and then checking whether the sequence succeeded or failed at the end. Doing this can clean up the calling code quite a bit in cases where you don't specifically expect an error.

Here is an example of code where you wouldn't necessarily expect any calls to fail, and you merely want to do a sanity check at the end to see if there was a problem. The exception will always contain the first exception hit, and no other var operations using that exception will have any effect.

```
pp::Var exception;  // Use initial "void" var for exception;
pp::Var window = GetWindowObject();
pp::Var doc = window.GetProperty("document", &exception);
pp::Var my_element = doc.Call("getElementById", "myelt", &exception);
my_element.SetProperty("innerHTML", "Hello, world", &exception);

if (!exception.is_void())
  return false;  // One of the above functions failed.
```

This code is equivalent to the JavaScript code:

```
try {
  window.document.getElementById("myelt").innerHTML = "Hello, world";
} catch (exception) {
  return false;
}
```

This var's type and contents will be dependent on the type of error received. Some basic usage errors (like trying to access a property on a numeric value) will throw string exceptions containing a description of what went wrong. Most errors will throw objects just like in JavaScript.

### Getting and setting properties ###

Properties include both functions and data members on an object. You can get (`GetProperty`) and set (`SetProperty`) properties, query if they exist (`HasProperty`), and see if a given property is a function that can be called (`HasMethod`). You can also get a list of all enumerable properties (`GetAllPropertyNames`) and remove existing ones (`RemoveProperty`).

Properties in JavaScript can be named with either string identifiers or numbers. Strings are used for most properties, but numeric properties are how arrays are implemented. For example, `foo[0]` in JavaScript is equivalent to `foo.GetProperty(pp::Var(0));`. Attempting to access properties with any non-integer or non-string identifiers will fail. In the case of calling functions, only string identifiers are supported.

# Implementing JavaScript-accessible objects #

It is possible to provide a backend implementation of a `PP_Var` in native code that can be called from JavaScript (or potentially other plugins). Calls to functions or to set and get properties will call through to functions you implement on your plugin's main thread.

In the C API, you provide an implementation of the interface `PPP_Class` and pass a pointer to your class to the `PPB_Var.CreateObject` function to actually create the object. In the C++ API, just inherit from `pp::ScriptableObject` and implement the virtual functions of that class. To convert your scriptable object to a var, just call the `pp::Var` constructor with a pointer to your object:

```
class MyObject : public pp::ScriptableObject {
  ...
};

pp::Var MakeMyObject() {
  return pp::Var(new MyObject());
}
```

You can now use your custom `pp::Var` just like any other in the system. You can call functions on it, get and set properties, and return it as the result of other functions for use by JavaScript and other plugins. The lifetime of your object will be bound to the lifetime of the var, which is reference counted by the browser. When there are no more references to the object, the object's destructor will automatically be called; you don't need to do anything special to manage the lifetime of your object.

### Properties ###

As discussed in "getting and setting properties" above, properties can be named with either strings or integers. Most plugins probably won't need to support arrays and will only implement properties with string names. However, you should be careful to check the type of the property name when responding to get/set/has property since it can be an integer.

If you have a property, be sure to not only implement the getter and setter, but also respond to `HasProperty` accordingly, and include your property name in response to `GetAllPropertyNames`. Forgetting to do this can lead to unexpected behavior.

If you do support numeric properties for array operations, be careful with the types. In JavaScript, you can say `object[0] = 1;` or use a string representation of the number `object["0"] = 1;`. These are semantically equivalent, but your get/set property implementation will be called with a var of type number in the first case, but a var of type string in the second. Although in most cases, you will be called with an integer and your array access can be very efficient, be sure to check for numbers passed as strings and treat them the same.

### Error handling ###

Each function on the `PPP_Class`/`pp::ScriptableObject` has an out parameter corresponding to the exception. If you set this value to a non-void var, it will correspond to throwing an exception in JavaScript (or, for PPAPI plugins, being passed up to the caller as the exception through the var interface). If you don't encounter an error, just don't write to the exception pointer.

When calling an object, you can specify a null exception (see above). However, when implementing an object, this exception will never be null. The browser will always create an exception object, and just won't pass it "up" the stack if the caller isn't interested. This frees the implementation from having to null-check the exception object.

Likewise, when manipulating an object, you can expect that the function won't do anything if the exception is non-void (see above). From the object implementation side, you don't need to worry about this behavior. Before calling into your implemention, the browser will check the exception passed in by the caller and will immediately return if the exception is non-void; you won't even see the call from the implementation side. This frees the implementation from having to check the exception for "type void".

When throwing an exception, your return value will be ignored. It is recommended that you return a void var (this is what you get using the default `pp::Var()` constructor).

This example shows how to do error handling to implement a function call to implement `object.doSomethingAwesome(3.14159);`.

```
pp::Var MyObject::Call(const pp::Var& method_name,
                       const std::vector<pp::Var>& args,
                       pp::Var* exception) {
  if (method_name.is_string() && method_name == "doSomethingAwesome") {
    if (args.size() != 1 || !args[0].is_number()) {
      // Error, we expect a number as the argument.
      *exception = pp::Var("Error calling doSomethingAwesome, you must pass exactly one number");
      return pp::Var();
    }
    return DoSomethingAwesome(args[0].AsDouble());
  }

  // Unknown method.
  *exception = pp::Var("Unknown function");
  return pp::Var();
}
```

### The plugin "instance object" ###

If you want to allow JavaScript on the page to call into your plugin, you expose an object that represents the plugin instance. In the C API, this is returned by the `PPP_Instance.GetInstanceObject` function implemented by the plugin. In the C++ API, just implement the virtual function `pp::Instance::GetInstanceObject`.

### See also ###

  * The header file for the C interface [PPP\_Class](http://code.google.com/p/ppapi/source/browse/trunk/c/ppp_class.h).
  * The header file for the C++ wrapper [ScriptableObject](http://code.google.com/p/ppapi/source/browse/trunk/cpp/scriptable_object.h).