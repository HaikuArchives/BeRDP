/*

BeRDP Windows Header

Author: Sikosis (phil@sikosis.com)

(C)2003-2004 Created using BRIE (http://brie.sf.net/)

*/

#ifndef __BeRDPWINDOWS_H__
#define __BeRDPWINDOWS_H__

#include "BeRDP.h"
#include "BeRDPViews.h"

class BeRDPView;
class GeneralView;
class DisplayView;
class AboutView;

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
