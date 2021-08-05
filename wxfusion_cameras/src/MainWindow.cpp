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
    //EVT_BUTTON(window::id::BPTZPOS1, PTZPanel::OnPTPreset1)
    //EVT_BUTTON(window::id::BPTZPOS2, PTZPanel::OnPTPreset2)
    //EVT_BUTTON(window::id::BPTZSAVEPRESET, PTZPanel::OnPTSetPreset)

END_EVENT_TABLE()

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

//class CameraSwitcher : public wxThread {
//public:
//    CameraSwitcher(const MainWindow& aObj) : aRef(aObj) {
//
//    };
//    wxThread::ExitCode Entry(wxThreadEvent& evt,MainWindow& instance);
//    void OnClose(wxCloseEvent&);
//private:
//    const CameraSwitcher& aRef;
//protected:
//    CameraSwitcher* m_pThread;
//    wxCriticalSection m_pThreadCS;    // protects the m_pThread pointer
//    friend class CameraSwitcher;            // allow it to access our m_pThread
//    //wxDECLARE_EVENT_TABLE();
//};
////wxBEGIN_EVENT_TABLE(CameraSwitcher, wxFrame)
//////EVT_CLOSE(CameraSwitcher::OnClose)
////EVT_MENU(Minimal_Start, CameraSwitcher::DoStartThread)
////EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_MYTHREAD_UPDATE, CameraSwitcher::OnThreadUpdate)
////EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_MYTHREAD_COMPLETED, MyCameraSwitcherFrame::OnThreadCompletion)
////wxEND_EVENT_TABLE()
//
//wxThread::ExitCode CameraSwitcher::Entry(wxThreadEvent& evt, MainWindow& instance) {
//    if (instance.current_mode != instance.m_mode) {
//        if (instance.m_mode == instance.IPCamera) {
//            if (instance.StartIPCameraCapture(instance.address))
//            {
//                instance.m_mode = instance.NIRCamera;
//            }
//            instance.m_logpanel->m_logtext->AppendText("Switching to the VIS camera.\n");
//            instance.current_mode = instance.IPCamera;
//        }
//        else if (instance.m_mode == instance.NIRCamera) {
//            if (instance.StartNIRCameraCapture())
//            {
//                instance.m_mode = instance.NIRCamera;
//            }
//            instance.m_logpanel->m_logtext->AppendText("Switching to the NIR camera.\n");
//            instance.current_mode = instance.NIRCamera;
//        }
//        else if (instance.m_mode == instance.LWIRCamera) {
//            if (instance.StartLWIRCameraCapture(instance.m_lwirhandle))
//            {
//                instance.m_mode = instance.NIRCamera;
//            }
//            instance.m_logpanel->m_logtext->AppendText("Switching to the LWIR camera.\n");
//            instance.current_mode = instance.LWIRCamera;
//        }
//        else if (instance.m_mode == instance.FuseNIRLWIR) {
//            if (instance.StartFusionCameraCapture())
//            {
//                instance.m_mode = instance.NIRCamera;
//            }
//            instance.m_logpanel->m_logtext->AppendText("Switching to the Fusion camera.\n");
//            instance.current_mode = instance.FuseNIRLWIR;
//        }
//    }
//
//    return static_cast<wxThread::ExitCode>(nullptr);
//}


//void CameraSwitcher::OnClose(wxCloseEvent&)
//{
//    {
//        wxCriticalSectionLocker enter(m_pThreadCS);
//        if (m_pThread)         // does the thread still exist?
//        {
//            wxMessageOutputDebug().Printf("MYFRAME: deleting thread");
//            if (m_pThread->Delete() != wxTHREAD_NO_ERROR)
//                wxLogError("Can't delete the thread!");
//        }
//    }       // exit from the critical section to give the thread
//            // the possibility to enter its destructor
//            // (which is guarded with m_pThreadCS critical section!)
//    while (1)
//    {
//        { // was the ~MyThread() function executed?
//            wxCriticalSectionLocker enter(m_pThreadCS);
//            if (!m_pThread) break;
//        }
//        // wait for thread completion
//        wxThread::This()->Sleep(1);
//    }
//    Destroy();
//}


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
            frame->timeGet = stopWatch.Time(); //measure retrieval time
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




