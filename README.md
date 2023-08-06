# OpenGLExamples
Modern OpenGL Example Projects

# Libraries Used:
-WxWidgets   - For window management<br />
-GLEW        - To simplify OpenGL calls<br />
-GLM         - For math support<br />
-stb_image.h - Image loading<br />
-SOIL2       - Image loading<br />

# Notes on getting started:
1. Clone this repo. (Your machine is expected to have a GPU that has minimum OpenGL version of 3.3)

2. I have used Visual Studio 2022 for the C++ projects this repo contains.

3. Due to the various libraries used, there is some setup involved to get the code to build.

4. GLEW 2.2 is included in this repo. I have not tested this code with any other version.

5. glew32d.dll and glew32.dll are required at runtime. They will need to be copied to the debug and release build bin directories respectively. For example, copy glew32d.dll to \bin\x64\Debug.

6. Download and build WxWidgets. I used version 3.2.

7. I recommend downloading and building the WxWidgets source. There are many examples online that show how to do this. To use WxWidgets, I recommend using the property sheet that they include with their code. The projects in this repo use this property sheet. You will likely need to adjust the WxWidgets version and the path in the project file to the specific location that this property sheet is on your machine (in your WxWidgets folder). To apply the property sheet to a project in Visual Studio go to the Property Manager, Right-click the project, and select 'Add Existing Property Sheet'. Then navigate to the WxWidgets folder where the property sheet is.

8. For any new projects you may create using WxWidgets, be sure to change the project linker subsystem to Windows (from Console)  

9. If you're having build issues, be sure to check and adjust as necessary the include paths, additional directories, and library files in the project file for the various libraries used.

# Credits
There are many resources to learn OpenGL. The following are some I used which collectively made the code in this repo possible.<br />
1. Website: https://learnopengl.com/ <br />
2. Book: Computer Graphics Programming in OpenGL with C++ by V.Scott Gordon and John Clevenger <br />
3. Book: Game Programming in C++ by Sanjay Madhav <br />
4. Video: Easy OpenGL + wxWidgets Integration: Simplify Your Graphics by Luke's Dev Tutorial (search YouTube) <br />