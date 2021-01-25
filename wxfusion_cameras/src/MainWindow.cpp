#include "MainWindow.h"


BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_EXIT, MainWindow::OnQuit)
    EVT_MENU(window::id::RFPOION, MainWindow::OnRFPointerOn)
    EVT_MENU(window::id::RFPOIOFF, MainWindow::OnRFPointerOff)
    EVT_MENU(window::id::RFMEASURE, MainWindow::OnRFMeasure)
    EVT_MENU(window::id::ENABLEZOOMCAMERA, MainWindow::OnConnectZoom)
    EVT_MENU(window::id::THERMALPOI, MainWindow::OnThermalPoi)
    EVT_MENU(window::id::NIRPOI, MainWindow::OnNirPoi)
    EVT_MENU(window::id::CROSSHAIR, MainWindow::OnCrosshair)
END_EVENT_TABLE()

MainWindow::MainWindow(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name):

    wxFrame(parent, id, title, pos, size, style, name)
{
    m_parent = new wxPanel(this, wxID_ANY);

    SetBackgroundColour(wxColor(32,32,32));
    wxMenuBar* menuBar = new wxMenuBar();


    //FILE MENU
    wxMenu* fileMenu = new wxMenu();
    menuBar->Append(fileMenu, _("&File"));
    fileMenu->Append(wxID_NEW);
    wxMenuItem* quitItem = new wxMenuItem(fileMenu, wxID_EXIT);
    quitItem->SetBitmap(wxArtProvider::GetBitmap("wxART_QUIT"));
    fileMenu->Append(quitItem);


    // rangefinder MENU
    wxMenu* rangefinderMenu = new wxMenu();
    menuBar->Append(rangefinderMenu, _("&Rangefinder"));
    rangefinderMenu->Append(window::id::RFPOION, _("Pointer ON"));
    rangefinderMenu->Append(window::id::RFPOIOFF, _("Pointer OFF"));
    rangefinderMenu->Append(window::id::RFMEASURE, _("Measure"));

    // camera MENU
    wxMenu* cameraMenu = new wxMenu();
    menuBar->Append(cameraMenu, _("&Camera"));
    cameraMenu->Append(window::id::ENABLEZOOMCAMERA, "Enable zoom camera");

    // VIEW MENU
    wxMenu* viewMenu = new wxMenu();
    menuBar->Append(viewMenu, _("&View"));
    viewMenu->Append(window::id::THERMALPOI,"Enable thermal POI");
    viewMenu->Append(window::id::NIRPOI, "Enable NIR POI");
    viewMenu->Append(window::id::CROSSHAIR, "Enable crosshair");

    // OPTIONS MENU
    wxMenu* optionsMenu = new wxMenu();
    menuBar->Append(optionsMenu, _("&Options"));
    optionsMenu->Append(window::id::CONNSETTINGS, "Connection settings");
        
    SetMenuBar(menuBar);

    




    wxPanel* panel_image = new wxPanel(m_parent, wxID_ANY, wxDefaultPosition, wxSize(960,540));
    panel_image->SetBackgroundColour(wxColor(64,64,64));





    wxPanel* panel_log = new wxPanel(m_parent, wxID_ANY, wxDefaultPosition, wxSize(800, 100));
    panel_log->SetBackgroundColour(wxColor(64, 64, 64));

    //wxPanel* panel_controls = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(300, 700));
    //panel_controls->SetBackgroundColour(wxColor(200, 100, 100));


    m_rfpanel = new RangeFinderPanel(m_parent);
    m_rfpanel->SetBackgroundColour(wxColor(64, 64, 64));

    m_ptzpanel = new PTZPanel(m_parent);
    m_ptzpanel->SetBackgroundColour(wxColor(64, 64, 64));

    m_videopanel = new VideoSetPanel(m_parent);
    m_videopanel->SetBackgroundColour(wxColor(64, 64, 64));

    m_fusionratiopanel = new FusionRatioPanel(m_parent);
    m_fusionratiopanel->SetBackgroundColour(wxColor(64, 64, 64));

    wxSizer* sizer_controls = new wxBoxSizer(wxVERTICAL);
    sizer_controls->Add(m_videopanel, 0, wxEXPAND | wxBOTTOM, 5);
    sizer_controls->Add(m_fusionratiopanel, 0, wxEXPAND | wxBOTTOM, 5);
    sizer_controls->Add(m_rfpanel, 0, wxEXPAND | wxBOTTOM, 5);
    sizer_controls->Add(m_ptzpanel, 1, wxEXPAND);


    wxSizer* sizer_top = new wxBoxSizer(wxHORIZONTAL);
    sizer_top->Add(panel_image, 0, wxEXPAND | wxRIGHT, 5);
    sizer_top->Add(sizer_controls, 0, wxEXPAND);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(sizer_top, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    sizer->Add(panel_log, 1, wxEXPAND | wxALL, 5);

    //wxSizer* sizer_buttons = new wxBoxSizer(wxVERTICAL);
    //sizer_buttons->Add(
    //    new wxButton(this, wxID_ANY, "Button 1"),
    //    0, wxALL, 10);
    //sizer_buttons->Add(
    //    new wxButton(this, wxID_ANY, "Button 2"),
    //    0, wxALL, 10);
    //sizer_buttons->Add(
    //    new wxButton(this, wxID_ANY, "Button 3"),
    //    0, wxALL, 10);

    //panel_controls->SetSizerAndFit(sizer_buttons);

    m_parent->SetSizerAndFit(sizer);
}

// file functions
void MainWindow::OnConnectZoom(wxCommandEvent& event) {
    wxMessageBox("Zoom camera connected");
}

void MainWindow::OnQuit(wxCommandEvent& event) {
    Close();
}

// view functions
void MainWindow::OnNirPoi(wxCommandEvent& event)
{
    wxMessageBox("NIR POI not implemented");
};
void MainWindow::OnThermalPoi(wxCommandEvent& event)
{
    wxMessageBox("Thermal POI not implemented");
};
void MainWindow::OnCrosshair(wxCommandEvent& event)
{
    wxMessageBox("Crosshair not implemented");
};
// rangefinder functions
void MainWindow::OnRFMeasure(wxCommandEvent& event)
{
    wxMessageBox("Rangefinder measurement not implemented");
};
void MainWindow::OnRFPointerOn(wxCommandEvent& event)
{
    wxMessageBox("Rangefinder pointer ON not implemented");
};
void MainWindow::OnRFPointerOff(wxCommandEvent& event)
{
    wxMessageBox("Rangefinder pointer OFF not implemented");
};

MainWindow::~MainWindow()
{

}

