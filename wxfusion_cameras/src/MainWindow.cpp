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
#include "Rangefinder.h"
#include "PanTilt.h"
//#include "Fusion.h"

BEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_EXIT, MainWindow::OnQuit)
    EVT_MENU(window::id::RFPOION, MainWindow::OnRFPointerOn) //+
    EVT_MENU(window::id::RFPOIOFF, MainWindow::OnRFPointerOff)//+
    EVT_MENU(window::id::RFMEASURE, MainWindow::OnRFMeasure)//+
    EVT_MENU(window::id::CAMERAINIT, MainWindow::InitializeCameras)
    EVT_MENU(window::id::ENABLEZOOMCAMERA, MainWindow::OnIPCamera)
    EVT_MENU(window::id::ENABLELWIRCAMERA, MainWindow::OnLWIRCamera)
    EVT_MENU(window::id::ENABLENIRCAMERA, MainWindow::OnNIRCamera)
    EVT_MENU(window::id::ENABLEFUSIONCAMERA, MainWindow::OnFusionCamera)
    EVT_MENU(window::id::THERMALPOI, MainWindow::OnThermalPoi)
    EVT_MENU(window::id::NIRPOI, MainWindow::OnNirPoi)
    EVT_MENU(window::id::STREAMINFO, MainWindow::OnStreamInfo)
    EVT_MENU(window::id::CROSSHAIR, MainWindow::OnCrosshair)
    EVT_MENU(window::id::DCCAMERAS, MainWindow::OnDCCameras)
    EVT_MENU(wxID_OPEN, MainWindow::OnOpen)
    EVT_BUTTON(window::id::BMEASUREDISTANCE, MainWindow::OnRFMeasure)
    EVT_RADIOBUTTON(window::id::BZOOMSTREAM, MainWindow::OnIPCamera)
    EVT_RADIOBUTTON(window::id::BNIRSTREAM, MainWindow::OnNIRCamera)
    EVT_RADIOBUTTON(window::id::BTHERMALSTREAM, MainWindow::OnLWIRCamera)
    EVT_RADIOBUTTON(window::id::BFUSIONSTREAM, MainWindow::OnFusionCamera)
END_EVENT_TABLE()


// Camera change events to use in the scripter class
wxDEFINE_EVENT(wxEVT_CAMERACHANGED_IP, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CAMERACHANGED_LWIR, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CAMERACHANGED_NIR, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CAMERACHANGED_FUSION, wxCommandEvent);

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
        cv::UMat matBitmap;
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
        cv::UMat matBitmap;
        long    timeGet{ 0 };
    };
    ThermalCam lwirt;
    LWIRCameraThread(wxEvtHandler* eventSink, HANDLE handle);

protected:
    wxEvtHandler* m_eventSink{ nullptr };
    HANDLE m_lwircamera{ nullptr };

    ExitCode Entry() override;
};

