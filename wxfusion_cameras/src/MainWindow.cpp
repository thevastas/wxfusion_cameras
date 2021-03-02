#include "Common.h"
#include <wx/wx.h>
#include <wx/choicdlg.h>
#include <wx/filedlg.h>
#include <wx/listctrl.h>
#include <wx/slider.h>
#include <wx/textdlg.h>
#include <wx/thread.h>
#include <wx/utils.h>

#include <peak/peak.hpp>
#include <peak_ipl/peak_ipl.hpp>
#include <peak/converters/peak_buffer_converter_ipl.hpp>

#include "NIRCam.h"
#include "bmpfromocvpanel.h"
#include "convertmattowxbmp.h"
#include "MainWindow.h"
//#include "Fusion.h"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_EXIT, MainWindow::OnQuit)
    EVT_MENU(window::id::RFPOION, MainWindow::OnRFPointerOn)
    EVT_MENU(window::id::RFPOIOFF, MainWindow::OnRFPointerOff)
    EVT_MENU(window::id::RFMEASURE, MainWindow::OnRFMeasure)
    EVT_MENU(window::id::CAMERAINIT, MainWindow::InitializeCameras)
    EVT_MENU(window::id::ENABLEZOOMCAMERA, MainWindow::OnIPCamera)
    EVT_MENU(window::id::ENABLELWIRCAMERA, MainWindow::OnLWIRCamera)
    EVT_MENU(window::id::ENABLENIRCAMERA, MainWindow::OnNIRCamera)
    EVT_MENU(window::id::ENABLEFUSIONCAMERA, MainWindow::OnFusionCamera)
    EVT_MENU(window::id::THERMALPOI, MainWindow::OnThermalPoi)
    EVT_MENU(window::id::NIRPOI, MainWindow::OnNirPoi)
    EVT_MENU(window::id::STREAMINFO, MainWindow::OnStreamInfo)
    EVT_MENU(window::id::CROSSHAIR, MainWindow::OnCrosshair)

END_EVENT_TABLE()

// A frame was retrieved from WebCam or IP Camera.
wxDEFINE_EVENT(wxEVT_IPCAMERA_FRAME, wxThreadEvent);
// Could not retrieve a frame, consider connection to the camera lost.
wxDEFINE_EVENT(wxEVT_IPCAMERA_EMPTY, wxThreadEvent);
// An exception was thrown in the camera thread.
wxDEFINE_EVENT(wxEVT_IPCAMERA_EXCEPTION, wxThreadEvent);


// The same for LWIR camera
wxDEFINE_EVENT(wxEVT_LWIRCAMERA_FRAME, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_LWIRCAMERA_EMPTY, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_LWIRCAMERA_EXCEPTION, wxThreadEvent);

// The same for NIR camera
wxDEFINE_EVENT(wxEVT_NIRCAMERA_FRAME, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_NIRCAMERA_EMPTY, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_NIRCAMERA_EXCEPTION, wxThreadEvent);

// The same for fusion mode
wxDEFINE_EVENT(wxEVT_FUSIONCAMERA_FRAME, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_FUSIONCAMERA_EMPTY, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_FUSIONCAMERA_EXCEPTION, wxThreadEvent);



//
// Worker thread for retrieving images from WebCam or IP Camera
// and sending them to the main thread for display.
class CameraThread : public wxThread
{
public:
    struct CameraFrame
    {
        cv::Mat matBitmap;
        long    timeGet{ 0 };
    };

    CameraThread(wxEvtHandler* eventSink, cv::VideoCapture* camera);

protected:
    wxEvtHandler* m_eventSink{ nullptr };
    cv::VideoCapture* m_camera{ nullptr };

    ExitCode Entry() override;
};

CameraThread::CameraThread(wxEvtHandler* eventSink, cv::VideoCapture* camera)
    : wxThread(wxTHREAD_JOINABLE),
    m_eventSink(eventSink), m_camera(camera)
{
    wxASSERT(m_eventSink);
    wxASSERT(m_camera);
}

