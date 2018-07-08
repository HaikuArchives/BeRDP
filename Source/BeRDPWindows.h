/*
 * Copyright 2003-2018. All rights reserved.
 * Distributed under the terms of the BSD license.
 *
 * Author:
 *	Sikosis, phil@sikosis.com
 *
 * BeRDP is a GUI for RDesktop
 */

#ifndef __BeRDPWINDOWS_H__
#define __BeRDPWINDOWS_H__

#include "BeRDP.h"
#include "BeRDPViews.h"

class BeRDPView;
class GeneralView;
class DisplayView;
class AboutView;

/* Predefs -- so we don't have to include them here */
class BTabView;
class BTab;
class BButton;
class BCheckBox;
class BSlider;
class BTextControl;
class BStringView;
class BTextView;
class BMenuField;
class BMenu;
class BMenuItem;

class BeRDPWindow : public BWindow
{
	public:
		BeRDPWindow(BRect frame);
		~BeRDPWindow();
		virtual bool QuitRequested();
		virtual void MessageReceived(BMessage *message);
		virtual void UpdateDisplaySlider();

	private:
		void InitWindow(void);

		void LoadSettings(BMessage *msg);
		void SaveSettings(const char *cnxname);
		void LoadConnectionList(BMessage *msg);
		void SaveConnectionList(void);
		void SaveConnectionDetails(const char *cnxname);

		BeRDPView*		ptrBeRDPView;
		GeneralView*    ptrGeneralView;
		DisplayView*	ptrDisplayView;
		AboutView*		ptrAboutView;

		BTabView		*tabView;
	    BTab			*tab;
	    BButton         *btnConnect;
	    BButton         *btnClose;
	    BCheckBox       *chkForceBitmapUpdates;
	    BSlider         *sldDisplaySize;
	    BTextControl    *txtComputer;
	    BTextControl    *txtUsername;
	    BTextControl    *txtPassword; // I dont like to use this Password param
	    BTextControl    *txtDomain;   // (so it'll be greyed out)
	    BStringView     *stvTitle;
	    BStringView     *stvAuthor1;
	    BTextView		*txvDescription;
	    BStringView     *stvURL;
	    BMenuField      *mnfConnection;
	    BMenu           *pmnConnection;
	    BMenuItem		*mniConnectionDelete;
};

#endif
