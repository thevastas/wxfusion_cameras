//#include "ControlPanel.h"
#include "MainWindow.h"
#include "PanTilt.h"
#include "ThermalZoom.h"

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
    m_upleft->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTUpLeft), NULL, this);
    m_upleft->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PTZPanel::OnPTStop), NULL, this);
    
    
    m_up = new wxBitmapButton(this, window::id::BUP, wxBitmap(wxT("img/arrow_up.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx + ptzbuttonsize + ptzbuttonspacing, ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_up->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTUp), NULL, this);
    m_up->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PTZPanel::OnPTStop), NULL, this);


    m_upright = new wxBitmapButton(this, window::id::BUPRIGHT, wxBitmap(wxT("img/arrow_upright.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx + 2 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_upright->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTUpRight), NULL, this);
    m_upright->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PTZPanel::OnPTStop), NULL, this);



    m_left = new wxBitmapButton(this, window::id::BLEFT, wxBitmap(wxT("img/arrow_left.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy+ptzbuttonsize+ptzbuttonspacing), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_left->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTLeft), NULL, this);
    m_left->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PTZPanel::OnPTStop), NULL, this);
    
    m_right = new wxBitmapButton(this, window::id::BRIGHT, wxBitmap(wxT("img/arrow_right.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx + 2 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + ptzbuttonsize + ptzbuttonspacing), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_right->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTRight), NULL, this);
    m_right->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PTZPanel::OnPTStop), NULL, this);


    m_downleft = new wxBitmapButton(this, window::id::BDOWNLEFT, wxBitmap(wxT("img/arrow_downleft.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy + 2 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_downleft->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTDownLeft), NULL, this);
    m_downleft->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PTZPanel::OnPTStop), NULL, this);


    m_down = new wxBitmapButton(this, window::id::BDOWN, wxBitmap(wxT("img/arrow_down.png"), wxBITMAP_TYPE_PNG),             wxPoint(ptzbuttonoriginx + ptzbuttonsize + ptzbuttonspacing, ptzbuttonoriginy + 2 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_down->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTDown), NULL, this);
    m_down->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PTZPanel::OnPTStop), NULL, this);
    
    
    
    m_downright = new wxBitmapButton(this, window::id::BDOWNRIGHT, wxBitmap(wxT("img/arrow_downright.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx + 2*(ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 2 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_downright->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTDownRight), NULL, this);
    m_downright->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PTZPanel::OnPTStop), NULL, this);



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

    m_textpos = new wxStaticText(this, -1, wxT("Position"), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy + 3.5 * (ptzbuttonsize + ptzbuttonspacing)));
    m_textpos->SetForegroundColour(wxColor(214, 214, 214));

    m_ptzspeed = new wxSpinCtrl(this, window::id::BPTZSPEED,
        wxT("3"), wxPoint(230, ptzbuttonoriginy + 2*(ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize+20, ptzbuttonsize), wxSP_ARROW_KEYS,  1, 5, 1);
    m_ptzspeed->SetBackgroundColour(wxColor(214, 214, 214));
    


    m_pos1 = new wxButton(this, window::id::BPTZPOS1, wxT("1"), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos1->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTPreset1), NULL, this);
    //m_pos1->Bind(wxEVT_BUTTON, &PTZPanel::OnPTPreset1, this);
    //Connect(wxEVT_BUTTON, wxCommandEventHandler(PTZPanel::OnPTPreset1));
        
    m_pos2 = new wxButton(this, window::id::BPTZPOS2, wxT("2"), wxPoint(ptzbuttonoriginx + ptzbuttonsize + ptzbuttonspacing, ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos2->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTPreset2), NULL, this);
    // m_pos2->Bind(wxEVT_BUTTON, &PTZPanel::OnPTPreset2, this);
        //->Connect(wxEVT_BUTTON, wxCommandEventHandler(PTZPanel::OnPTPreset2)); 
    
    m_pos3 = new wxButton(this, window::id::BPTZPOS3, wxT("3"), wxPoint(ptzbuttonoriginx + 2 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos3->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTPreset3), NULL, this);
   // m_pos3->Connect(wxEVT_BUTTON, wxCommandEventHandler(PTZPanel::OnPTPreset3));

    m_pos4 = new wxButton(this, window::id::BPTZPOS4, wxT("4"), wxPoint(ptzbuttonoriginx + 3 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos4->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTPreset4), NULL, this);
    //m_pos4->Connect(wxEVT_BUTTON, wxCommandEventHandler(PTZPanel::OnPTPreset4));

    m_pos5 = new wxButton(this, window::id::BPTZPOS5, wxT("5"), wxPoint(ptzbuttonoriginx + 4 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos5->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTPreset5), NULL, this);
    //m_pos5->Connect(wxEVT_BUTTON, wxCommandEventHandler(PTZPanel::OnPTPreset5));

    m_pos6 = new wxButton(this, window::id::BPTZPOS6, wxT("6"), wxPoint(ptzbuttonoriginx + 5 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos6->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTPreset6), NULL, this);
    //m_pos6->Connect(wxEVT_BUTTON, wxCommandEventHandler(PTZPanel::OnPTPreset6));

    m_pos7 = new wxButton(this, window::id::BPTZPOS7, wxT("7"), wxPoint(ptzbuttonoriginx + 6 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos7->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTPreset7), NULL, this);
    //m_pos7->Connect(wxEVT_BUTTON, wxCommandEventHandler(PTZPanel::OnPTPreset7));

    m_pos8 = new wxButton(this, window::id::BPTZPOS8, wxT("8"), wxPoint(ptzbuttonoriginx + 7 * (ptzbuttonsize + ptzbuttonspacing), ptzbuttonoriginy + 4 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_pos8->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTPreset8), NULL, this);
    //m_pos8->Connect(wxEVT_BUTTON, wxCommandEventHandler(PTZPanel::OnPTPreset8));

    m_preset = new wxSpinCtrl(this, window::id::BPTZSETPRESET,
        wxT("1"), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy + 5 * (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize + 10, ptzbuttonsize), wxSP_ARROW_KEYS, 1, 8, 1);
    m_preset->SetBackgroundColour(wxColor(214, 214, 214));
    
    m_savepreset = new wxButton(this, window::id::BPTZSAVEPRESET, wxT("Save preset"), wxPoint(70, ptzbuttonoriginy + 5 * (ptzbuttonsize + ptzbuttonspacing)));
    m_savepreset->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(PTZPanel::OnPTSetPreset), NULL, this);

    //m_savepreset->Connect(wxEVT_BUTTON, wxCommandEventHandler(PTZPanel::OnPTSetPreset));
    //m_savepreset->Bind(wxEVT_BUTTON, &PTZPanel::OnPTSetPreset, this);

    //m_textpreset = new wxStaticText(this, -1, wxT("Save Preset"), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy + 5 * (ptzbuttonsize + ptzbuttonspacing)));
    //m_textpreset->SetForegroundColour(wxColor(214, 214, 214));
    //Connect(window::id::BZOOMIN, wxEVT_COMMAND_BUTTON_CLICKED,
    //    wxCommandEventHandler(PTZPanel::OnZoomIn));
    //Connect(window::id::BZOOMOUT, wxEVT_COMMAND_BUTTON_CLICKED,
    //    wxCommandEventHandler(PTZPanel::OnZoomOut));
    //Connect(window::id::BUP, wxEVT_LEFT_DOWN,
    //    wxMouseEventHandler(PTZPanel::OnPTUp));
    //Connect(window::id::BUP, wxEVT_LEFT_UP,
    //    wxMouseEventHandler(PTZPanel::OnPTStop));
    //Connect(window::id::BDOWN, wxEVT_LEFT_DOWN,
    //    wxMouseEventHandler(PTZPanel::OnPTDown));
    //Connect(window::id::BDOWN, wxEVT_LEFT_UP,
    //    wxMouseEventHandler(PTZPanel::OnPTStop));

}

void PTZPanel::OnPTUp(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Up(m_ptzspeed->GetValue());
    event.Skip();
}


void PTZPanel::OnPTDown(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Down(m_ptzspeed->GetValue());
    event.Skip();
}

void PTZPanel::OnPTLeft(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Left(m_ptzspeed->GetValue());
    event.Skip();
}


void PTZPanel::OnPTRight(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Right(m_ptzspeed->GetValue());
    event.Skip();
}

void PTZPanel::OnPTUpRight(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.UpRight(m_ptzspeed->GetValue());
    event.Skip();
}

void PTZPanel::OnPTDownRight(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.DownRight(m_ptzspeed->GetValue());
    event.Skip();
}

void PTZPanel::OnPTUpLeft(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.UpLeft(m_ptzspeed->GetValue());
    event.Skip();
}

void PTZPanel::OnPTDownLeft(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.DownLeft(m_ptzspeed->GetValue());
    event.Skip();
}

void PTZPanel::OnPTStop(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Stop();
    event.Skip();
}

void PTZPanel::OnPTSetPreset(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    //m_preset->Disable();
        //->GetValue();
    pt.SetPreset(m_preset->GetValue());
        //m_preset->GetValue());
    event.Skip();
    //event.Skip();
}

void PTZPanel::OnPTPreset1(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Preset(1);
    event.Skip();
    //event.Skip();
}
void PTZPanel::OnPTPreset2(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Preset(2);
    event.Skip();
}
void PTZPanel::OnPTPreset3(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Preset(3);
    event.Skip();
}
void PTZPanel::OnPTPreset4(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Preset(4);
    event.Skip();
}
void PTZPanel::OnPTPreset5(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Preset(5);
    event.Skip();
}
void PTZPanel::OnPTPreset6(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Preset(6);
    event.Skip();
}
void PTZPanel::OnPTPreset7(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Preset(7);
    event.Skip();
}
void PTZPanel::OnPTPreset8(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Preset(8);
    event.Skip();
}
void PTZPanel::OnPTPreset9(wxMouseEvent& event)
{
    PanTilt pt("COM5", 2400);
    pt.Preset(9);
    event.Skip();
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

    //Connect(window::id::BZOOMSTREAM, wxEVT_RADIOBUTTON,
        //wxCommandEventHandler(VideoSetPanel::OnZoomStream));
    /*Connect(window::id::BNIRSTREAM, wxEVT_RADIOBUTTON,
        wxCommandEventHandler(VideoSetPanel::OnNirStream));
    Connect(window::id::BTHERMALSTREAM, wxEVT_RADIOBUTTON,
        wxCommandEventHandler(VideoSetPanel::OnThermalStream));
    Connect(window::id::BFUSIONSTREAM, wxEVT_RADIOBUTTON,
        wxCommandEventHandler(VideoSetPanel::OnFusionStream));
    Connect(window::id::BPIP, wxEVT_LISTBOX,
        wxCommandEventHandler(VideoSetPanel::OnPip));*/

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

ThermalLensPanel::ThermalLensPanel(wxPanel* parent) : wxPanel(parent, -1, wxDefaultPosition, wxSize(300, 100))
{
    //SetBackgroundColour(wxColor(32, 32, 32));
    wxImage::AddHandler(new wxPNGHandler);
    m_parent = parent;

    int ptzbuttonsize = 30;
    wxSize ptzarrowsize = wxSize(ptzbuttonsize, ptzbuttonsize);
    int ptzbuttonspacing = 5;
    int ptzbuttonoriginx = 10;
    int ptzbuttonoriginy = 10;


    m_zoomin = new wxBitmapButton(this, window::id::BTZOOMIN, wxBitmap(wxT("img/arrow_up.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_zoomin->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ThermalLensPanel::OnZoomIn), NULL, this);
    m_zoomin->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ThermalLensPanel::OnStop), NULL, this);

    m_focusin = new wxBitmapButton(this, window::id::BTFOCUSIN, wxBitmap(wxT("img/arrow_left.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx + ptzbuttonsize + ptzbuttonspacing, ptzbuttonoriginy), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_focusin->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ThermalLensPanel::OnFocusIn), NULL, this);
    m_focusin->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ThermalLensPanel::OnStop), NULL, this);



    m_zoomout = new wxBitmapButton(this, window::id::BTZOOMOUT, wxBitmap(wxT("img/arrow_down.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx, ptzbuttonoriginy +  (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_zoomout->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ThermalLensPanel::OnZoomOut), NULL, this);
    m_zoomout->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ThermalLensPanel::OnStop), NULL, this);


    m_focusout = new wxBitmapButton(this, window::id::BTFOCUSOUT, wxBitmap(wxT("img/arrow_right.png"), wxBITMAP_TYPE_PNG), wxPoint(ptzbuttonoriginx + ptzbuttonsize + ptzbuttonspacing, ptzbuttonoriginy +  (ptzbuttonsize + ptzbuttonspacing)), wxSize(ptzbuttonsize, ptzbuttonsize));
    m_focusout->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(ThermalLensPanel::OnFocusOut), NULL, this);
    m_focusout->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ThermalLensPanel::OnStop), NULL, this);

}

void ThermalLensPanel::OnStop(wxMouseEvent& event)
{
    ThermalZoom pt(m_parent, "COM4", 2400);
    pt.Stop();
    event.Skip();
}

void ThermalLensPanel::OnZoomIn(wxMouseEvent& event)
{
    ThermalZoom pt(m_parent, "COM4", 2400);
    pt.ZoomIn(5);
    //Close();
    event.Skip();
}

void ThermalLensPanel::OnZoomOut(wxMouseEvent& event)
{
    ThermalZoom pt(m_parent, "COM4", 2400);
    pt.ZoomOut(5);
    event.Skip();
}

void ThermalLensPanel::OnFocusIn(wxMouseEvent& event)
{
    ThermalZoom pt(m_parent, "COM4", 2400);
    pt.FocusIn(5);
    event.Skip();
}

void ThermalLensPanel::OnFocusOut(wxMouseEvent& event)
{
    ThermalZoom pt(m_parent, "COM4", 2400);
    pt.FocusOut(5);
    event.Skip();
}