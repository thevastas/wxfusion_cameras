#pragma once
#include "Common.h"
#include "MainWindow.h"
#include <wx/wx.h>
#include <wx/panel.h>
#include "wx/statbox.h"
#include "wx/spinctrl.h"
#include "wx/radiobut.h"
#include "wx/listbox.h"
#include "wx/bitmap.h"
//#include "MainWindow.h"
class PTZPanel : public wxPanel
{
public:
	PTZPanel(wxPanel* parent);

	//wxButton* m_up;
	wxBitmapButton* m_up;
	wxBitmapButton* m_down;
	wxBitmapButton* m_left;
	wxBitmapButton* m_right;
	wxBitmapButton* m_upright;
	wxBitmapButton* m_downright;
	wxBitmapButton* m_upleft;
	wxBitmapButton* m_downleft;

	wxStaticText* m_textzoom;
	wxStaticText* m_textfocus;
	wxStaticText* m_textspeed;

	wxButton* m_zoomin;
	wxButton* m_zoomout;
	wxButton* m_focusin;
	wxButton* m_focusout;

	wxSpinCtrl* m_ptzspeed;

	wxButton* m_pos1;
	wxButton* m_pos2;
	wxButton* m_pos3;
	wxButton* m_pos4;
	wxButton* m_pos5;
	wxButton* m_pos6;
	wxButton* m_pos7;
	wxButton* m_pos8;
	wxStaticText* m_textpos;

	wxPanel* m_parent;
	//wxPanel* m_parent;

	void OnZoomIn(wxCommandEvent& event);
	void OnZoomOut(wxCommandEvent& event);

};

class RangeFinderPanel : public wxPanel
{
public:
	RangeFinderPanel(wxPanel *parent);

	wxButton* m_measuredistance;

	wxPanel* m_parent;

};

class VideoSetPanel : public wxPanel
{
public:
	VideoSetPanel(wxPanel* parent);

	void OnZoomStream(wxCommandEvent& event);
	void OnNirStream(wxCommandEvent& event);
	void OnThermalStream(wxCommandEvent& event);
	void OnFusionStream(wxCommandEvent& event);
	void OnPip(wxCommandEvent& event);

	wxStaticText* m_textstream;
	wxStaticText* m_textpip;

	wxRadioButton* m_zoomstream;
	wxRadioButton* m_nirstream;
	wxRadioButton* m_thermalstream;
	wxRadioButton* m_fusionstream;

	wxListBox* m_pip;

	wxPanel* m_parent;
};

class FusionRatioPanel : public wxPanel
{
public:
	FusionRatioPanel(wxPanel* parent);
	
	wxStaticText* m_textratio;
	wxSlider* m_fusionslider;

	wxPanel* m_parent;
};

class LogPanel : public wxPanel
{
public:
	LogPanel(wxPanel* parent);

	wxPanel* m_parent;
	wxTextCtrl* m_logtext;
};
