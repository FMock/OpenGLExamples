# OpenGLExamples
OpenGL Example Projects

# Libraries Used:
WxWidgets - For window management
GLEW      - To simplify OpenGL calls
GLM       - For math support
SOIL2     - Image loading

# Notes on getting started:
1. Clone this repo. (Your machine is expected to have a GPU that has OpenCV)

3. Due to the various libraries used, there is some setup involved to get the code to build.

2. Download and build WxWidgets. I used version 3.2.

3. For each project that uses WxWidgets, you will likely need to adjust the WxWidgets version and the path to the WxWidgets propertysheet that is noted in the project file.

   <Import Project="..\..\..\..\wxWidgets-3.2.2.1\wxwidgets.props" />  <-- change path is necessary

4. View the project files for paths, directories, and library files for the various libraries used.
