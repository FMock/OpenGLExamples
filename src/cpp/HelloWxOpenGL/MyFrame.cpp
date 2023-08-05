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

    auto bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    auto colorButton = new wxButton(this, wxID_ANY, "Change Color");
    auto moveUpButton = new wxButton(this, wxID_ANY, "Move Up");
    auto moveDownButton = new wxButton(this, wxID_ANY, "Move Down");

    bottomSizer->Add(colorButton, 0, wxALL | wxALIGN_CENTER, FromDIP(15));
    bottomSizer->Add(moveUpButton, 0, wxALL | wxALIGN_CENTER, FromDIP(15));
    bottomSizer->Add(moveDownButton, 0, wxALL | wxALIGN_CENTER, FromDIP(15));
    bottomSizer->AddStretchSpacer(1);

    sizer->Add(bottomSizer, 0, wxEXPAND);

    SetSizerAndFit(sizer);

    // Button changes the triangleColor variable of the openGLCanvas
    colorButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
    {
        wxColourData colorData;
        colorData.SetColour(this->openGLCanvas->triangleColor);
        wxColourDialog dialog(this, &colorData);

        if (dialog.ShowModal() == wxID_OK)
        {
            openGLCanvas->triangleColor = dialog.GetColourData().GetColour();
            openGLCanvas->Refresh();
        } 
    });

    // These buttons change the yOffSet variable of the openGLCanvas
    moveUpButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
    {
            openGLCanvas->SetYOffSet(0.01f);
            openGLCanvas->Refresh();
    });

    moveDownButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event)
    {
            openGLCanvas->SetYOffSet(-0.01f);
            openGLCanvas->Refresh();
    });
}