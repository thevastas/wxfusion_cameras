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
class PTZPanel : public wxPanel
{
public:
	PTZPanel(wxPanel* parent);

	// arrow buttons
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
	wxSpinCtrl* m_preset;

	// position buttons 1 - 8
	wxButton* m_pos1;
	wxButton* m_pos2;
	wxButton* m_pos3;
	wxButton* m_pos4;
	wxButton* m_pos5;
	wxButton* m_pos6;
	wxButton* m_pos7;
	wxButton* m_pos8;
	wxStaticText* m_textpos;
	wxButton* m_savepreset;
	


	wxPanel* m_parent;

	void OnZoomIn(wxCommandEvent& event);
	void OnZoomOut(wxCommandEvent& event);
	void OnPTPreset1(wxMouseEvent& event);
	void OnPTPreset2(wxMouseEvent& event);
	void OnPTPreset3(wxMouseEvent& event);
	void OnPTPreset4(wxMouseEvent& event);
	void OnPTPreset5(wxMouseEvent& event);
	void OnPTPreset6(wxMouseEvent& event);
	void OnPTPreset7(wxMouseEvent& event);
	void OnPTPreset8(wxMouseEvent& event);
	void OnPTPreset9(wxMouseEvent& event);
	void OnPTSetPreset(wxMouseEvent& event);
	void OnPTUp(wxMouseEvent& event);
	void OnPTDown(wxMouseEvent& event);
	void OnPTLeft(wxMouseEvent& event);
	void OnPTRight(wxMouseEvent& event);
	void OnPTUpLeft(wxMouseEvent& event);
	void OnPTUpRight(wxMouseEvent& event);
	void OnPTDownLeft(wxMouseEvent& event);
	void OnPTDownRight(wxMouseEvent& event);
	void OnPTStop(wxMouseEvent& event);

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

class ThermalLensPanel : public wxPanel
{
public:

	ThermalLensPanel(wxPanel* parent);
	wxPanel* m_parent;
	wxButton* m_zoomin;
	wxButton* m_zoomout;
	wxButton* m_focusin;
	wxButton* m_focusout;
	wxStaticText* m_textzoom;
	wxStaticText* m_textfocus;
	void OnStop(wxMouseEvent& event);
	void OnZoomIn(wxMouseEvent& event);
	void OnZoomOut(wxMouseEvent& event);
	void OnFocusIn(wxMouseEvent& event);
	void OnFocusOut(wxMouseEvent& event);
};