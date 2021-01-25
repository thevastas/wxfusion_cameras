#include "View.h"



View::View(wxFrame* parent) : wxPanel(parent, window::id::VIEW, wxPoint(0, 0), wxSize(960, 540))
{
    m_p_cap = NULL;
    m_p_picture = NULL;
    //m_width = 640;
    //m_height = 480;
    //m_width = 1920;
    //m_height = 1080;
    m_width = 960;
    m_height = 540;
    m_is_display = false;
    m_timer = new wxTimer(this, -1);
    Connect(wxEVT_TIMER, wxTimerEventHandler(View::OnTimer));
    Connect(wxEVT_PAINT, wxPaintEventHandler(View::OnPaint));
    //Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler(View::OnEraseBackground));
}

View::~View()
{
    delete m_timer;
}

void View::OnPaint(wxPaintEvent& event)
{
    if (!m_is_display)
    {
        return;
    }
    cv::Mat capture;
    *m_p_cap >> capture;
    //res = cv2.resize(img, None, fx = 2, fy = 2, interpolation = cv2.INTER_CUBIC)
    cv::resize(capture, capture, cv::Size(), 0.5, 0.5);

    bool ret = SetPicture(capture);
    if (!ret)
    {
        wxPuts(wxT("Error: can't get picture data."));
        return;
    }

    wxImage image(m_width, m_height, m_p_picture, true);
    wxBitmap current_capture(image);

    wxBufferedPaintDC dc(this);
    dc.DrawBitmap(current_capture, wxPoint(0, 0));
}

void View::OnTimer(wxTimerEvent& event)
{
    Refresh(false);
}

void View::Stop()
{
    if (m_is_display)
    {
        m_timer->Stop();
        m_is_display = false;
        delete m_p_cap;
        delete[] m_p_picture;
    }
    Refresh();
}

void View::Start()
{

    if (m_is_display)
    {
        m_is_display = false;
        delete m_p_cap;
        delete[] m_p_picture;

    }

    //m_p_cap = new cv::VideoCapture(0);
    m_p_cap = new cv::VideoCapture("rtsp://192.168.30.168/main");
    if (!m_p_cap->isOpened())
    {
        wxPuts(wxT("Camera Open Error!"));


        delete m_p_cap;
        return;
    }

    m_p_picture = new unsigned char[m_width * m_height * 3];

    wxPuts(wxT("Camera Open Success!"));

    m_is_display = true;

    //((MainFrame*)GetParent())->SetSize(wxSize(m_width, m_height));

    m_timer->Start(1000 / 30);
}

bool View::SetPicture(cv::Mat& mat)
{
    if (mat.rows != m_height) return false;
    if (mat.cols != m_width) return false;

    if (NULL == m_p_picture) return false;

    for (int i = 0; i < m_width * m_height * 3 - 2; i += 3)
    {
        m_p_picture[i] = mat.data[i + 2];
        m_p_picture[i + 1] = mat.data[i + 1];
        m_p_picture[i + 2] = mat.data[i];
    }
    return true;
}


void View::OnEraseBackground(wxEraseEvent& event)
{
}