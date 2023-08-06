#include "MyApp.h"
#include "MyFrame.h"

/*
* This app draws a quad and applies a texture to it.
* When either the 'Move Up' or 'Move Down' button is
* pressed the quad moves accordingly and the texture
* changes.
*/

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    MyFrame* frame = new MyFrame("OpenGL Texture Demo");
    frame->Show(true);

    return true;
}