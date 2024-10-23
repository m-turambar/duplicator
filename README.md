# duplicator
Uses an IDXGISwapChain to do HW rendering to a regular window. Contents of which are duplicated from the screen using a IDXGIOutputDuplication

To build:
cd build
cmake ..
This will create solution/build file.
You can either build manually from there, or use tasks.json, which contains command to use msbuild.

To debug:
launch.json contains command to debug.
