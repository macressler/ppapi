<h2>API documentation</h2>

The C and C++ API documentation for Pepper can be found at the Native Client developer [website](https://developers.google.com/native-client/).

<h2>Pepper code location</h2>

Currently the canonical version of the PPAPI code has moved to the Chromium subversion repo http://src.chromium.org/viewvc/chrome/trunk/src/ppapi/ to help with browser versioning and dependencies (see [Get the code](http://dev.chromium.org/developers/how-tos/get-the-code) for more details). We're still working out what role this public repo should have (maybe it will be a copy of the public interface where it's easier for external projects to pull from).



&lt;hr&gt;



PPAPI is a cross-platform API for plugins for web browsers. It is currently an experimental feature of [Chromium](http://www.chromium.org/) and [Google Chrome](http://www.google.com/chrome).

For an introduction, please see:
  * [Getting started](GettingStarted.md)
  * [Important concepts](Concepts.md)

If you are working on the Chromium implementation of this API, please see the [Pepper plugin implementation design doc](http://sites.google.com/a/chromium.org/dev/developers/design-documents/pepper-plugin-implementation).