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
    : wxPanel(parent, -1, wxDefaultPosition, wxSize(100, 100))
{
    m_parent = parent;

    m_measuredistance = new wxButton(this, window::id::BMEASUREDISTANCE, wxT("Measure distance"), wxPoint(10,10));

}

VideoSetPanel::VideoSetPanel(wxFrame* parent)
    : wxPanel(parent, -1, wxDefaultPosition, wxSize(100, 100))
{
    int originx = 10;
    int originy = 10;
    int spacing = 20;
    m_parent = parent;

    m_text = new wxStaticText(this, -1, wxT("Video stream:"), wxPoint(originx, originy));
    m_text->SetForegroundColour(wxColor(214, 214, 214));

    m_zoomstream = new wxRadioButton(this, window::id::BZOOMSTREAM, wxT("Zoom camera"), wxPoint(originx, originy+spacing));
    m_zoomstream->SetForegroundColour(wxColor(214, 214, 214));
    m_nirstream = new wxRadioButton(this, window::id::BNIRSTREAM, wxT("NIR camera"), wxPoint(originx, originy + 2*spacing));
    m_nirstream->SetForegroundColour(wxColor(214, 214, 214));
    m_thermalstream = new wxRadioButton(this, window::id::BTHERMALSTREAM, wxT("LWIR camera"), wxPoint(originx, originy + 3*spacing));
    m_thermalstream->SetForegroundColour(wxColor(214, 214, 214));
    m_fusionstream = new wxRadioButton(this, window::id::BFUSIONSTREAM, wxT("Enable fusion"), wxPoint(originx, originy + 4*spacing));
    m_fusionstream->SetForegroundColour(wxColor(214, 214, 214));


}


