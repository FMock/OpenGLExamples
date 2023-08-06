#pragma once
#include <wx/wx.h>
#include <GL/glew.h>
#include <wx/glcanvas.h>
#include "MyFrame.h"
#include "Texture.h"
#include "VertexArray.h"
#include <unordered_map>

/*
* This class is the OpenGL canvas.
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

    void SetYOffSet(float y);
    Texture* GetTexture(const std::string& fileName);
    bool SetTexture(Texture* texture);

private:
    wxGLContext* openGLContext;
    bool isOpenGLInitialized{ false };
    bool CompileShaderString(const std::string& fileName, GLenum shaderType, GLuint& outShader);
    bool CreateShaderProgramAndLink(GLuint& vertexShader, GLuint& fragmentShader);
    bool IsCompiled(GLuint& shader);
    void UnloadData();
    unsigned int m_shaderProgram;
    float myOffSet = 0.0f;
    Texture* mTexture;
    std::string tex0;
    VertexArray* mVertexArray;
    std::unordered_map<std::string, Texture*> mTextures; // Map of textures loaded
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;
};