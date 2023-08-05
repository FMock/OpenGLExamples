#pragma once
#include <wx/wx.h>
#include <GL/glew.h> // must be included before glcanvas.h
#include <wx/glcanvas.h>
#include "MyFrame.h"
#include "VertexArray.h"

/*
* This class contains the OpenGL canvas.
*/

class OpenGLCanvas : public wxGLCanvas
{
public:
    OpenGLCanvas(MyFrame* parent, const wxGLAttributes& canvasAttrs);
    ~OpenGLCanvas();

    bool InitializeOpenGLFunctions();
    bool InitializeOpenGL();

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);

    wxColour triangleColor{ wxColour(255, 128, 51) };

    void SetYOffSet(float y);

private:
    wxGLContext* openGLContext;
    bool isOpenGLInitialized{ false };
    bool CompileShaderString(const std::string& fileName, GLenum shaderType, GLuint& outShader);
    bool CreateShaderProgramAndLink(GLuint& vertexShader, GLuint& fragmentShader);
    bool IsCompiled(GLuint& shader);
    unsigned int m_shaderProgram;
    float m_yOffSet = 0.0f;
    VertexArray* mVertexArray;
};