class Functions
{
public:
    Functions(wxPanel* parent);


    
    void RFPointerOn();
    void RFPointerOff();
    void RFMeasure();
    //void DeleteIPCameraThread(CameraThread* m_cameraThread);
    //void DeleteNIRCameraThread(NIRCameraThread* m_nircameraThread);
    //void DeleteLWIRCameraThread(LWIRCameraThread* m_lwircameraThread);
    //void DeleteFusionCameraThread(FusionCameraThread* m_fusioncameraThread);
    //bool StartIPCameraThread(cv::VideoCapture* m_videoCapture);
    //bool StartIPCameraCapture(const wxString& address);

    //void Clear();
    ////void wait(int i) { Sleep(i*1000); }
    ////void testFunc2(int i) { std::cout << "func2\n"; }
    //cv::VideoCapture* m_videoCapture{ nullptr };
    //CameraThread* m_cameraThread{ nullptr };
    //int m_test;
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
    myParent->m_logpanel->m_logtext->AppendText("Pointer was turned ON \n");
    //m_test = 2;
}

void Functions::RFPointerOff()
{
    Rangefinder rangefinder("COM6", 19200);
    rangefinder.PointerOff();
    myParent->m_logpanel->m_logtext->AppendText("Pointer was turned OFF \n");
    //m_test = 3;
}

void Functions::RFMeasure()
{
    Rangefinder rangefinder("COM6", 19200);
    myParent->m_logpanel->m_logtext->AppendText("Measuring distance.. \n");
    wxString measurement = wxString::Format(wxT("Distance: %.2f meters \n"), rangefinder.Measure());
    myParent->m_logpanel->m_logtext->AppendText(measurement);
}
//// CAMERAS
//void Functions::DeleteIPCameraThread(CameraThread* m_cameraThread)
//{
//    if (m_cameraThread)
//    {
//        m_cameraThread->Delete();
//        delete m_cameraThread;
//        m_cameraThread = nullptr;
//    }
//}
//
//void Functions::DeleteNIRCameraThread(NIRCameraThread* m_nircameraThread)
//{
//    if (m_nircameraThread)
//    {
//        m_nircameraThread->Delete();
//        delete m_nircameraThread;
//        m_nircameraThread = nullptr;
//    }
//}
//
//void Functions::DeleteLWIRCameraThread(LWIRCameraThread* m_lwircameraThread)
//{
//    if (m_lwircameraThread)
//    {
//        m_lwircameraThread->Delete();
//        delete m_lwircameraThread;
//        m_lwircameraThread = nullptr;
//    }
//}
//
//void Functions::DeleteFusionCameraThread(FusionCameraThread* m_fusioncameraThread)
//{
//    if (m_fusioncameraThread)
//    {
//        m_fusioncameraThread->Delete();
//        delete m_fusioncameraThread;
//        m_fusioncameraThread = nullptr;
//    }
//}
//
//bool Functions::StartIPCameraThread(cv::VideoCapture* m_videoCapture) {
//   m_cameraThread = new CameraThread(m_parent->GetEventHandler(), m_videoCapture);
//    if (m_cameraThread->Run() != wxTHREAD_NO_ERROR)
//    {
//        delete m_cameraThread;
//        m_cameraThread = nullptr;
//        wxLogError("Could not create the thread needed to retrieve the images from a camera.");
//        return false;
//    }
//
//    return true;
//};
//
//bool Functions::StartIPCameraCapture(const wxString& address)
//{
//    //Functions fs(m_parent);// = new Functions;
//
//    const bool        isDefaultWebCam = address.empty();
//    cv::VideoCapture* cap = nullptr;
//
//    Clear();
//
//    {
//        wxWindowDisabler disabler;
//        wxBusyCursor     busyCursor;
//
//
//        cap = new cv::VideoCapture(address.ToStdString());
//    }
//
//    if (!cap->isOpened())
//    {
//        delete cap;
//        wxLogError("Could not connect to the IP camera.");
//        return false;
//    }
//
//    m_videoCapture = cap;
//
//    if (!StartIPCameraThread(m_videoCapture))
//        //if (!StartIPCameraThread())
//
//    {
//        Clear();
//        return false;
//    }
//
//    return true;
//}
//
//void Functions::Clear()
//{
//
//    DeleteIPCameraThread(m_cameraThread);
//    //fs.DeleteNIRCameraThread(m_nircameraThread);
//    //fs.DeleteLWIRCameraThread(m_lwircameraThread);
//    //fs.DeleteFusionCameraThread(m_fusioncameraThread);
//
//    //DeleteIPCameraThread();
//    //DeleteNIRCameraThread();
//    //DeleteLWIRCameraThread();
//    //DeleteFusionCameraThread();
//
//    if (m_videoCapture)
//    {
//        delete m_videoCapture;
//        m_videoCapture = nullptr;
//    }
//    /*
//    m_mode = Empty;
//    m_sourceName.clear();
//    m_currentVideoFrameNumber = 0;
//
//    m_bitmapPanel->SetBitmap(wxBitmap(), 0, 0);
//
//    optionsMenu->Enable(window::id::STREAMINFO, 0);
//    */
//    
//    
//    //m_propertiesButton->Disable();
//
//}

