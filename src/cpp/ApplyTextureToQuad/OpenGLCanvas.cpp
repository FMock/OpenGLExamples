#include "OpenGLCanvas.h"
#include "Texture.h"

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
    UnloadData();
    delete openGLContext;
}

/*
* InitializeOpenGLFunctions
* Thsi method Initializes GLEW.
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
    wxLogDebug("OpenGL vendor: %s", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));

    tex0 = R"(..\..\..\assets\container.jpg)"; // default

    // Load Textures
    SetTexture(GetTexture(tex0));
    if (mTexture == nullptr)
    {
        wxMessageBox("Error: Problem loading texture",
            "OpenGL initialization error", wxOK | wxICON_INFORMATION, this);
        return false;
    }

    // Shader code to draw triangle
    constexpr auto vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 inTexCoord;
        uniform float u_yOffSet;

        out vec2 fragTexCoord; // Add texture coordinate as output

        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y + u_yOffSet, aPos.z, 1.0);
	        
	        fragTexCoord = inTexCoord; // Pass along the texture coordinate to frag shader
        }
    )";

    constexpr auto fragmentShaderSource = R"(
        #version 330 core
        
        in vec2 fragTexCoord; // Tex coord input from vertex shader
        out vec4 FragColor;
        uniform sampler2D uTexture;

        void main()
        {
            FragColor = texture(uTexture, fragTexCoord);
        }
    )";

    // Compile vertex shader source
    GLuint vertexShader;
    CompileShaderString(vertexShaderSource, GL_VERTEX_SHADER, vertexShader);

    // Compile fragment shader source
    GLuint fragmentShader;
    CompileShaderString(fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader);

    // Now create a shader program that links together the vertex & frag shaders
    CreateShaderProgramAndLink(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
    -0.5f,  0.5f, 0.f, 0.f, 0.f, // top left
     0.5f,  0.5f, 0.f, 1.f, 0.f, // top right
     0.5f, -0.5f, 0.f, 1.f, 1.f, // bottom right
    -0.5f, -0.5f, 0.f, 0.f, 1.f  // bottom left
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

void OpenGLCanvas::UnloadData()
{
    // Destroy textures
    for (auto i : mTextures)
    {
        i.second->Unload();
        delete i.second;
    }
    mTextures.clear();
}

/// <summary>
/// Callback method for the OnPaint event which clears the current
/// window and does a re-draw.
/// </summary>
/// <param name="WXUNUSED">event that triggered OnPaint</param>
void OpenGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);

    if (!isOpenGLInitialized)
        return;

    SetCurrent(*openGLContext);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this

    // Move the triangle
    glUseProgram(m_shaderProgram);
    GLint yOffSetLocation = glGetUniformLocation(m_shaderProgram, "u_yOffSet");

    if (yOffSetLocation >= 0) // success, found location of u_yOffSet on the GPU
    {
        // Now adjust the uniform value
        glUniform1f(yOffSetLocation, myOffSet);
    }
    else
    {
        wxMessageBox("Could not find u_yOffSet location on GPU", "Location lookup failure", wxOK | wxICON_INFORMATION, this);
    }

    glBindVertexArray(mVertexArray->GetVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexArray->GetEBO());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draws the two triangles that form a rectangle

    SwapBuffers();
}

/// <summary>
/// Callback for the window re-size event.
/// The texture will be changed back to the default.
/// </summary>
/// <param name="event">The event that triggered OnSize</param>
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

        SetTexture(GetTexture(tex0)); // reset to default texture
    }

    event.Skip();
}

/// <summary>
/// Adjusts the myOffSet member which facilitates the movement of the 
/// quad. glUniform1f uses yOffSet to change the value of u_yOffSet 
/// in the vertex shader.
/// </summary>
/// <param name="y"></param>
void OpenGLCanvas::SetYOffSet(float y)
{
    myOffSet += y;
}

/// <summary>
/// Gets the Texture from the map. If not found, it loads the image and 
/// adds a new Texture to the map.
/// </summary>
/// <param name="fileName">std::string which is the image file name with path</param>
/// <returns></returns>
Texture* OpenGLCanvas::GetTexture(const std::string& fileName)
{
    Texture* tex = nullptr;
    auto iter = mTextures.find(fileName);
    if (iter != mTextures.end())
    {
        tex = iter->second;
    }
    else
    {
        tex = new Texture();
        if (tex->Load(fileName))
        {
            mTextures.emplace(fileName, tex);
        }
        else
        {
            delete tex;
            tex = nullptr;
            wxMessageBox("Texture Load Problem", "Load Error", wxOK | wxICON_INFORMATION, this);
        }
    }
    return tex;
}

/// <summary>
/// Sets the mTexture member to the Texture passed in if valid.
/// </summary>
/// <param name="texture">Pointer to a Texture object</param>
/// <returns>true if successful, false other wise</returns>
bool OpenGLCanvas::SetTexture(Texture* texture)
{
    if (!texture)
        return false;
    mTexture = texture;
    mTexWidth = texture->GetWidth();
    mTexHeight = texture->GetHeight();
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID()); // you must bind or the texture will not change
    return true;
}