wxThread::ExitCode CameraThread::Entry()
{
    wxStopWatch  stopWatch;

    while (!TestDestroy())
    {
        CameraFrame* frame = nullptr;

        try
        {
            frame = new CameraFrame;
            stopWatch.Start();
            (*m_camera) >> frame->matBitmap; //retrieve frame from capture
            frame->timeGet = stopWatch.Time(); //measure retrieval time

            if (!frame->matBitmap.empty()) //if successful, set payload
            {
                wxThreadEvent* evt = new wxThreadEvent(wxEVT_IPCAMERA_FRAME);

                evt->SetPayload(frame);
                m_eventSink->QueueEvent(evt);
            }
            else // connection to camera lost
            {
                m_eventSink->QueueEvent(new wxThreadEvent(wxEVT_IPCAMERA_EMPTY));
                wxDELETE(frame);
                break;
            }
        }
        catch (const std::exception& e)
        {
            wxThreadEvent* evt = new wxThreadEvent(wxEVT_IPCAMERA_EXCEPTION);

            wxDELETE(frame);
            evt->SetString(e.what());
            m_eventSink->QueueEvent(evt);
            break;
        }
        catch (...)
        {
            wxThreadEvent* evt = new wxThreadEvent(wxEVT_IPCAMERA_EXCEPTION);

            wxDELETE(frame);
            evt->SetString("Unknown exception");
            m_eventSink->QueueEvent(evt);
            break;

        }


    }

    return static_cast<wxThread::ExitCode>(nullptr);
}


//--------------------------------------------------------

class LWIRCameraThread : public wxThread
{
public:
    struct CameraFrame
    {
        cv::Mat matBitmap;
        long    timeGet{ 0 };
    };
    ThermalCam lwirt;
    LWIRCameraThread(wxEvtHandler* eventSink, HANDLE handle);

protected:
    wxEvtHandler* m_eventSink{ nullptr };
    HANDLE m_lwircamera { nullptr };

    ExitCode Entry() override;
};

LWIRCameraThread::LWIRCameraThread(wxEvtHandler* eventSink, HANDLE handle)
    : wxThread(wxTHREAD_JOINABLE),
    m_eventSink(eventSink), m_lwircamera(handle)
{
    
    wxASSERT(m_eventSink);
    //wxASSERT(m_lwircamera);
}

wxThread::ExitCode LWIRCameraThread::Entry()
{
    wxStopWatch  stopWatch;

    while (!TestDestroy())
    {
        CameraFrame* frame = nullptr;

        try
        {
            frame = new CameraFrame;
            stopWatch.Start();
            frame->matBitmap = lwirt.GetFrame(m_lwircamera);
            //cv::cvtColor(frame->matBitmap, frame->matBitmap, cv::COLOR_GRAY2BGR);
            //Proxy640USB_GetImage(m_lwircamera, frame->matBitmap, frame->meta, 1);
            //(*m_lwircamera) >> frame->matBitmap; //retrieve frame from capture
            //thermal.getframe
            frame->timeGet = stopWatch.Time(); //measure retrieval time

                //wxThreadEvent* evt = new wxThreadEvent(wxEVT_LWIRCAMERA_FRAME);

                //evt->SetPayload(frame);
                //m_eventSink->QueueEvent(evt);

            //WARNING
            if (!frame->matBitmap.empty()) //if successful, set payload
            {
                wxThreadEvent* evt = new wxThreadEvent(wxEVT_LWIRCAMERA_FRAME);

                evt->SetPayload(frame);
                m_eventSink->QueueEvent(evt);
            }
            else // connection to camera lost
            {
                m_eventSink->QueueEvent(new wxThreadEvent(wxEVT_IPCAMERA_EMPTY));
                wxDELETE(frame);
                break;
            }
            //WARNING
        }
        catch (const std::exception& e)
        {
            wxThreadEvent* evt = new wxThreadEvent(wxEVT_LWIRCAMERA_EXCEPTION);

            wxDELETE(frame);
            evt->SetString(e.what());
            m_eventSink->QueueEvent(evt);
            break;
        }
        catch (...)
        {
            wxThreadEvent* evt = new wxThreadEvent(wxEVT_LWIRCAMERA_EXCEPTION);

            wxDELETE(frame);
            evt->SetString("Unknown exception");
            m_eventSink->QueueEvent(evt);
            break;

        }


    }

    return static_cast<wxThread::ExitCode>(nullptr);
}

//--------------------------------------------------------

class NIRCameraThread : public wxThread
{
public:
    struct CameraFrame
    {
        cv::Mat matBitmap;
        long    timeGet{ 0 };
    };
    NIRCam nirt;
    NIRCameraThread(wxEvtHandler* eventSink, std::shared_ptr<peak::core::DataStream> dataStream);

protected:
    wxEvtHandler* m_eventSink{ nullptr };
    std::shared_ptr<peak::core::DataStream> m_dataStream{ nullptr };

