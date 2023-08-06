#include <wx/colordlg.h>
#include "MyFrame.h"
#include "OpenGLCanvas.h"

MyFrame::MyFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{
    auto sizer = new wxBoxSizer(wxVERTICAL);

    wxGLAttributes vAttrs;
    vAttrs.PlatformDefaults().Defaults().EndList();

    if (wxGLCanvas::IsDisplaySupported(vAttrs))
    {
        openGLCanvas = new OpenGLCanvas(this, vAttrs);
        openGLCanvas->SetMinSize(FromDIP(wxSize(640, 480)));
        sizer->Add(openGLCanvas, 1, wxEXPAND);
    }

    auto bottomSizer    = new wxBoxSizer(wxHORIZONTAL);
    auto moveUpButton   = new wxButton(this, wxID_ANY, "Move Up");
    auto moveDownButton = new wxButton(this, wxID_ANY, "Move Down");

    bottomSizer->Add(moveUpButton, 0, wxALL | wxALIGN_CENTER, FromDIP(15));
    bottomSizer->Add(moveDownButton, 0, wxALL | wxALIGN_CENTER, FromDIP(15));
    bottomSizer->AddStretchSpacer(1);

    sizer->Add(bottomSizer, 0, wxEXPAND);

    SetSizerAndFit(sizer);

    // These buttons change the yOffSet variable of the openGLCanvas
    moveUpButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
    {
            openGLCanvas->SetYOffSet(0.01f);
            
            static std::string tex1 = R"(..\..\..\assets\container_up.jpg)";
            openGLCanvas->SetTexture(openGLCanvas->GetTexture(tex1));
            openGLCanvas->Refresh();
    });

    moveDownButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
    {
            openGLCanvas->SetYOffSet(-0.01f);

            static std::string tex2 = R"(..\..\..\assets\container_down.jpg)";
            openGLCanvas->SetTexture(openGLCanvas->GetTexture(tex2));
            openGLCanvas->Refresh();
    });
}