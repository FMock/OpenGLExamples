#pragma once
#include <wx/wx.h>

class OpenGLCanvas;

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);

private:
    OpenGLCanvas* openGLCanvas{ nullptr };
};