    ExitCode Entry() override;
};

NIRCameraThread::NIRCameraThread(wxEvtHandler* eventSink, std::shared_ptr<peak::core::DataStream> dataStream)
    : wxThread(wxTHREAD_JOINABLE),
    m_eventSink(eventSink), m_dataStream(dataStream)
{

    //wxASSERT(m_eventSink);
    //wxASSERT(m_lwircamera);
}

wxThread::ExitCode NIRCameraThread::Entry()
{
    wxStopWatch  stopWatch;

    while (!TestDestroy())
    {
        CameraFrame* frame = nullptr;

        try
        {
            frame = new CameraFrame;
            stopWatch.Start();
            frame->matBitmap = nirt.GetFrame(true, m_dataStream);
            

            frame->timeGet = stopWatch.Time(); //measure retrieval time

            //WARNING
            if (!frame->matBitmap.empty()) //if successful, set payload
            {
                wxThreadEvent* evt = new wxThreadEvent(wxEVT_NIRCAMERA_FRAME);

                evt->SetPayload(frame);
                m_eventSink->QueueEvent(evt);
            }
            else // connection to camera lost
            {
                m_eventSink->QueueEvent(new wxThreadEvent(wxEVT_NIRCAMERA_EMPTY));
                wxDELETE(frame);
                break;
            }
            //WARNING
        }
        catch (const std::exception& e)
        {
            wxThreadEvent* evt = new wxThreadEvent(wxEVT_NIRCAMERA_EXCEPTION);

            wxDELETE(frame);
            evt->SetString(e.what());
            m_eventSink->QueueEvent(evt);
            break;
        }
        catch (...)
        {
            wxThreadEvent* evt = new wxThreadEvent(wxEVT_NIRCAMERA_EXCEPTION);

            wxDELETE(frame);
            evt->SetString("Unknown exception");
            m_eventSink->QueueEvent(evt);
            break;

        }


    }

    return static_cast<wxThread::ExitCode>(nullptr);
}


//--------------------------------------------------------

class FusionCameraThread : public wxThread
{
public:
    struct CameraFrame
    {
        cv::Mat matNirBitmap;
        cv::Mat matLwirBitmap;
        long    timeGet{ 0 };
    };
    NIRCam nirt;
    ThermalCam lwirt;
    FusionCameraThread(wxEvtHandler* eventSink, std::shared_ptr<peak::core::DataStream> dataStream, HANDLE handle);

protected:
    wxEvtHandler* m_eventSink{ nullptr };
    std::shared_ptr<peak::core::DataStream> m_dataStream{ nullptr };
    HANDLE m_lwircamera{ nullptr };
    ExitCode Entry() override;
};

FusionCameraThread::FusionCameraThread(wxEvtHandler* eventSink, std::shared_ptr<peak::core::DataStream> dataStream, HANDLE handle)
    : wxThread(wxTHREAD_JOINABLE),
    m_eventSink(eventSink), m_dataStream(dataStream)
{

    //wxASSERT(m_eventSink);
    //wxASSERT(m_lwircamera);
}

wxThread::ExitCode FusionCameraThread::Entry()
{
    wxStopWatch  stopWatch;

    while (!TestDestroy())
    {
        CameraFrame* frame = nullptr;

        try
        {
            frame = new CameraFrame;
            stopWatch.Start();
            frame->matNirBitmap = nirt.GetFrame(true, m_dataStream);
            frame->matLwirBitmap = lwirt.GetFrame(m_lwircamera);

            frame->timeGet = stopWatch.Time(); //measure retrieval time

            //WARNING
            if (!frame->matNirBitmap.empty() || !frame->matLwirBitmap.empty()) //if successful, set payload
            {
                wxThreadEvent* evt = new wxThreadEvent(wxEVT_FUSIONCAMERA_FRAME);

                evt->SetPayload(frame);
                m_eventSink->QueueEvent(evt);
            }
            else // connection to camera lost
            {
                m_eventSink->QueueEvent(new wxThreadEvent(wxEVT_FUSIONCAMERA_EMPTY));
                wxDELETE(frame);
                break;
            }
            //WARNING
        }
        catch (const std::exception& e)
        {
            wxThreadEvent* evt = new wxThreadEvent(wxEVT_FUSIONCAMERA_EXCEPTION);

            wxDELETE(frame);
            evt->SetString(e.what());
            m_eventSink->QueueEvent(evt);
            break;
        }
        catch (...)
        {
            wxThreadEvent* evt = new wxThreadEvent(wxEVT_FUSIONCAMERA_EXCEPTION);

            wxDELETE(frame);
            evt->SetString("Unknown exception");
            m_eventSink->QueueEvent(evt);
            break;

        }


    }

    return static_cast<wxThread::ExitCode>(nullptr);
}





