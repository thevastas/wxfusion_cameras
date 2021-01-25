#pragma once
#include "Common.h"
#include "ControlPanel.h"
#include "wx/artprov.h"
#include "id.h"
class MainWindow: public wxFrame
{
public:
	MainWindow(wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxASCII_STR(wxFrameNameStr));
	~MainWindow();

    PTZPanel *m_ptzpanel;
    RangeFinderPanel* m_rfpanel;
    VideoSetPanel* m_videopanel;
    FusionRatioPanel* m_fusionratiopanel;
    LogPanel* m_logpanel;
    wxPanel* m_parent;

    // file functions
    void OnQuit(wxCommandEvent& event);
    // view functions
    void OnNirPoi(wxCommandEvent& event);
    void OnThermalPoi(wxCommandEvent& event);
    void OnCrosshair(wxCommandEvent& event);
    // rangefinder functions
    void OnRFMeasure(wxCommandEvent& event);
    void OnRFPointerOn(wxCommandEvent& event);
    void OnRFPointerOff(wxCommandEvent& event);
    // camera functions
    void OnConnectZoom(wxCommandEvent& event);
    // options functions



    

    DECLARE_EVENT_TABLE()
};

