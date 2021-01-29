//#include "ControlPanel.h"
#include "MainWindow.h"

//ControlPanel::ControlPanel(wxPanel* parent)
PTZPanel::PTZPanel(wxPanel* parent)
	: wxPanel(parent, -1, wxDefaultPosition, wxSize(300, 200))
{
    wxImage::AddHandler(new wxPNGHandler);
    m_parent = parent;
    
    // Pan tilt buttons
    int ptzbuttonsize = 30;
    wxSize ptzarrowsize = wxSize(ptzbuttonsize, ptzbuttonsize);
    int ptzbuttonspacing = 5;
    int ptzbuttonoriginx = 10;
    int ptzbuttonoriginy = 10;


    m_upleft = new wxBitmapButton(this, window::id::BUPLEFT, wxBitmap(wxT("img/arrow_upleft.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_up = new wxBitmapButton(this, window::id::BUP, wxBitmap(wxT("img/arrow_up.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx + ptzbuttonsize + ptzbuttonspacing, ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_upright = new wxBitmapButton(this, window::id::BUPRIGHT, wxBitmap(wxT("img/arrow_upright.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx + 2 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));

    m_left = new wxBitmapButton(this, window::id::BLEFT, wxBitmap(wxT("img/arrow_left.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy+ptzbuttonsize+ptzbuttonspacing), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_right = new wxBitmapButton(this, window::id::BRIGHT, wxBitmap(wxT("img/arrow_right.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx + 2 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + ptzbuttonsize + ptzbuttonspacing), wxSize(ptzbuttonsize, ptzbuttonsize));

    m_downleft = new wxBitmapButton(this, window::id::BDOWNLEFT, wxBitmap(wxT("img/arrow_downleft.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy + 2 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_down = new wxBitmapButton(this, window::id::BDOWN, wxBitmap(wxT("img/arrow_down.png"), wxBITMAP_TYPE_PNG),             wxPoint(ptzbuttonoriginx + ptzbuttonsize + ptzbuttonspacing, ptzbuttonoriginy + 2 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_downright = new wxBitmapButton(this, window::id::BDOWNRIGHT, wxBitmap(wxT("img/arrow_downright.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx + 2*(ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 2 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));

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

RangeFinderPanel::RangeFinderPanel(wxPanel* parent)
    : wxPanel(parent, -1, wxDefaultPosition, wxSize(300, 50))
{
    m_parent = parent;

    m_measuredistance = new wxButton(this, window::id::BMEASUREDISTANCE, wxT("Measure distance"), wxPoint(10,10));

}

VideoSetPanel::VideoSetPanel(wxPanel* parent)
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
    m_fusionstream = new wxRadioButton(this, window::id::BFUSIONSTREAM, wxT("Fusion"), wxPoint(originx, originy + 4*spacing));
    m_fusionstream->SetForegroundColour(wxColor(214, 214, 214));





    m_textpip = new wxStaticText(this, -1, wxT("Picture-In-Picture:"), wxPoint(originx + 150, originy));
    m_textpip->SetForegroundColour(wxColor(214, 214, 214));


    m_pip = new wxListBox(this, window::id::BPIP, wxPoint(originx + 150, originy + spacing), wxSize(120, 4 * spacing));
    m_pip->SetBackgroundColour(wxColor(214, 214, 214));
    m_pip->Append("Disabled");
    m_pip->SetSelection(m_pip->FindString("Disabled"));

    Connect(window::id::BZOOMSTREAM, wxEVT_RADIOBUTTON,
        wxCommandEventHandler(VideoSetPanel::OnZoomStream));
    Connect(window::id::BNIRSTREAM, wxEVT_RADIOBUTTON,
        wxCommandEventHandler(VideoSetPanel::OnNirStream));
    Connect(window::id::BTHERMALSTREAM, wxEVT_RADIOBUTTON,
        wxCommandEventHandler(VideoSetPanel::OnThermalStream));
    Connect(window::id::BFUSIONSTREAM, wxEVT_RADIOBUTTON,
        wxCommandEventHandler(VideoSetPanel::OnFusionStream));
    Connect(window::id::BPIP, wxEVT_LISTBOX,
        wxCommandEventHandler(VideoSetPanel::OnPip));

}

void VideoSetPanel::OnZoomStream(wxCommandEvent& WXUNUSED(event))
{
    wxArrayString strings;
    strings.push_back("NIR");
    strings.push_back("LWIR");
    strings.push_back("Fusion");
    strings.push_back("Disabled");
    m_pip->Set(strings);
}



void VideoSetPanel::OnNirStream(wxCommandEvent& WXUNUSED(event))
{
    wxArrayString strings;
    strings.push_back("Zoom");
    strings.push_back("LWIR");
    strings.push_back("Fusion");
    strings.push_back("Disabled");
    m_pip->Set(strings);
}

void VideoSetPanel::OnThermalStream(wxCommandEvent& WXUNUSED(event))
{
    wxArrayString strings;
    strings.push_back("NIR");
    strings.push_back("Zoom");
    strings.push_back("Fusion");
    strings.push_back("Disabled");
    m_pip->Set(strings);
}

void VideoSetPanel::OnFusionStream(wxCommandEvent& WXUNUSED(event))
{
    wxArrayString strings;
    strings.push_back("NIR");
    strings.push_back("LWIR");
    strings.push_back("Zoom");
    strings.push_back("Disabled");
    m_pip->Set(strings);
}

void VideoSetPanel::OnPip(wxCommandEvent& WXUNUSED(event))
{
    MainWindow* comm = (MainWindow*)m_parent->GetParent();

    wxString m_pipchoice = m_pip->GetString(m_pip->GetSelection());
    if (m_pipchoice == wxT("Fusion")) {
        comm->m_fusionratiopanel->m_fusionslider->Enable();
        comm->m_logpanel->m_logtext->AppendText("Enabled fusion mode\n");
    }
    else if (m_fusionstream->GetValue() == FALSE)  comm->m_fusionratiopanel->m_fusionslider->Disable();
    else if (m_fusionstream->GetValue() == TRUE)  comm->m_fusionratiopanel->m_fusionslider->Enable();
    if (m_pipchoice == wxT("Zoom")) {
    }

    if (m_pipchoice == wxT("NIR")) {
    }

    if (m_pipchoice == wxT("LWIR")) {
    }

    if (m_pipchoice == wxT("Disabled")) {
    }
}


FusionRatioPanel::FusionRatioPanel(wxPanel* parent)
    : wxPanel(parent, -1, wxDefaultPosition, wxSize(300, 70))
{
    m_parent = parent;

    m_textratio = new wxStaticText(this, -1, wxT("Fusion ratio:"), wxPoint(10,10));
    m_textratio->SetForegroundColour(wxColor(214, 214, 214));

    m_fusionslider = new wxSlider(this, window::id::BFUSIONRATIO, 50, 0, 100,
        wxPoint(10, 30), wxSize(280, -1));
    m_fusionslider->Disable();

}



LogPanel::LogPanel(wxPanel* parent)
    : wxPanel(parent, -1, wxDefaultPosition, wxSize(1100, 100))
{

    m_parent = parent;
    m_logtext = new wxTextCtrl(this,window::id::LOGTEXT, wxT("Welcome!\n"), wxDefaultPosition, wxSize(1300, 100), wxBORDER_NONE | wxTE_MULTILINE);
    //m_logtext = new wxTextCtrl(this, window::id::LOGTEXT, wxT("Welcome!\n"), wxDefaultPosition, wxSize(800, 100), wxTE_MULTILINE);

    m_logtext->SetBackgroundColour(wxColor(64, 64, 64));
    m_logtext->SetForegroundColour(wxColor(214, 214, 214));

    wxBoxSizer* sizer_log = new wxBoxSizer(wxHORIZONTAL);
    sizer_log->Add(m_logtext, 0, wxEXPAND);
    this->SetSizerAndFit(sizer_log);
}