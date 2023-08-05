#include "MyApp.h"
#include "MyFrame.h"

/*
* This app draws a colored quad (rectangle) that can be moved
* and its color changed.
*/

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    MyFrame* frame = new MyFrame("Hello Wx OpenGL!");
    frame->Show(true);

    return true;
}