//--------------------------------------------------------
MainWindow::MainWindow(wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name):

    wxFrame(parent, id, title, pos, size, style, name)
{
    //cv::setBreakOnError(true);
    //warning
    Fusion fusiont;
    //main panel
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
    cameraMenu->Append(window::id::CAMERAINIT, "Initialize cameras");
    cameraMenu->Append(window::id::ENABLEZOOMCAMERA, "Enable zoom camera");
    cameraMenu->Append(window::id::ENABLELWIRCAMERA, "Enable LWIR camera");
    cameraMenu->Append(window::id::ENABLENIRCAMERA, "Enable NIR camera");
    cameraMenu->Append(window::id::ENABLEFUSIONCAMERA, "Enable fusion camera");

    // VIEW MENU
    wxMenu* viewMenu = new wxMenu();
    menuBar->Append(viewMenu, _("&View"));
    viewMenu->Append(window::id::THERMALPOI,"Enable thermal POI");
    viewMenu->Append(window::id::NIRPOI, "Enable NIR POI");
    viewMenu->Append(window::id::CROSSHAIR, "Enable crosshair");

    // OPTIONS MENU
    optionsMenu = new wxMenu();
    menuBar->Append(optionsMenu, _("&Options"));
    optionsMenu->Append(window::id::CONNSETTINGS, "Connection settings");
    optionsMenu->Append(window::id::STREAMINFO, "Stream information");
        
    SetMenuBar(menuBar);

    m_bitmapPanel = new wxBitmapFromOpenCVPanel(m_parent);

    m_logpanel = new LogPanel(m_parent);
    m_logpanel->SetBackgroundColour(wxColor(64, 64, 64));

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

    sizer_top->Add(m_bitmapPanel, 1, wxEXPAND | wxRIGHT, 5);
    sizer_top->Add(sizer_controls, 0, wxEXPAND);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(sizer_top, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    sizer->Add(m_logpanel, 0, wxEXPAND | wxALL, 5);

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

    SetMinClientSize(FromDIP(wxSize(600, 400)));
    SetSize(FromDIP(wxSize(800, 600)));

    m_parent->SetSizerAndFit(sizer);

    Clear();
    Bind(wxEVT_IPCAMERA_FRAME, &MainWindow::OnCameraFrame, this);
    Bind(wxEVT_IPCAMERA_EMPTY, &MainWindow::OnCameraEmpty, this);
    Bind(wxEVT_IPCAMERA_EXCEPTION, &MainWindow::OnCameraException, this);
    Bind(wxEVT_LWIRCAMERA_FRAME, &MainWindow::OnLWIRCameraFrame, this);
    Bind(wxEVT_LWIRCAMERA_EMPTY, &MainWindow::OnCameraEmpty, this);
    Bind(wxEVT_LWIRCAMERA_EXCEPTION, &MainWindow::OnCameraException, this);
    Bind(wxEVT_NIRCAMERA_FRAME, &MainWindow::OnNIRCameraFrame, this);
    Bind(wxEVT_NIRCAMERA_EMPTY, &MainWindow::OnCameraEmpty, this);
    Bind(wxEVT_NIRCAMERA_EXCEPTION, &MainWindow::OnCameraException, this);
    Bind(wxEVT_FUSIONCAMERA_FRAME, &MainWindow::OnFusionCameraFrame, this);
    Bind(wxEVT_FUSIONCAMERA_EMPTY, &MainWindow::OnCameraEmpty, this);
    Bind(wxEVT_FUSIONCAMERA_EXCEPTION, &MainWindow::OnCameraException, this);
}


MainWindow::~MainWindow()
{
    DeleteIPCameraThread();
}


// file functions
void MainWindow::OnConnectZoom(wxCommandEvent& event) {

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

wxBitmap MainWindow::ConvertMatToBitmap(const cv::Mat& matBitmap, long& timeConvert)
{
    wxCHECK(!matBitmap.empty(), wxBitmap());

    wxBitmap    bitmap(matBitmap.cols, matBitmap.rows, 24);
    bool        converted = false;
    wxStopWatch stopWatch;
    long        time = 0;

    stopWatch.Start();
    converted = ConvertMatBitmapTowxBitmap(matBitmap, bitmap);
    time = stopWatch.Time();

    if (!converted)
    {
        wxLogError("Could not convert Mat to wxBitmap.");
        return wxBitmap();
    }

    timeConvert = time;
    return bitmap;
}

void MainWindow::Clear()
{
    DeleteIPCameraThread();

    if (m_videoCapture)
    {
        delete m_videoCapture;
        m_videoCapture = nullptr;
    }

    m_mode = Empty;
    m_sourceName.clear();
    m_currentVideoFrameNumber = 0;

    m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);

    optionsMenu->Enable(window::id::STREAMINFO,0);
    //m_propertiesButton->Disable();

}

// IP CAMERA 

bool MainWindow::StartIPCameraCapture(const wxString& address, const wxSize& resolution,
    bool useMJPEG)
{
    const bool        isDefaultWebCam = address.empty();
    cv::VideoCapture* cap = nullptr;

    Clear();

    {
        wxWindowDisabler disabler;
        wxBusyCursor     busyCursor;


        cap = new cv::VideoCapture(address.ToStdString());
    }

    if (!cap->isOpened())
    {
        delete cap;
        wxLogError("Could not connect to the IP camera.");
        return false;
    }

    m_videoCapture = cap;

    if (!StartIPCameraThread())
    {
        Clear();
        return false;
    }

    return true;
}

bool MainWindow::StartIPCameraThread()
{
    DeleteIPCameraThread();

    m_cameraThread = new CameraThread(this, m_videoCapture);
    if (m_cameraThread->Run() != wxTHREAD_NO_ERROR)
    {
        delete m_cameraThread;
        m_cameraThread = nullptr;
        wxLogError("Could not create the thread needed to retrieve the images from a camera.");
        return false;
    }

    return true;
}

void MainWindow::OnIPCamera(wxCommandEvent&)
{
    static wxString address = "rtsp://192.168.30.168/main";
    if (StartIPCameraCapture(address))
    {
        m_mode = IPCamera;
        m_sourceName = address;
        //m_propertiesButton->Enable();
        optionsMenu->Enable(window::id::STREAMINFO, 1);
    }
}

void MainWindow::OnCameraFrame(wxThreadEvent& evt)
{
    CameraThread::CameraFrame* frame = evt.GetPayload<CameraThread::CameraFrame*>();

    // After deleting the camera thread we may still get a stray frame
    // from yet unprocessed event, just silently drop it.
    if (m_mode != IPCamera)
    {
        delete frame;
        return;
    }

    long     timeConvert = 0;
    wxBitmap bitmap = ConvertMatToBitmap(frame->matBitmap, timeConvert);

    if (bitmap.IsOk())
        m_bitmapPanel->SetBitmap(bitmap, frame->timeGet, timeConvert);
    else
        m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);

    delete frame;
}

