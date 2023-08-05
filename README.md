# OpenGLExamples
OpenGL Example Projects

# Libraries Used:
-WxWidgets - For window management
-GLEW      - To simplify OpenGL calls
-GLM       - For math support
-SOIL2     - Image loading

# Notes on getting started:
1. Clone this repo. (Your machine is expected to have a GPU that has OpenCV)

3. Due to the various libraries used, there is some setup involved to get the code to build.

3. GLEW 2.2 is included in this repo. I have not tested this code with any other version.

4. glew32d.dll and glew32.dll are required at runtime. They will need to be copied to the debug and release build bin directories respectively. For example, copy glew32d.dll to \bin\x64\Debug.

2. Download and build WxWidgets. I used version 3.2.

3. For each project that uses WxWidgets, you will likely need to adjust the WxWidgets version and the path to the WxWidgets propertysheet that is noted in the project file.

4. View the project files for paths, directories, and library files for the various libraries used.
