#pragma once
namespace window
{
	enum id
	{
		MAINWINDOW = wxID_HIGHEST + 1,
		KEY=99,
		VIEW = 100,
		RFPOION = 110,
		RFPOIOFF = 111,
		RFMEASURE = 112,
		ENABLEZOOMCAMERA = 120,
		ENABLELWIRCAMERA = 121,
		ENABLENIRCAMERA = 122,
		ENABLEFUSIONCAMERA = 123,
		CAMERAINIT = 129,
		THERMALPOI = 130,
		NIRPOI = 131,
		CROSSHAIR = 132,
		DCCAMERAS = 133,
		CONNSETTINGS = 140,
		STREAMINFO = 141,
		BZOOMIN = 1001,
		BZOOMOUT = 1002,
		BUP = 1003,
		BDOWN = 1004,
		BLEFT = 1005,
		BRIGHT = 1006,
		BUPLEFT = 1007,
		BUPRIGHT = 1008,
		BDOWNLEFT = 1009,
		BDOWNRIGHT = 1010,
		BFOCUSIN = 1011,
		BFOCUSOUT = 1012,
		BPTZSPEED = 1013,
		BMEASUREDISTANCE = 1014,
		BZOOMSTREAM = 1015,
		BNIRSTREAM = 1016,
		BTHERMALSTREAM = 1017,
		BFUSIONSTREAM = 1018,
		BPIP = 1019,
		BPTZPOS1 = 1020,
		BPTZPOS2 = 1021,
		BPTZPOS3 = 1022,
		BPTZPOS4 = 1023,
		BPTZPOS5 = 1024,
		BPTZPOS6 = 1025,
		BPTZPOS7 = 1026,
		BPTZPOS8 = 1027,
		BFUSIONRATIO = 1028,
		BPTZSETPRESET = 1029,
		BPTZSAVEPRESET = 1030,
		BTZOOMIN = 1031,
		BTZOOMOUT = 1032,
		BTFOCUSIN = 1033,
		BTFOCUSOUT = 1034,
		LOGTEXT = 1100
	};
}