void MainWindow::DeleteIPCameraThread()
{
    if (m_cameraThread)
    {
        m_cameraThread->Delete();
        delete m_cameraThread;
        m_cameraThread = nullptr;
    }
}

void MainWindow::InitializeCameras(wxCommandEvent& event)
{
    m_logpanel->m_logtext->AppendText("Initializing cameras...\n");
    m_dataStream=nir.OpenDevice();
    m_lwirhandle = lwir.Init();
    if (!Proxy640USB_IsConnectToModule(m_lwirhandle) == eProxy640USBSuccess)
    {
        m_logpanel->m_logtext->AppendText("Could not connect to the LWIR camera.\n");
    }
    
    lwir.Setup(m_lwirhandle);
    fusion.init(nir.GetFrame(true,m_dataStream), lwir.GetFrame(m_lwirhandle), 0, 0, 0.5, false);
    m_logpanel->m_logtext->AppendText("Cameras initialized\n");
}

// LWIR CAMERA

bool MainWindow::StartLWIRCameraCapture(HANDLE handle)
{
    Clear();

    if (!StartLWIRCameraThread())
    {
        Clear();
        return false;
    }

    return true;

}

bool MainWindow::StartLWIRCameraThread()
{
    DeleteLWIRCameraThread();

    m_lwircameraThread = new LWIRCameraThread(this, m_lwirhandle);
    if (m_lwircameraThread->Run() != wxTHREAD_NO_ERROR)
    {
        delete m_lwircameraThread;
        m_lwircameraThread = nullptr;
        m_logpanel->m_logtext->AppendText("Could not create the thread needed to retrieve the images from a camera.\n");
        return false;
    }

    return true;
}

