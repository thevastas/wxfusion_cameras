#pragma once
#include "Common.h"
#include "ControlPanel.h"
#include "wx/artprov.h"
#include "id.h"
#include "ThermalCam.h"
#include "NIRCam.h"
#include "Fusion.h"

#include <wx/stdpaths.h>
#include <wx/config.h>
#include <wx/confbase.h>
#include <wx/fileconf.h>
//#include "bmpfromocvpanel.h"
//#include "View.h"

class wxBitmapFromOpenCVPanel;

namespace cv
{
    class Mat;
    class VideoCapture;
}


class CameraThread;
class LWIRCameraThread;
class NIRCameraThread;
class FusionCameraThread;
class Scripter;



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
//public:
//    MainWindow();
//    ~MainWindow();

    PTZPanel *m_ptzpanel;
    RangeFinderPanel* m_rfpanel;
    VideoSetPanel* m_videopanel;
    FusionRatioPanel* m_fusionratiopanel;
    LogPanel* m_logpanel;
    ThermalLensPanel* m_thermalpanel;
    wxPanel* m_parent;
    // file functions

    wxMenu* fileMenu;
    wxMenu* rangefinderMenu;
    wxMenu* optionsMenu;
    wxMenu* cameraMenu;
    wxMenu* viewMenu;
    
    void OnOpen(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    // view functions
    void OnNirPoi(wxCommandEvent& event);
    void OnThermalPoi(wxCommandEvent& event);
    void OnCrosshair(wxCommandEvent& event);
    void OnDCCameras(wxCommandEvent& event);
    // rangefinder functions
    void OnRFMeasure(wxCommandEvent& event);
    void OnRFPointerOn(wxCommandEvent& event);
    void OnRFPointerOff(wxCommandEvent& event);
    //void RFThread();
    // camera functions
    void OnConnectZoom(wxCommandEvent& event);
    void InitializeCameras(wxCommandEvent& event);
    void OnKeyDown(wxKeyEvent& event);
    // options functions
    ThermalCam lwir;
    NIRCam nir;
    Fusion fusion;
    bool m_isInitialized = false;
    bool m_onlyZoom = false;
    cv::UMat m_ocvmat;
    wxString address = "rtsp://192.168.30.168/main";
    // pan tilt    
    void OnQueryPan(wxCommandEvent& event);
    void OnQueryTilt(wxCommandEvent& event);
    enum Mode //for the future cameras
    {
        Empty,
            Image,
            Video,
            WebCam,
            IPCamera,
            LWIRCamera,
            NIRCamera,
            FuseNIRLWIR
    };

    Mode                     m_mode{ Empty };
    Mode                     current_mode{ Empty };

//private:

    wxString                 m_sourceName;
    int                      m_currentVideoFrameNumber{ 0 };

    cv::VideoCapture* m_videoCapture{ nullptr };
    CameraThread* m_cameraThread{ nullptr };

    LWIRCameraThread* m_lwircameraThread{ nullptr };
    HANDLE m_lwirhandle;

    NIRCameraThread* m_nircameraThread{ nullptr };
    std::shared_ptr<peak::core::DataStream> m_dataStream{ nullptr };

    FusionCameraThread* m_fusioncameraThread{ nullptr };
    


    Scripter* m_scripterThread{ nullptr };


    wxBitmapFromOpenCVPanel* m_bitmapPanel;
    wxSlider* m_videoSlider;
    wxButton* m_propertiesButton;


    wxString m_rfport = "COM6";
    wxString m_thermalzoomport = "COM4";
    wxString m_pantiltport = "COM5";
    int m_fusionoffsetx = 0;
    int m_fusionoffsety = 0;

    bool m_crosshair = false;

    static wxBitmap ConvertMatToBitmap(const cv::UMat matBitmap, long& timeConvert);
    //cv::Mat m_ocvbitmap;

    void Clear();

    //wxFileConfig* ConfigINI = new wxFileConfig(wxEmptyString, wxEmptyString, wxEmptyString, "C:/Users/PANTILT/source/repos/wxfusion_cameras/settings.ini");
        wxFileConfig* ConfigINI = new wxFileConfig(wxEmptyString, wxEmptyString, wxEmptyString, wxGetCwd() + "\\settings.ini");

    wxString group;
    
    // If address is empty, the default webcam is used.
    // resolution and useMJPEG are used only for webcam.
    bool StartIPCameraCapture(wxString& address,
        const wxSize& resolution = wxSize(),
        bool useMJPEG = false);
    bool StartIPCameraThread();
    void DeleteIPCameraThread();
    void OnIPCamera(wxCommandEvent&);
    void OnCameraFrame(wxThreadEvent& evt);

    void OnLWIRCamera(wxCommandEvent&);
    bool StartLWIRCameraCapture(HANDLE handle);
    bool StartLWIRCameraThread();
    void DeleteLWIRCameraThread();
    void OnLWIRCameraFrame(wxThreadEvent& evt);

    void OnNIRCamera(wxCommandEvent&);
    bool StartNIRCameraCapture();
    bool StartNIRCameraThread();
    void DeleteNIRCameraThread();
    void OnNIRCameraFrame(wxThreadEvent& evt);

    void OnFusionCamera(wxCommandEvent&);
    bool StartFusionCameraCapture();
    bool StartFusionCameraThread();
    void DeleteFusionCameraThread();
    void OnFusionCameraFrame(wxThreadEvent& evt);

    
    void OnClear(wxCommandEvent&);
    void OnStreamInfo(wxCommandEvent&);
    
    void OnCameraEmpty(wxThreadEvent&);
    void OnCameraException(wxThreadEvent& evt);

    void ReadSettings(wxCommandEvent& event);
    void SaveSettings(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