LWIRCameraThread::LWIRCameraThread(wxEvtHandler* eventSink, HANDLE handle)
    : wxThread(wxTHREAD_JOINABLE),
    m_eventSink(eventSink), m_lwircamera(handle)
{

    wxASSERT(m_eventSink);
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
            frame->timeGet = stopWatch.Time(); //measure retrieval time
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
        cv::UMat matBitmap;
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
        cv::UMat matNirBitmap;
        cv::UMat matLwirBitmap;
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




class Functions
{
public:
    Functions(wxPanel* parent);


    
    void RFPointerOn();
    void RFPointerOff();
    void RFMeasure();
    //void QueryPan();
    //void QueryTilt();
    wxPanel* m_parent;
    MainWindow* myParent;
};

Functions::Functions(wxPanel* parent) :
    m_parent(parent)
{
    myParent = (MainWindow*)m_parent->GetParent();
}
//Rangefinder functions
void Functions::RFPointerOn()
{
    
    Rangefinder rangefinder("COM6", 19200);
    rangefinder.PointerOn();
    myParent->m_logpanel->m_logtext->AppendText("\nPointer was turned ON \n");
    //m_test = 2;
}

void Functions::RFPointerOff()
{
    Rangefinder rangefinder("COM6", 19200);
    rangefinder.PointerOff();
    myParent->m_logpanel->m_logtext->AppendText("\nPointer was turned OFF \n");
    //m_test = 3;
}

void Functions::RFMeasure()
{
    Rangefinder rangefinder("COM6", 19200);
    myParent->m_logpanel->m_logtext->AppendText("Measuring distance.. \n");
    wxString measurement = wxString::Format(wxT("Distance: %.2f meters \n"), rangefinder.Measure());
    myParent->m_logpanel->m_logtext->AppendText(measurement);
}


class Scripter : public wxFrame, public wxThread
{
public:
    Scripter(wxPanel* parent, wxString file, wxEvtHandler* eventSink);
    ~Scripter() {
    };
    void OnThreadUpdate(wxThreadEvent& evt);
    void OnClose(wxCloseEvent& evt);
    wxPanel* m_parent;
    wxString m_filename;
    wxTextFile tfile;
    wxString        str;
    wxArrayString arrstr;
protected:
    wxEvtHandler* m_eventSink{ nullptr };
    ExitCode Entry() override;
    DECLARE_EVENT_TABLE();
};

Scripter::Scripter(wxPanel* parent, wxString file, wxEvtHandler* eventSink) : wxThread(wxTHREAD_JOINABLE),
    m_parent(parent), m_filename(file), m_eventSink(eventSink)
{
    typedef void (*funcPointer)(int);

    tfile.Open(m_filename);
    Bind(wxEVT_THREAD, &Scripter::OnThreadUpdate, this);
}

BEGIN_EVENT_TABLE(Scripter, wxFrame)
EVT_CLOSE(Scripter::OnClose)
END_EVENT_TABLE()

wxThread::ExitCode Scripter::Entry()
{
    MainWindow* myParent = (MainWindow*)m_parent->GetParent();
    
    
    Functions fs(m_parent);
    int offset = 0;
    myParent->m_logpanel->m_logtext->AppendText(m_filename);
    while (!TestDestroy())
    {



        for (str = tfile.GetFirstLine(); !tfile.Eof(); str = tfile.GetNextLine())
        {
            //str = tfile.GetNextLine();
            arrstr=wxSplit(str, '=');
            int cval = wxAtoi(arrstr[1]);
            if (!arrstr.IsEmpty()) {
                myParent->m_logpanel->m_logtext->AppendText("\n");
                myParent->m_logpanel->m_logtext->AppendText(arrstr[0]);
                myParent->m_logpanel->m_logtext->AppendText("   ");
                myParent->m_logpanel->m_logtext->AppendText(arrstr[1]);
                if (arrstr[0].ToStdString() == "loop") {
                    
                    if (wxAtoi(arrstr[1]) > 0) {
                        int curline = tfile.GetCurrentLine();
                        int loopsize = wxAtoi(arrstr[1]);
                        int i = 0;
                        while (i < loopsize) {

                            str = tfile.GetNextLine();
                            
                            arrstr = wxSplit(str, '=');
                                if (arrstr[0].ToStdString() == "wait") {
                                    if (wxAtoi(arrstr[1]) > 0) {
                                        myParent->m_logpanel->m_logtext->AppendText("\n");
                                        myParent->m_logpanel->m_logtext->AppendText("Sleeping for ");
                                        myParent->m_logpanel->m_logtext->AppendText(arrstr[1]);
                                        myParent->m_logpanel->m_logtext->AppendText(" seconds");
                                        wxThread::Sleep(wxAtoi(arrstr[1]) * 1000);
                                    }
                                    else {
                                        //invalid variable
                                    }

                            }
                                else if (arrstr[0].ToStdString() == "loop" && arrstr[1].ToStdString() == "0") {
                                    tfile.GoToLine(curline);
                                    i++;
                                }
                                else if (arrstr[0].ToStdString() == "pos") {
                                    
                                    if (wxAtoi(arrstr[1]) <= 10 && wxAtoi(arrstr[1]) > 0) {
                                        myParent->m_logpanel->m_logtext->AppendText("\n");
                                        myParent->m_logpanel->m_logtext->AppendText("Going to position: ");
                                        myParent->m_logpanel->m_logtext->AppendText(arrstr[1]);


                                        PanTilt pt("COM5", 2400);
                                        pt.Preset(wxAtoi(arrstr[1]));
                                    }
                                    else {
                                        
                                        //invalid variable
                                    }
                                }
                                else if (arrstr[0].ToStdString() == "rfp") {
                                    if (arrstr[1].ToStdString() == "1") {
                                        fs.RFPointerOn();
                                    }
                                    else if (arrstr[1].ToStdString() == "0") {
                                        fs.RFPointerOff();
                                    }
                                    else {
                                        //invalid variable
                                    }
                                }
                                else if (arrstr[0].ToStdString() == "rfd") {
                                    if (arrstr[1].ToStdString() == "1") {
                                        Rangefinder rangefinder("COM6", 19200);
                                        myParent->m_logpanel->m_logtext->AppendText("\n");
                                        myParent->m_logpanel->m_logtext->AppendText("Distance:");
                                        myParent->m_logpanel->m_logtext->AppendText(wxString::Format(wxT(" %f"), rangefinder.Measure()));
                                        myParent->m_logpanel->m_logtext->AppendText(" meters.");
                                        
                                    }
                                    else {
                                        //invalid variable
                                    }
                                }

                                else if (arrstr[0].ToStdString() == "cam") {
                                    if (arrstr[1].ToStdString() == "vis") {
                                        myParent->m_logpanel->m_logtext->AppendText("\nTurning on the VIS camera");
                                        wxThreadEvent* evt = new wxThreadEvent(wxEVT_CAMERACHANGED_IP);
                                        m_eventSink->QueueEvent(evt);
                                    }
                                    else if (arrstr[1].ToStdString() == "nir") {
                                        myParent->m_logpanel->m_logtext->AppendText("\nTurning on the NIR camera");
                                        wxThreadEvent* evt = new wxThreadEvent(wxEVT_CAMERACHANGED_NIR);
                                        m_eventSink->QueueEvent(evt);
                                    }
                                    else if (arrstr[1].ToStdString() == "lwir") {
                                        myParent->m_logpanel->m_logtext->AppendText("\nTurning on the LWIR camera");
                                        wxThreadEvent* evt = new wxThreadEvent(wxEVT_CAMERACHANGED_LWIR);
                                        m_eventSink->QueueEvent(evt);
                                    }
                                    else if (arrstr[1].ToStdString() == "fus") {
                                        myParent->m_logpanel->m_logtext->AppendText("\nTurning on the Fusion camera");
                                        wxThreadEvent* evt = new wxThreadEvent(wxEVT_CAMERACHANGED_FUSION);
                                        m_eventSink->QueueEvent(evt);
                                    }
                                }

                                else {
                                    myParent->m_logpanel->m_logtext->AppendText("\n");
                                    myParent->m_logpanel->m_logtext->AppendText(str);
                                }

                        }
                    }




                    else wxThread::Delete();
                                    




                }

                
                


            }
        }

    }
    return (wxThread::ExitCode)0;
}
void Scripter::OnClose(wxCloseEvent&)
{

}
void Scripter::OnThreadUpdate(wxThreadEvent& evt)
{
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
    fileMenu = new wxMenu();
    menuBar->Append(fileMenu, _("&File"));
    
    fileMenu->Append(wxID_NEW);


    wxMenuItem* openitem = new wxMenuItem(fileMenu, wxID_OPEN);
    openitem->SetBitmap(wxArtProvider::GetBitmap("wxART_FILE_OPEN"));
    openitem->SetItemLabel("Open script file");
    fileMenu->Append(openitem);
    
    wxMenuItem* quitItem = new wxMenuItem(fileMenu, wxID_EXIT);
    quitItem->SetBitmap(wxArtProvider::GetBitmap("wxART_QUIT"));
    fileMenu->Append(quitItem);


    // rangefinder MENU
    rangefinderMenu = new wxMenu();
    menuBar->Append(rangefinderMenu, _("&Rangefinder"));
    rangefinderMenu->Append(window::id::RFPOION, _("Pointer ON"));
    rangefinderMenu->Append(window::id::RFPOIOFF, _("Pointer OFF"));
    rangefinderMenu->Append(window::id::RFMEASURE, _("Measure"));

    // camera MENU
    cameraMenu = new wxMenu();
    menuBar->Append(cameraMenu, _("&Camera"));
    cameraMenu->Append(window::id::CAMERAINIT, "Initialize cameras");
    cameraMenu->Append(window::id::ENABLEZOOMCAMERA, "Enable zoom camera");
    cameraMenu->Append(window::id::ENABLELWIRCAMERA, "Enable LWIR camera");
    cameraMenu->Append(window::id::ENABLENIRCAMERA, "Enable NIR camera");
    cameraMenu->Append(window::id::ENABLEFUSIONCAMERA, "Enable fusion camera");
    cameraMenu->Append(window::id::DCCAMERAS, "Disconnect cameras");

    // VIEW MENU
    viewMenu = new wxMenu();
    menuBar->Append(viewMenu, _("&View"));
    viewMenu->Append(window::id::THERMALPOI,"Thermal POI");
    viewMenu->Append(window::id::NIRPOI, "NIR POI");
    viewMenu->Append(window::id::CROSSHAIR, "Crosshair");

    // OPTIONS MENU
    optionsMenu = new wxMenu();
    menuBar->Append(optionsMenu, _("&Options"));
    optionsMenu->Append(window::id::CONNSETTINGS, "Connection settings");
    optionsMenu->Append(window::id::STREAMINFO, "Stream information");
        
    SetMenuBar(menuBar);

    m_bitmapPanel = new wxBitmapFromOpenCVPanel(m_parent);

    m_logpanel = new LogPanel(m_parent);
    m_logpanel->SetBackgroundColour(wxColor(64, 64, 64));

    m_rfpanel = new RangeFinderPanel(m_parent);
    m_rfpanel->SetBackgroundColour(wxColor(64, 64, 64));

    m_ptzpanel = new PTZPanel(m_parent);
    m_ptzpanel->SetBackgroundColour(wxColor(64, 64, 64));

    m_videopanel = new VideoSetPanel(m_parent);
    m_videopanel->SetBackgroundColour(wxColor(64, 64, 64));

    m_fusionratiopanel = new FusionRatioPanel(m_parent);
    m_fusionratiopanel->SetBackgroundColour(wxColor(64, 64, 64));

    m_thermalpanel = new ThermalLensPanel(m_parent); 
    m_thermalpanel->SetBackgroundColour(wxColor(64, 64, 64));

    wxSizer* sizer_controls = new wxBoxSizer(wxVERTICAL);
    sizer_controls->Add(m_videopanel, 0, wxEXPAND | wxBOTTOM, 5);
    sizer_controls->Add(m_fusionratiopanel, 0, wxEXPAND | wxBOTTOM, 5);
    sizer_controls->Add(m_rfpanel, 0, wxEXPAND | wxBOTTOM, 5);
    sizer_controls->Add(m_thermalpanel, 0, wxEXPAND | wxBOTTOM, 5);
    sizer_controls->Add(m_ptzpanel, 1, wxEXPAND);


    wxSizer* sizer_top = new wxBoxSizer(wxHORIZONTAL);

    sizer_top->Add(m_bitmapPanel, 1, wxEXPAND | wxRIGHT, 5);
    sizer_top->Add(sizer_controls, 0, wxEXPAND);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(sizer_top, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
    sizer->Add(m_logpanel, 0, wxEXPAND | wxALL, 5);


    SetMinClientSize(FromDIP(wxSize(1000, 800)));
    SetSize(FromDIP(wxSize(1000, 800)));

    m_parent->SetSizerAndFit(sizer);








    Clear();
    Bind(wxEVT_CAMERACHANGED_IP, &MainWindow::OnIPCamera, this);
    Bind(wxEVT_CAMERACHANGED_LWIR, &MainWindow::OnLWIRCamera, this);
    Bind(wxEVT_CAMERACHANGED_NIR, &MainWindow::OnNIRCamera, this);
    Bind(wxEVT_CAMERACHANGED_FUSION, &MainWindow::OnFusionCamera, this);
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
    Bind(wxEVT_KEY_DOWN, &MainWindow::OnKeyDown, this);
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
    if (!m_crosshair) {
        m_crosshair = true;
    }
    else m_crosshair = false;
};
void MainWindow::OnDCCameras(wxCommandEvent& event)
{
    DeleteIPCameraThread();
    DeleteNIRCameraThread();
    DeleteLWIRCameraThread();
    DeleteFusionCameraThread();


    if (m_isInitialized) {
        cameraMenu->Enable(window::id::CAMERAINIT, 1);
        m_logpanel->m_logtext->AppendText("Disconnecting LWIR camera...\n");


        Proxy640USB_DisconnectFromModule(m_lwirhandle);
        m_logpanel->m_logtext->AppendText("LWIR camera disconnected\n");
        m_isInitialized = false;

    }
    else {
        m_isInitialized = false;
    }

};

wxBitmap MainWindow::ConvertMatToBitmap(const cv::UMat matBitmap, long& timeConvert)
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

    Functions fs(m_parent);

    DeleteIPCameraThread();
    DeleteNIRCameraThread();
    DeleteLWIRCameraThread();
    DeleteFusionCameraThread();

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

void MainWindow::InitializeCameras(wxCommandEvent& event)
{
    DeleteIPCameraThread();
    DeleteNIRCameraThread();
    DeleteLWIRCameraThread();
    DeleteFusionCameraThread();


    if (!m_isInitialized) {
        cameraMenu->Enable(window::id::CAMERAINIT, 0);
        m_logpanel->m_logtext->AppendText("Initializing cameras...\n");
        m_dataStream = nir.OpenDevice();
        m_lwirhandle = lwir.Init();
        if (!Proxy640USB_IsConnectToModule(m_lwirhandle) == eProxy640USBSuccess)
        {
            m_logpanel->m_logtext->AppendText("Could not connect to the LWIR camera.\n");
        }

        lwir.Setup(m_lwirhandle);
        //fusion.init(nir.GetFrame(true, m_dataStream), lwir.GetFrame(m_lwirhandle), 0, 0, 0.5, true);
        m_logpanel->m_logtext->AppendText("Cameras initialized\n");
        m_isInitialized = true;

    }

}

// IP CAMERA 

bool MainWindow::StartIPCameraCapture(wxString& address, const wxSize& resolution,
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
void MainWindow::OnIPCamera(wxCommandEvent& event)
{
    m_onlyZoom = true;
    wxArrayString strings;
    strings.push_back("NIR");
    strings.push_back("LWIR");
    strings.push_back("Fusion");
    strings.push_back("Disabled");
    m_videopanel->m_pip->Set(strings);


    
    if (StartIPCameraCapture(address))
    {
        m_mode = IPCamera;
        m_sourceName = address;
        optionsMenu->Enable(window::id::STREAMINFO, 1);
    }
    else Clear();
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
    frame->matBitmap.copyTo(m_ocvmat);
    if (m_crosshair) cv::drawMarker(m_ocvmat, cv::Point(960, 540), cv::Scalar(0, 0, 255), cv::MARKER_CROSS, 50, 1);
  
    wxBitmap bitmap = ConvertMatToBitmap(m_ocvmat, timeConvert);

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
void MainWindow::OnLWIRCamera(wxCommandEvent& event)
{
    m_onlyZoom = false;
    InitializeCameras(event);
    wxArrayString strings;
    strings.push_back("NIR");
    strings.push_back("Zoom");
    strings.push_back("Fusion");
    strings.push_back("Disabled");
    m_videopanel->m_pip->Set(strings);
    if (StartLWIRCameraCapture(m_lwirhandle))
    {
        m_mode = LWIRCamera;
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
    if (m_crosshair) cv::drawMarker(frame->matBitmap, cv::Point(320, 240), cv::Scalar(0, 0, 255), cv::MARKER_CROSS, 50, 1);
    wxBitmap bitmap = ConvertMatToBitmap(frame->matBitmap, timeConvert);
    
    if (bitmap.IsOk())
        m_bitmapPanel->SetBitmap(bitmap, frame->timeGet, timeConvert);
    else
        m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);

    delete frame;
}

// NIR CAMERA

void MainWindow::OnNIRCamera(wxCommandEvent& event) {
    m_onlyZoom = false;
    InitializeCameras(event);
    wxArrayString strings;
    strings.push_back("Zoom");
    strings.push_back("LWIR");
    strings.push_back("Fusion");
    strings.push_back("Disabled");
    m_videopanel->m_pip->Set(strings);

    if (StartNIRCameraCapture())
    {
        m_mode = NIRCamera;
    }
}
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
    DeleteIPCameraThread();
    DeleteNIRCameraThread();
    DeleteLWIRCameraThread();
    DeleteFusionCameraThread();
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
void MainWindow::OnNIRCameraFrame(wxThreadEvent& evt) {
    NIRCameraThread::CameraFrame* frame = evt.GetPayload<NIRCameraThread::CameraFrame*>();

    if (m_mode != NIRCamera)
    {
        delete frame;
        return;
    }

    long     timeConvert = 0;
    if (m_crosshair) cv::drawMarker(frame->matBitmap, cv::Point(320, 240), cv::Scalar(0, 0, 255), cv::MARKER_CROSS, 50, 1); //TODO fix center
    wxBitmap bitmap = ConvertMatToBitmap(frame->matBitmap, timeConvert);

    if (bitmap.IsOk())
        m_bitmapPanel->SetBitmap(bitmap, frame->timeGet, timeConvert);
    else
        m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);

    delete frame;
}

// FUSION
void MainWindow::OnFusionCamera(wxCommandEvent& event) {
    m_onlyZoom = false;

    InitializeCameras(event);
    wxArrayString strings;
    strings.push_back("NIR");
    strings.push_back("LWIR");
    strings.push_back("Zoom");
    strings.push_back("Disabled");
    m_videopanel->m_pip->Set(strings);
    if (StartFusionCameraCapture())
    {
        m_mode = FuseNIRLWIR;
    }
}
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
    DeleteIPCameraThread();
    DeleteNIRCameraThread();
    DeleteLWIRCameraThread();
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
void MainWindow::OnFusionCameraFrame(wxThreadEvent& evt) {
    FusionCameraThread::CameraFrame* frame = evt.GetPayload<FusionCameraThread::CameraFrame*>();

    if (m_mode != FuseNIRLWIR)
    {
        delete frame;
        return;
    }

    long     timeConvert = 0;

    fusion.m_fused_img = fusion.fuse_offset(frame->matNirBitmap, frame->matLwirBitmap);
    if (m_crosshair) cv::drawMarker(fusion.m_fused_img, cv::Point(320, 240), cv::Scalar(0, 0, 255), cv::MARKER_CROSS, 50, 1); //TODO fix center
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


// Rangefinder functions
void MainWindow::OnRFPointerOn(wxCommandEvent& event)
{
    Functions fs(m_parent);
    fs.RFPointerOn();
}

void MainWindow::OnRFPointerOff(wxCommandEvent& event)
{
    Functions fs(m_parent);
    fs.RFPointerOff();
}



void MainWindow::OnRFMeasure(wxCommandEvent& event)
{
    Functions fs(m_parent);
    fs.RFMeasure();
}

void MainWindow::OnOpen(wxCommandEvent& event)
{
    wxString fileName;
    wxFileDialog* openFileDialog = new wxFileDialog(this);
    if (openFileDialog->ShowModal() == wxID_OK) {
        fileName = openFileDialog->GetPath();
    }
    m_scripterThread = new Scripter(m_parent,fileName,this);
    if (m_scripterThread->Run() != wxTHREAD_NO_ERROR)
    {
        delete m_scripterThread;
        m_scripterThread = nullptr;
        m_logpanel->m_logtext->AppendText("Could not create the scripter thread.\n");
    }
    else {
        m_logpanel->m_logtext->AppendText("Launching Script file... \n");
        InitializeCameras(event);
    }
}


void MainWindow::OnKeyDown(wxKeyEvent& event) // TODO neveikia
{

    m_logpanel->m_logtext->AppendText("Key pressed \n");

    switch (event.GetKeyCode())
    {
    case WXK_SPACE:
        //MainWindow::RFThread();
        break;
    case WXK_UP:
        break;
    case WXK_DOWN:
        break;
    case WXK_LEFT:
        break;
    case WXK_RIGHT:
        break;
    default:
        event.Skip();
    }
}