void MainWindow::DeleteLWIRCameraThread()
{
    if (m_lwircameraThread)
    {
        m_lwircameraThread->Delete();
        delete m_lwircameraThread;
        m_lwircameraThread = nullptr;
    }
}

void MainWindow::OnLWIRCamera(wxCommandEvent&)
{
    
    //static wxString address = "rtsp://192.168.30.168/main";
    if (StartLWIRCameraCapture(m_lwirhandle))
    {
        //m_lwirhandle = lwir.Init();
        m_mode = LWIRCamera;
        //m_propertiesButton->Enable();
        //optionsMenu->Enable(window::id::STREAMINFO, 1);
    }
}

void MainWindow::OnLWIRCameraFrame(wxThreadEvent& evt)
{
    LWIRCameraThread::CameraFrame* frame = evt.GetPayload<LWIRCameraThread::CameraFrame*>();

    // After deleting the camera thread we may still get a stray frame
    // from yet unprocessed event, just silently drop it.
    if (m_mode != LWIRCamera)
    {
        delete frame;
        return;
    }

    long     timeConvert = 0;
    wxBitmap bitmap = ConvertMatToBitmap(frame->matBitmap, timeConvert);

    if (bitmap.IsOk())
        m_bitmapPanel->SetBitmap(bitmap, frame->timeGet, timeConvert);
    else
        m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);

    delete frame;
}

// NIR CAMERA

bool MainWindow::StartNIRCameraCapture() {
    Clear();

    if (!StartNIRCameraThread())
    {
        Clear();
        return false;
    }

    return true;
}
bool MainWindow::StartNIRCameraThread() {
    DeleteNIRCameraThread();

    m_nircameraThread = new NIRCameraThread(this, m_dataStream);
    if (m_nircameraThread->Run() != wxTHREAD_NO_ERROR)
    {
        delete m_nircameraThread;
        m_nircameraThread = nullptr;
        m_logpanel->m_logtext->AppendText("Could not create the thread needed to retrieve the images from a camera.\n");
        return false;
    }

    return true;
}
void MainWindow::DeleteNIRCameraThread() {
    if (m_nircameraThread)
    {
        m_nircameraThread->Delete();
        delete m_nircameraThread;
        m_nircameraThread = nullptr;
    }
}
void MainWindow::OnNIRCamera(wxCommandEvent&) {

    if (StartNIRCameraCapture())
    {
        m_mode = NIRCamera;
    }
}
void MainWindow::OnNIRCameraFrame(wxThreadEvent& evt) {
    NIRCameraThread::CameraFrame* frame = evt.GetPayload<NIRCameraThread::CameraFrame*>();

    // After deleting the camera thread we may still get a stray frame
    // from yet unprocessed event, just silently drop it.
    if (m_mode != NIRCamera)
    {
        delete frame;
        return;
    }

    long     timeConvert = 0;
    wxBitmap bitmap = ConvertMatToBitmap(frame->matBitmap, timeConvert);

    if (bitmap.IsOk())
        m_bitmapPanel->SetBitmap(bitmap, frame->timeGet, timeConvert);
    else
        m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);

    delete frame;
}

// FUSION

