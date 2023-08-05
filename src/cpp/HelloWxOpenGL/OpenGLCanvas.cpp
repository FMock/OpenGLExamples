#include "OpenGLCanvas.h"


/*
* OpenGLCanvas
* Initializes an OpenGL 3.3 Context
*/
OpenGLCanvas::OpenGLCanvas(MyFrame* parent, const wxGLAttributes& canvasAttrs)
    : wxGLCanvas(parent, canvasAttrs)
{
    wxGLContextAttrs ctxAttrs;
    ctxAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 3).EndList();
    openGLContext = new wxGLContext(this, nullptr, &ctxAttrs);

    if (!openGLContext->IsOK())
    {
        wxMessageBox("This sample needs an OpenGL 3.3 capable driver.",
            "OpenGL version error", wxOK | wxICON_INFORMATION, this);
        delete openGLContext;
        openGLContext = nullptr;
    }

    Bind(wxEVT_PAINT, &OpenGLCanvas::OnPaint, this);
    Bind(wxEVT_SIZE, &OpenGLCanvas::OnSize, this);
}

OpenGLCanvas::~OpenGLCanvas()
{
    delete openGLContext;
    delete mVertexArray;
}

/*
* InitializeOpenGLFunctions
* This method initializes GLEW.
* GLEW makes it easier to initialize OpenGL functions.
*/
bool OpenGLCanvas::InitializeOpenGLFunctions()
{
    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        wxLogError("OpenGL GLEW initialization failed: %s", reinterpret_cast<const char*>(glewGetErrorString(err)));
        return false;
    }

    wxLogDebug("Status: Using GLEW %s", reinterpret_cast<const char*>(glewGetString(GLEW_VERSION)));

    return true;
}

/*
* InitializeOpenGL
* This is the main OpenGL initialization function
*/
bool OpenGLCanvas::InitializeOpenGL()
{
    if (!openGLContext)
        return false;

    // Activate the OpenGL context
    SetCurrent(*openGLContext);

    // Initialize GLEW
    if (!InitializeOpenGLFunctions())
    {
        wxMessageBox("Error: Could not initialize OpenGL function pointers.",
            "OpenGL initialization error", wxOK | wxICON_INFORMATION, this);
        return false;
    }

    // Output graphics driver info
    wxLogDebug("OpenGL version: %s", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    wxLogDebug("OpenGL vendor: %s",  reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
     

    // Shader code to draw triangle
    constexpr auto vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform float u_yOffSet;
        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y + u_yOffSet, aPos.z, 1.0);
        }
    )";

    constexpr auto fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 triangleColor;
        void main()
        {
            FragColor = triangleColor;
        }
    )";

    // Compile vertex shader source
    unsigned int vertexShader;
    if (!CompileShaderString(vertexShaderSource, GL_VERTEX_SHADER, vertexShader))
        return false;

    // Compile fragment shader source
    unsigned int fragmentShader;
    if (!CompileShaderString(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader))
        return false;

    // Now create a shader program that links together the vertex & frag shaders
    if (!CreateShaderProgramAndLink(vertexShader, fragmentShader))
        return false;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // verticies for a quad (rectangle)
    float vertices[] = {
    -0.5f,   0.5f, 0.0f,  // top LEFT
     0.5f,   0.5f, 0.0f,  // top right
     0.5f,  -0.5f, 0.0f,  // bottom right
    -0.5f,  -0.5f, 0.0f   // bottom left 
    };

    unsigned int indices[] = { 
        0, 1, 2,  // first Triangle
        2, 3, 0   // second Triangle
    };

    mVertexArray = new VertexArray(vertices, 4, indices, 6);
    mVertexArray->SetActive(); // bind VAO

    isOpenGLInitialized = true;

    return true;
}

bool OpenGLCanvas::CompileShaderString(const std::string& contents, GLenum shaderType, GLuint& outShader)
{
    if (!contents.empty())
    {
        const char* contentsChar = contents.c_str();

        // Create a shader of the specified type
        outShader = glCreateShader(shaderType);

        // Set the source characters and try to compile
        glShaderSource(outShader, 1, &(contentsChar), nullptr);
        glCompileShader(outShader);

        if (!IsCompiled(outShader))
        {
            std::cout << "Failed to compile shader " << contents << std::endl;
            return false;
        }
    }
    else
    {
        wxMessageBox("Shader string was empty", "Shader compile error", wxOK | wxICON_INFORMATION, this);
        return false;
    }

    return true;
}

bool OpenGLCanvas::IsCompiled(GLuint& shader)
{
    GLint status;
    // Query the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetShaderInfoLog(shader, 511, nullptr, buffer);
        wxMessageBox(buffer, "GLSL Compile Failed", wxOK | wxICON_INFORMATION, this);
        return false;
    }

    return true;
}

bool OpenGLCanvas::CreateShaderProgramAndLink(GLuint& vertexShader, GLuint& fragmentShader)
{
    int success;
    char infoLog[512];

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    // Link shader program
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        wxLogDebug("Shader Program Linking Failed: %s", infoLog);
        return false;
    }

    return true;
}

void OpenGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);

    if (!isOpenGLInitialized)
        return;

    SetCurrent(*openGLContext);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this

    // Move the triangles (the quad)
    glUseProgram(m_shaderProgram);
    GLint yOffSetLocation = glGetUniformLocation(m_shaderProgram, "u_yOffSet");

    if (yOffSetLocation >= 0) // success, found location of u_yOffSet on the GPU
    {
        // Now adjust the uniform value
        glUniform1f(yOffSetLocation, m_yOffSet);
    }
    else
    {
        wxMessageBox("Could not find u_yOffSet location on GPU", "Location lookup failure", wxOK | wxICON_INFORMATION, this);
    }

    //Change the color of the triangles (the quad)
    int colorLocation = glGetUniformLocation(m_shaderProgram, "triangleColor");
    glUniform4f(colorLocation, triangleColor.Red() / 255.0f, triangleColor.Green() / 255.0f, triangleColor.Blue() / 255.0f, 1.0f);

    glBindVertexArray(mVertexArray->GetVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexArray->GetEBO());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draws the two triangles that form a rectangle

    SwapBuffers();
}

void OpenGLCanvas::OnSize(wxSizeEvent& event)
{
    bool firstApperance = IsShownOnScreen() && !isOpenGLInitialized;

    if (firstApperance)
    {
        InitializeOpenGL(); // calls glewInit()
    }

    if (isOpenGLInitialized)
    {
        auto viewPortSize = event.GetSize() * GetContentScaleFactor();
        glViewport(0, 0, viewPortSize.x, viewPortSize.y);
    }

    event.Skip();
}

// glUniform1f uses m_yOffSet to change the value of u_yOffSet in the vertex shader
void OpenGLCanvas::SetYOffSet(float y)
{
    m_yOffSet += y;
}