class Scripter : public wxFrame, public wxThread
{
public:
    Scripter(wxPanel* parent, wxString file, wxEvtHandler* eventSink);
    ~Scripter() {
    };
    //~Scripter()
    //{
        // it's better to do any thread cleanup in the OnClose()
        // event handler, rather than in the destructor.
        // This is because the event loop for a top-level window is not
        // active anymore when its destructor is called and if the thread
        // sends events when ending, they won't be processed unless
        // you ended the thread from OnClose.
        // See @ref overview_windowdeletion for more info.
    //}
    //void DoStartALongTask(wxPanel* parent);
    //void pause(int);
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
    // the output data of the Entry() routine:
    //char m_data[1024];
    //wxCriticalSection m_dataCS; // protects field above
    DECLARE_EVENT_TABLE();
};

Scripter::Scripter(wxPanel* parent, wxString file, wxEvtHandler* eventSink) : wxThread(wxTHREAD_JOINABLE),
    m_parent(parent), m_filename(file), m_eventSink(eventSink)
{
    typedef void (*funcPointer)(int);

    tfile.Open(m_filename);

    


    // It is also possible to use event tables, but dynamic binding is simpler.
    Bind(wxEVT_THREAD, &Scripter::OnThreadUpdate, this);
}

BEGIN_EVENT_TABLE(Scripter, wxFrame)
EVT_CLOSE(Scripter::OnClose)
END_EVENT_TABLE()

//void Scripter::DoStartALongTask(wxPanel* parent)
//{
//    m_parent = parent;
//    // we want to start a long task, but we don't want our GUI to block
//    // while it's executed, so we use a thread to do it.
//    if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
//    {
//        wxLogError("Could not create the worker thread!");
//        return;
//    }
//    // go!
//    if (GetThread()->Run() != wxTHREAD_NO_ERROR)
//    {
//        wxLogError("Could not run the worker thread!");
//        return;
//    }
//}