bool MainWindow::StartFusionCameraCapture() {
    Clear();

    if (!StartFusionCameraThread())
    {
        Clear();
        return false;
    }

    return true;
}
bool MainWindow::StartFusionCameraThread() {
    DeleteFusionCameraThread();
  
    m_fusioncameraThread = new FusionCameraThread(this, m_dataStream, m_lwirhandle);
    if (m_fusioncameraThread->Run() != wxTHREAD_NO_ERROR)
    {
        delete m_fusioncameraThread;
        m_fusioncameraThread = nullptr;
        m_logpanel->m_logtext->AppendText("Could not create the thread needed to retrieve the images from a NIR or LWIR camera.\n");
        return false;
    }

    return true;
}
void MainWindow::DeleteFusionCameraThread() {
    if (m_fusioncameraThread)
    {
        m_fusioncameraThread->Delete();
        delete m_fusioncameraThread;
        m_fusioncameraThread = nullptr;
    }
}
void MainWindow::OnFusionCamera(wxCommandEvent&) {
    
    if (StartFusionCameraCapture())
    {
        m_mode = FuseNIRLWIR;
    }
}
void MainWindow::OnFusionCameraFrame(wxThreadEvent& evt) {
    FusionCameraThread::CameraFrame* frame = evt.GetPayload<FusionCameraThread::CameraFrame*>();

    // After deleting the camera thread we may still get a stray frame
    // from yet unprocessed event, just silently drop it.
    if (m_mode != FuseNIRLWIR)
    {
        delete frame;
        return;
    }

    long     timeConvert = 0;

    fusion.m_fused_img = fusion.fuse_offset(frame->matNirBitmap, frame->matLwirBitmap);
    //fusion.m_fused_img

    wxBitmap bitmap = ConvertMatToBitmap(fusion.m_fused_img, timeConvert);

    if (bitmap.IsOk())
        m_bitmapPanel->SetBitmap(bitmap, frame->timeGet, timeConvert);
    else
        m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);

    delete frame;
}



// ETC

void MainWindow::OnClear(wxCommandEvent&)
{
    Clear();
}

void MainWindow::OnStreamInfo(wxCommandEvent&)
{
    wxArrayString properties;

    properties.push_back(wxString::Format("Source: %s", m_sourceName));

    if (m_mode == Image)
    {
        const wxBitmap& bmp = m_bitmapPanel->GetBitmap();

        wxCHECK_RET(bmp.IsOk(), "Invalid bitmap in m_bitmapPanel");
        properties.push_back(wxString::Format("Width: %d", bmp.GetWidth()));
        properties.push_back(wxString::Format("Height: %d", bmp.GetHeight()));
    }

    if (m_videoCapture)
    {
        const int  fourCCInt = static_cast<int>(m_videoCapture->get(cv::CAP_PROP_FOURCC));
        const char fourCCStr[] = { (char)(fourCCInt & 0XFF),
                                  (char)((fourCCInt & 0XFF00) >> 8),
                                  (char)((fourCCInt & 0XFF0000) >> 16),
                                  (char)((fourCCInt & 0XFF000000) >> 24), 0 };

        properties.push_back(wxString::Format("Backend: %s", wxString(m_videoCapture->getBackendName())));

        properties.push_back(wxString::Format("Width: %.0f", m_videoCapture->get(cv::CAP_PROP_FRAME_WIDTH)));
        properties.push_back(wxString::Format("Height: %0.f", m_videoCapture->get(cv::CAP_PROP_FRAME_HEIGHT)));

        properties.push_back(wxString::Format("FourCC: %s", fourCCStr));
        properties.push_back(wxString::Format("FPS: %.1f", m_videoCapture->get(cv::CAP_PROP_FPS)));

        if (m_mode == Video)
        {
            // abuse wxDateTime to display position in video as time
            wxDateTime time(static_cast<time_t>(m_videoCapture->get(cv::CAP_PROP_POS_MSEC) / 1000));

            time.MakeUTC(true);

            properties.push_back(wxString::Format("Current frame: %.0f", m_videoCapture->get(cv::CAP_PROP_POS_FRAMES) - 1.0));
            properties.push_back(wxString::Format("Current time: %s", time.FormatISOTime()));
            properties.push_back(wxString::Format("Total frame count: %.f", m_videoCapture->get(cv::CAP_PROP_FRAME_COUNT)));
#if CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR >= 3 )
            properties.push_back(wxString::Format("Bitrate: %.0f kbits/s", m_videoCapture->get(cv::CAP_PROP_BITRATE)));
#endif
        }
    }

    wxGetSingleChoice("Name: value", "Properties", properties, this);
}









void MainWindow::OnCameraEmpty(wxThreadEvent&)
{
    wxLogError("Connection to the camera lost.");

    Clear();
}

void MainWindow::OnCameraException(wxThreadEvent& evt)
{
    wxLogError("Exception in the camera thread: %s", evt.GetString());
    Clear();
}





