#pragma once
#include "Common.h"
#include "ControlPanel.h"
#include "wx/artprov.h"
#include "id.h"
//#include "bmpfromocvpanel.h"
//#include "View.h"

class wxBitmapFromOpenCVPanel;

namespace cv
{
    class Mat;
    class VideoCapture;
}


class CameraThread;

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
    wxPanel* m_parent;
    // file functions

    wxMenu* optionsMenu;
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

private:
    enum Mode //for the future cameras
    {
        Empty,
        Image,
        Video,
        WebCam,
        IPCamera,
    };

    Mode                     m_mode{ Empty };
    wxString                 m_sourceName;
    int                      m_currentVideoFrameNumber{ 0 };

    cv::VideoCapture* m_videoCapture{ nullptr };
    CameraThread* m_cameraThread{ nullptr };
    
    wxBitmapFromOpenCVPanel* m_bitmapPanel;
    wxSlider* m_videoSlider;
    wxButton* m_propertiesButton;

    static wxBitmap ConvertMatToBitmap(const cv::Mat& matBitmap, long& timeConvert);

    void Clear();

    // If address is empty, the default webcam is used.
    // resolution and useMJPEG are used only for webcam.
    bool StartCameraCapture(const wxString& address,
        const wxSize& resolution = wxSize(),
        bool useMJPEG = false);
    bool StartCameraThread();
    void DeleteCameraThread();

    void OnIPCamera(wxCommandEvent&);
    void OnClear(wxCommandEvent&);

    void OnStreamInfo(wxCommandEvent&);


    void OnCameraFrame(wxThreadEvent& evt);
    void OnCameraEmpty(wxThreadEvent&);
    void OnCameraException(wxThreadEvent& evt);

    

    DECLARE_EVENT_TABLE()
};

