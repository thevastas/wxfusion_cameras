#pragma once
#include <wx/wx.h>
#include <wx/panel.h>
#include "wx/statbox.h"
#include "wx/spinctrl.h"
#include "wx/radiobut.h"
class PTZPanel : public wxPanel
{
public:
	PTZPanel(wxFrame* parent);

	wxButton* m_up;
	wxButton* m_down;
	wxButton* m_left;
	wxButton* m_right;
	wxButton* m_upright;
	wxButton* m_downright;
	wxButton* m_upleft;
	wxButton* m_downleft;

	wxStaticText* m_textzoom;
	wxStaticText* m_textfocus;
	wxStaticText* m_textspeed;

	wxButton* m_zoomin;
	wxButton* m_zoomout;
	wxButton* m_focusin;
	wxButton* m_focusout;

	wxSpinCtrl* m_ptzspeed;


	wxFrame* m_parent;
	//wxPanel* m_parent;

	void OnZoomIn(wxCommandEvent& event);
	void OnZoomOut(wxCommandEvent& event);

};

class RangeFinderPanel : public wxPanel
{
public:
	RangeFinderPanel(wxFrame* parent);

	wxButton* m_measuredistance;

	wxFrame* m_parent;

};

class VideoSetPanel : public wxPanel
{
public:
	VideoSetPanel(wxFrame* parent);

	wxStaticText* m_text;

	wxRadioButton* m_zoomstream;
	wxRadioButton* m_nirstream;
	wxRadioButton* m_thermalstream;
	wxRadioButton* m_fusionstream;

	wxFrame* m_parent;
};