//void Scripter::pause(int) {
//
//}
wxThread::ExitCode Scripter::Entry()
{
    MainWindow* myParent = (MainWindow*)m_parent->GetParent();
    
    
    Functions fs(m_parent);
    //ScriptFunctions sfunctions;
    
    //MainWindow* myParent = (MainWindow*)GetParent();
    // VERY IMPORTANT: this function gets executed in the secondary thread context!
    // Do not call any GUI function inside this function; rather use wxQueueEvent():
    int offset = 0;
    // here we do our long task, periodically calling TestDestroy():
    //myParent->m_logpanel->m_logtext->AppendText("Script running... \n");
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

                            //myParent->m_logpanel->m_logtext->AppendText(wxString::Format(wxT("\n Loop %i of %i: "), i+1, cval));
                            str = tfile.GetNextLine();
                            
                            arrstr = wxSplit(str, '=');
                                if (arrstr[0].ToStdString() == "wait") {
                                    if (wxAtoi(arrstr[1]) > 0) {
                                        myParent->m_logpanel->m_logtext->AppendText("\n");
                                        myParent->m_logpanel->m_logtext->AppendText("Sleeping for ");
                                        myParent->m_logpanel->m_logtext->AppendText(arrstr[1]);
                                        myParent->m_logpanel->m_logtext->AppendText(" seconds");
                                        //sfunctions.wait(wxAtoi(arrstr[1]));
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
                                        //myParent->m_logpanel->m_logtext->AppendText("\n");
                                        //myParent->m_logpanel->m_logtext->AppendText("Turning pointer ON");
                                        //Rangefinder rangefinder("COM6", 19200);
                                        //rangefinder.PointerOn();
                                    }
                                    else if (arrstr[1].ToStdString() == "0") {
                                        myParent->m_logpanel->m_logtext->AppendText("\n");
                                        myParent->m_logpanel->m_logtext->AppendText("Turning pointer OFF");
                                        Rangefinder rangefinder("COM6", 19200);
                                        rangefinder.PointerOff();
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
                                        // TODO
                                        
                                    }
                                    else {
                                        //myParent->m_videopanel->m_zoomstream->SetValue(true);
                                        //invalid variable
                                    }
                                }

                                else if (arrstr[0].ToStdString() == "cam") {
                                    if (arrstr[1].ToStdString() == "vis") {
                                        myParent->m_logpanel->m_logtext->AppendText("Turning on the VIS camera");
                                        wxThreadEvent* evt = new wxThreadEvent(wxEVT_CAMERACHANGED_IP);
                                        m_eventSink->QueueEvent(evt);
                                    }
                                    else if (arrstr[1].ToStdString() == "nir") {
                                        myParent->m_logpanel->m_logtext->AppendText("Turning on the NIR camera");
                                        wxThreadEvent* evt = new wxThreadEvent(wxEVT_CAMERACHANGED_NIR);
                                        m_eventSink->QueueEvent(evt);
                                    }
                                    else if (arrstr[1].ToStdString() == "lwir") {
                                        myParent->m_logpanel->m_logtext->AppendText("Turning on the LWIR camera");
                                        wxThreadEvent* evt = new wxThreadEvent(wxEVT_CAMERACHANGED_LWIR);
                                        m_eventSink->QueueEvent(evt);
                                    }
                                    else if (arrstr[1].ToStdString() == "fus") {
                                        myParent->m_logpanel->m_logtext->AppendText("Turning on the Fusion camera");
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

        // since this Entry() is implemented in MyFrame context we don't
        // need any pointer to access the m_data, m_processedData, m_dataCS
        // variables... very nice!
        // this is an example of the generic structure of a download thread:
        
        
        
        //char buffer[1024];
        //download_chunk(buffer, 1024);     // this takes time...
        {
            //wxMessageBox("Instruction set");
            ////////myParent->m_logpanel->m_logtext->AppendText("Script running... \n");
            //parent->m_logpanel->m_logtext->AppendText("Script running... \n");
            // ensure no one reads m_data while we write it
            //wxCriticalSectionLocker lock(m_dataCS);
            //memcpy(m_data + offset, buffer, 1024);
            //offset += 1024;
        }
        
        
        
        
        // signal to main thread that download is complete
        /////wxQueueEvent(GetEventHandler(), new wxThreadEvent());
    }
    // TestDestroy() returned true (which means the main thread asked us
    // to terminate as soon as possible) or we ended the long task...
    return (wxThread::ExitCode)0;
}
void Scripter::OnClose(wxCloseEvent&)
{
    // important: before terminating, we _must_ wait for our joinable
    // thread to end, if it's running; in fact it uses variables of this
    // instance and posts events to *this event handler
    //if (GetThread() &&      // DoStartALongTask() may have not been called
    //    GetThread()->IsRunning())
    //    GetThread()->Wait();
    //Destroy();
}
void Scripter::OnThreadUpdate(wxThreadEvent& evt)
{
    // ...do something... e.g. m_pGauge->Pulse();
    // read some parts of m_data just for fun:
    //wxCriticalSectionLocker lock(m_dataCS);
    //wxPrintf("%c", m_data[100]);


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

    SetMinClientSize(FromDIP(wxSize(1000, 700)));
    SetSize(FromDIP(wxSize(1000, 700)));

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
    //Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MainWindow::OnKeyDown));
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
void MainWindow::OnDCCameras(wxCommandEvent& event)
{
    //Functions fs(m_parent);

    //fs.DeleteIPCameraThread(m_cameraThread);
    //fs.DeleteNIRCameraThread(m_nircameraThread);
    //fs.DeleteLWIRCameraThread(m_lwircameraThread);
    //fs.DeleteFusionCameraThread(m_fusioncameraThread);


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

//wxBitmap MainWindow::ConvertMatToBitmap(const cv::UMat& matBitmap, long& timeConvert)
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

    //fs.DeleteIPCameraThread(m_cameraThread);
    //fs.DeleteNIRCameraThread(m_nircameraThread);
    //fs.DeleteLWIRCameraThread(m_lwircameraThread);
    //fs.DeleteFusionCameraThread(m_fusioncameraThread);

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
    //Functions fs(m_parent);

    //fs.DeleteIPCameraThread(m_cameraThread);
    //fs.DeleteNIRCameraThread(m_nircameraThread);
    //fs.DeleteLWIRCameraThread(m_lwircameraThread);
    //fs.DeleteFusionCameraThread(m_fusioncameraThread);

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
    //else {
    //    m_isInitialized = false;
    //}

}

// IP CAMERA 

bool MainWindow::StartIPCameraCapture(wxString& address, const wxSize& resolution,
    bool useMJPEG)
{
    //Functions fs(m_parent);// = new Functions;

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

    //if (!fs.StartIPCameraThread(m_videoCapture))
        if (!StartIPCameraThread())
        
    {
        Clear();
        return false;
    }

    return true;
}
bool MainWindow::StartIPCameraThread()
{


    //DeleteIPCameraThread();

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
    //Functions fs(m_parent);
    m_onlyZoom = true;
    //InitializeCameras(event);
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
        //m_propertiesButton->Enable();
        optionsMenu->Enable(window::id::STREAMINFO, 1);
        //WARNING
        //m_cameraThread = new CameraThread(this, m_videoCapture);
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

    // After deleting the camera thread we may still get a stray frame
    // from yet unprocessed event, just silently drop it.
    if (m_mode != FuseNIRLWIR)
    {
        delete frame;
        return;
    }

    long     timeConvert = 0;

    fusion.m_fused_img = fusion.fuse_offset(frame->matNirBitmap, frame->matLwirBitmap);

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


//void MainWindow::RFThread() {
//    Rangefinder rangefinder("COM6", 19200);
//    m_logpanel->m_logtext->AppendText("Measuring distance.. \n");
//    wxString measurement = wxString::Format(wxT("Distance: %.2f meters \n"), rangefinder.Measure());
//    m_logpanel->m_logtext->AppendText(measurement);
//}

void MainWindow::OnRFMeasure(wxCommandEvent& event)
{
    Functions fs(m_parent);
    fs.RFMeasure();
    //Rangefinder rangefinder("COM6", 19200);
    //m_logpanel->m_logtext->AppendText("Measuring distance.. \n");
    //wxString measurement = wxString::Format(wxT("Distance: %.2f meters \n"), rangefinder.Measure());
    //m_logpanel->m_logtext->AppendText(measurement);
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

    }
    //wxLogError("Works.");
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

