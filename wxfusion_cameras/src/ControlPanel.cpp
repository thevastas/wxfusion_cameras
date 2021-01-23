#include "ControlPanel.h"

#include "MainWindow.h"
#include "id.h"

//ControlPanel::ControlPanel(wxPanel* parent)
PTZPanel::PTZPanel(wxFrame* parent)
	: wxPanel(parent, -1, wxDefaultPosition, wxSize(300, 200))
{
    m_parent = parent;
    
    // Pan tilt buttons
    int ptzbuttonsize = 30;
    wxSize ptzarrowsize = wxSize(ptzbuttonsize, ptzbuttonsize);
    int ptzbuttonspacing = 5;
    int ptzbuttonoriginx = 10;
    int ptzbuttonoriginy = 10;


    m_upleft = new wxButton(this, window::id::BUPLEFT, wxT("UL"),       wxPoint(ptzbuttonoriginx, ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_up = new wxButton(this, window::id::BUP, wxT("U"),                wxPoint(ptzbuttonoriginx+ptzbuttonsize+ptzbuttonspacing, ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_upright = new wxButton(this, window::id::BUPRIGHT, wxT("UR"),     wxPoint(ptzbuttonoriginx + 2 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));

    m_left = new wxButton(this, window::id::BLEFT, wxT("L"),            wxPoint(ptzbuttonoriginx, ptzbuttonoriginy+ptzbuttonsize+ptzbuttonspacing), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_right = new wxButton(this, window::id::BRIGHT, wxT("R"),          wxPoint(ptzbuttonoriginx + 2 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + ptzbuttonsize + ptzbuttonspacing), wxSize(ptzbuttonsize, ptzbuttonsize));

    m_downleft = new wxButton(this, window::id::BDOWNLEFT, wxT("DL"),   wxPoint(ptzbuttonoriginx, ptzbuttonoriginy + 2 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_down = new wxButton(this, window::id::BDOWN, wxT("D"),            wxPoint(ptzbuttonoriginx + ptzbuttonsize + ptzbuttonspacing, ptzbuttonoriginy + 2 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_downright = new wxButton(this, window::id::BDOWNRIGHT, wxT("DR"), wxPoint(ptzbuttonoriginx + 2*(ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 2 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));

    m_zoomin = new wxButton(this, window::id::BZOOMIN, wxT("+"),
        wxPoint(250, ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_textzoom = new wxStaticText(this, -1, wxT("Zoom"), wxPoint(150+ptzbuttonsize+ ptzbuttonspacing+15, ptzbuttonoriginy+6));
    m_textzoom->SetForegroundColour(wxColor(214, 214, 214));
    m_zoomout = new wxButton(this, window::id::BZOOMOUT, wxT("-"),
        wxPoint(150, ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));
    

    m_focusin = new wxButton(this, window::id::BFOCUSIN, wxT("+"),
        wxPoint(250, ptzbuttonoriginy+ptzbuttonsize+ptzbuttonspacing), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_textfocus = new wxStaticText(this, -1, wxT("Focus"), wxPoint(150 + ptzbuttonsize + ptzbuttonspacing + 15, ptzbuttonoriginy + ptzbuttonsize + ptzbuttonspacing + 6));
    m_textfocus->SetForegroundColour(wxColor(214, 214, 214));
    m_focusout = new wxButton(this, window::id::BFOCUSOUT, wxT("-"),
        wxPoint(150, ptzbuttonoriginy + ptzbuttonsize + ptzbuttonspacing), wxSize(ptzbuttonsize, ptzbuttonsize));


    m_textspeed = new wxStaticText(this, -1, wxT("Speed"), wxPoint(150 + 20, ptzbuttonoriginy + 2 * (ptzbuttonsize + ptzbuttonspacing) + 6));
    m_textspeed->SetForegroundColour(wxColor(214, 214, 214));
    m_ptzspeed = new wxSpinCtrl(this, window::id::BPTZSPEED,
        wxT("3"), wxPoint(230, ptzbuttonoriginy + 2*(ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize+20, ptzbuttonsize), wxSP_ARROW_KEYS,
        1, 5, 1);
    m_ptzspeed->SetBackgroundColour(wxColor(214, 214, 214));
    
    m_textpos = new wxStaticText(this, -1, wxT("Position"), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy + 3.5 * (ptzbuttonsize + ptzbuttonspacing)));
    m_textpos->SetForegroundColour(wxColor(214, 214, 214));

    m_pos1 = new wxButton(this, window::id::BPTZPOS1, wxT("1"), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos2 = new wxButton(this, window::id::BPTZPOS2, wxT("2"), wxPoint(ptzbuttonoriginx + ptzbuttonsize + ptzbuttonspacing, ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos3 = new wxButton(this, window::id::BPTZPOS3, wxT("3"), wxPoint(ptzbuttonoriginx + 2 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos4 = new wxButton(this, window::id::BPTZPOS4, wxT("4"), wxPoint(ptzbuttonoriginx + 3 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos5 = new wxButton(this, window::id::BPTZPOS5, wxT("5"), wxPoint(ptzbuttonoriginx + 4 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos6 = new wxButton(this, window::id::BPTZPOS6, wxT("6"), wxPoint(ptzbuttonoriginx + 5 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos7 = new wxButton(this, window::id::BPTZPOS7, wxT("7"), wxPoint(ptzbuttonoriginx + 6 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos8 = new wxButton(this, window::id::BPTZPOS8, wxT("8"), wxPoint(ptzbuttonoriginx + 7 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));

    Connect(window::id::BZOOMIN, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(PTZPanel::OnZoomIn));
    Connect(window::id::BZOOMOUT, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(PTZPanel::OnZoomOut));

}

void PTZPanel::OnZoomIn(wxCommandEvent& WXUNUSED(event))
{

}

void PTZPanel::OnZoomOut(wxCommandEvent& WXUNUSED(event))
{

}

RangeFinderPanel::RangeFinderPanel(wxFrame* parent)
    : wxPanel(parent, -1, wxDefaultPosition, wxSize(300, 50))
{
    m_parent = parent;

    m_measuredistance = new wxButton(this, window::id::BMEASUREDISTANCE, wxT("Measure distance"), wxPoint(10,10));

}

VideoSetPanel::VideoSetPanel(wxFrame* parent)
    : wxPanel(parent, -1, wxDefaultPosition, wxSize(300, 200))
{
    int originx = 10;
    int originy = 10;
    int spacing = 20;
    m_parent = parent;

    m_textstream = new wxStaticText(this, -1, wxT("Video stream:"), wxPoint(originx, originy));
    m_textstream->SetForegroundColour(wxColor(214, 214, 214));

    m_zoomstream = new wxRadioButton(this, window::id::BZOOMSTREAM, wxT("Zoom camera"), wxPoint(originx, originy+spacing));
    m_zoomstream->SetForegroundColour(wxColor(214, 214, 214));
    m_nirstream = new wxRadioButton(this, window::id::BNIRSTREAM, wxT("NIR camera"), wxPoint(originx, originy + 2*spacing));
    m_nirstream->SetForegroundColour(wxColor(214, 214, 214));
    m_thermalstream = new wxRadioButton(this, window::id::BTHERMALSTREAM, wxT("LWIR camera"), wxPoint(originx, originy + 3*spacing));
    m_thermalstream->SetForegroundColour(wxColor(214, 214, 214));
    m_fusionstream = new wxRadioButton(this, window::id::BFUSIONSTREAM, wxT("Enable fusion"), wxPoint(originx, originy + 4*spacing));
    m_fusionstream->SetForegroundColour(wxColor(214, 214, 214));

    m_textpip = new wxStaticText(this, -1, wxT("Picture-In-Picture:"), wxPoint(originx + 150, originy));
    m_textpip->SetForegroundColour(wxColor(214, 214, 214));

    m_pip = new wxListBox(this, window::id::BPIP, wxPoint(originx + 150, originy + spacing), wxSize(120, 4 * spacing));
    m_pip->SetBackgroundColour(wxColor(214, 214, 214));
    m_pip->Append("Disabled");
}

FusionRatioPanel::FusionRatioPanel(wxFrame* parent)
    : wxPanel(parent, -1, wxDefaultPosition, wxSize(300, 70))
{
    m_parent = parent;

    m_textratio = new wxStaticText(this, -1, wxT("Fusion ratio:"), wxPoint(10,10));
    m_textratio->SetForegroundColour(wxColor(214, 214, 214));

    m_fusionslider = new wxSlider(this, window::id::BFUSIONRATIO, 50, 0, 100,
        wxPoint(10, 30), wxSize(280, -1));

}
