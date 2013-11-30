/*

BeRDPWindow

Author: BeRDP

(C)2003 Created using BRIE (http://brie.sf.net/)

*/

// Includes ------------------------------------------------------------------------------------------ //
#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <CheckBox.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>
#include <Screen.h>
#include <ScrollView.h>
#include <Slider.h>
#include <stdio.h>
#include <String.h>
#include <StringView.h>
#include <TabView.h>
#include <TextControl.h>
#include <Window.h>
#include <View.h>

#include "BeRDP.h"
#include "BeRDPWindows.h"
#include "BeRDPViews.h"
#include "BeRDPConstants.h"
// -------------------------------------------------------------------------------------------------- //



// CenterWindowOnScreen -- Centers the BWindow to the Current Screen
static void CenterWindowOnScreen(BWindow* w)
{
	BRect	screenFrame = (BScreen(B_MAIN_SCREEN_ID).Frame());	BPoint	pt;
	pt.x = screenFrame.Width()/2 - w->Bounds().Width()/2;
	pt.y = screenFrame.Height()/2 - w->Bounds().Height()/2;

	if (screenFrame.Contains(pt))
		w->MoveTo(pt);
}
// -------------------------------------------------------------------------------------------------- //

// BeRDPWindow - Constructor
BeRDPWindow::BeRDPWindow(BRect frame) : BWindow (frame, "BeRDP v0.2", B_TITLED_WINDOW, B_NORMAL_WINDOW_FEEL , 0)
{
	InitWindow();
	CenterWindowOnScreen(this);

	// Load User Settings 
	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	path.Append("BeRDP_Settings",true);
	BFile file(path.Path(),B_READ_ONLY);
	BMessage msg;
	msg.Unflatten(&file);
	LoadSettings (&msg);
    
    // Set Window Limits
	SetSizeLimits(315,315,205,205);
    
    // Update from our Loaded Settings
    UpdateDisplaySlider();
    
	Show();
}
// -------------------------------------------------------------------------------------------------- //

// BeRDPWindow - Destructor
BeRDPWindow::~BeRDPWindow()
{
	exit(0);
}
// -------------------------------------------------------------------------------------------------- //

// BeRDPWindow::InitWindow -- Initialization Commands here
void BeRDPWindow::InitWindow(void)
{
	BRect r;
	BRect rtab;
	BRect rlist;
	r = Bounds();
    rtab = Bounds();
    rtab.top += 10;
    rlist = Bounds();
    rlist.top += 44;
    rlist.left += 12;
    rlist.right -= 12;
    rlist.bottom -= 12;
    
    // Create Views for our Tabs
    ptrGeneralView = new GeneralView(r);
    ptrDisplayView = new DisplayView(r);
    ptrAboutView = new AboutView(r);
    
    // Create the Buttons for GeneralView
	btnConnect = new BButton(BRect (133,135,213,165), 
					"btnConnect","Connect", new BMessage(BTN_CONNECT),
					B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btnConnect->MakeDefault(true);
	btnConnect->MakeFocus(true);					
	btnClose = new BButton(BRect (225,135,305,165), 
					"btnClose","Close", new BMessage(BTN_CLOSE),
					B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);	
	ptrGeneralView->AddChild(btnConnect);
    ptrGeneralView->AddChild(btnClose);
    
    // Create TextControls for GeneralView
    txtComputer = new BTextControl(BRect (30,25,220,40), "txtComputer",
      					"Computer: ", "", new BMessage (TXT_COMPUTER),
						B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtComputer->SetDivider(55);
	txtUsername = new BTextControl(BRect (30,50,250,65), "txtUsername",
      					"Username: ", "", new BMessage (TXT_USERNAME),
						B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtUsername->SetDivider(55);					
    
    txtPassword = new BTextControl(BRect (30,75,250,90), "txtPassword",
      					"Password: ", "", new BMessage (TXT_PASSWORD),
						B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtPassword->SetDivider(55);					
    txtPassword->SetEnabled(false);
    
    txtDomain = new BTextControl(BRect (30,100,250,115), "txtDomain",
      					"Domain: ", "", new BMessage (TXT_DOMAIN),
						B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtDomain->SetDivider(55);
	ptrGeneralView->AddChild(txtComputer);
	ptrGeneralView->AddChild(txtUsername);					
    ptrGeneralView->AddChild(txtPassword);					
    ptrGeneralView->AddChild(txtDomain);
    
    // Create CheckBox for Display
    chkForceBitmapUpdates = new BCheckBox(BRect (30,85,300,100), "chkForceBitmapUpdates",
      					"Force Bitmap Updates", new BMessage (CHK_FORCE_BITMAP_UPDATES),
      					B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    
    // Create Slider for Display
    sldDisplaySize = new BSlider(BRect (30,30,250,45), "sldDisplaySize",
      					"Full Screen", new BMessage (SLD_DISPLAYSIZE), 0, 3,
      					B_BLOCK_THUMB, B_FOLLOW_LEFT | B_FOLLOW_TOP,
						B_FRAME_EVENTS|B_WILL_DRAW | B_NAVIGABLE);
	sldDisplaySize->SetHashMarkCount(1);
	sldDisplaySize->SetKeyIncrementValue(1);					
    sldDisplaySize->SetHashMarks(B_HASH_MARKS_BOTH);
    sldDisplaySize->SetLimitLabels("Less", "More");
    sldDisplaySize->SetValue(3);  
    ptrDisplayView->AddChild(sldDisplaySize);
    ptrDisplayView->AddChild(chkForceBitmapUpdates);
    
    // Create StringViews for AboutView
    float fLeftMargin = 8;
    float fDescTop = 60;
    stvTitle = new BStringView(BRect (fLeftMargin,10,200,25), "BeRDP", "BeRDP - BeOS Remote Desktop Client",
      				B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
    stvAuthor1 = new BStringView(BRect (fLeftMargin,30,200,45), "Author 1", "Coded by Sikosis, ...",
      				B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
    stvDescription1 = new BStringView(BRect (fLeftMargin,fDescTop,r.right,fDescTop+13), "Description 1", "BeRDP is a Remote Desktop Protocol Client which is used to",
      				B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
    fDescTop = fDescTop + 13;  				
    stvDescription2 = new BStringView(BRect (fLeftMargin,fDescTop,r.right,fDescTop+13), "Description 2", "connect to Microsoft Windows 2000/2003 Servers and",
      				B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
    fDescTop = fDescTop + 13;  				
    stvDescription3 = new BStringView(BRect (fLeftMargin,fDescTop,r.right,fDescTop+13), "Description 3", "Windows XP Professional workstations.",
      				B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
    stvURL = new BStringView(BRect (fLeftMargin,153,r.right,168), "URL", "Web Site: http://berdp.sf.net/",
      				B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
    ptrAboutView->AddChild(stvAuthor1);
    ptrAboutView->AddChild(stvTitle);
    ptrAboutView->AddChild(stvDescription1); 
    ptrAboutView->AddChild(stvDescription2); 
    ptrAboutView->AddChild(stvDescription3); 				
    ptrAboutView->AddChild(stvURL); 				
        
	// Create the TabView and Tabs
	tabView = new BTabView(rtab,"berdp_tabview");
	tabView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	rtab = tabView->Bounds();
	rtab.InsetBy(5,5);
	tab = new BTab();
	tabView->AddTab(ptrGeneralView, tab);
	tab->SetLabel("General");
	tab = new BTab();
	tabView->AddTab(ptrDisplayView, tab);
	tab->SetLabel("Display");
	tab = new BTab();
	tabView->AddTab(ptrAboutView, tab);
	tab->SetLabel("About BeRDP");
	
	// Create the Views
	AddChild(ptrBeRDPView = new BeRDPView(r));
	ptrBeRDPView->AddChild(tabView);
}
// -------------------------------------------------------------------------------------------------- //


// BeRDPWindow::QuitRequested -- Post a message to the app to quit
bool BeRDPWindow::QuitRequested()
{
	SaveSettings();
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
// -------------------------------------------------------------------------------------------------- //


// BeRDPWindow::LoadSettings -- Loads your current settings
void BeRDPWindow::LoadSettings(BMessage *msg)
{
	BRect     frame;
	BString   tmpC;
	BString   tmpU;
	BString   tmpP;
	BString   tmpD;
	long 	  FBU;
	long      slide;

	if (B_OK == msg->FindRect("windowframe",&frame)) {
		MoveTo(frame.left,frame.top);
		ResizeTo(frame.right-frame.left,frame.bottom-frame.top);
	}
	
	if (B_OK == msg->FindString("txtComputer",&tmpC)) {
		txtComputer->SetText(tmpC.String());
	}
	
	if (B_OK == msg->FindString("txtUsername",&tmpU)) {
		txtUsername->SetText(tmpU.String());
	}
	
	if (B_OK == msg->FindString("txtPassword",&tmpP)) {
		txtPassword->SetText(tmpP.String());
	}
	
	if (B_OK == msg->FindString("txtDomain",&tmpD)) {
		txtDomain->SetText(tmpD.String());
	}
	
	if (B_OK == msg->FindInt32("chkForceBitmapUpdates",&FBU)) {
		if (&FBU != 0) {
			chkForceBitmapUpdates->SetValue(FBU);
    	}	
	}
	if (B_OK == msg->FindInt32("sldDisplaySize",&slide)) {
		sldDisplaySize->SetValue(slide);
	}
}
// -------------------------------------------------------------------------------------------------- //


// BeRDPWindow::SaveSettings -- Saves the Users settings
void BeRDPWindow::SaveSettings(void)
{
	BMessage msg;
	msg.AddRect("windowframe",Frame());
	msg.AddString("txtComputer",txtComputer->Text());
	msg.AddString("txtUsername",txtUsername->Text());
	msg.AddString("txtPassword",txtPassword->Text());
	msg.AddString("txtDomain",txtDomain->Text());
	msg.AddInt32("chkForceBitmapUpdates",chkForceBitmapUpdates->Value());
	msg.AddInt32("sldDisplaySize",sldDisplaySize->Value());
	
	BPath path;
	status_t result = find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	if (result == B_OK) {
		path.Append("BeRDP_Settings",true);
		BFile file(path.Path(),B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);
		msg.Flatten(&file);
	}
}
// -------------------------------------------------------------------------------------------------- //


void BeRDPWindow::UpdateDisplaySlider()
{
	switch (sldDisplaySize->Value())
	{
		case 0:
			{
				sldDisplaySize->SetLabel("640 by 480 pixels");
			}
			break;
		case 1:
			{
				sldDisplaySize->SetLabel("800 by 600 pixels");
			}
			break;
		case 2:
			{
				sldDisplaySize->SetLabel("1024 by 768 pixels");
			}
			break;
		case 3:
			{
				sldDisplaySize->SetLabel("Full Screen");
			}
			break;			
	}
	//printf("Slider Value: %d\n",sldDisplaySize->Value());
}
// -------------------------------------------------------------------------------------------------- //


// BeRDPWindow::MessageReceived -- receives messages
void BeRDPWindow::MessageReceived (BMessage *message)
{
	switch(message->what)
	{
		case BTN_CONNECT:
			{
				SaveSettings();
				BString cmdline;
				cmdline.SetTo("rdesktop ");
				switch (sldDisplaySize->Value())
				{
					case 0:
						{
							cmdline.Append("-g640x480 ");
						}
						break;
					case 1:
						{
							cmdline.Append("-g800x600 ");
						}
						break;
						case 2:
						{
							cmdline.Append("-g1024x768 ");
						}
						break;
					case 3:
						{
							cmdline.Append("-f ");
						}
						break;			
				}
				cmdline.Append("-u");
				cmdline.Append(txtUsername->Text());
				cmdline.Append(" ");
				cmdline.Append("-d");
				cmdline.Append(txtDomain->Text());
				cmdline.Append(" ");
				if (chkForceBitmapUpdates->Value() == B_CONTROL_ON) {
					cmdline.Append("-b ");
				}
				cmdline.Append(txtComputer->Text());
				printf("%s\n\n",cmdline.String());
			}
			break;
		case BTN_CLOSE:
			{
				SaveSettings();
				Quit();
			}
			break;
		case SLD_DISPLAYSIZE:
			{
				UpdateDisplaySlider();
			}
			break;		
		default:
			BWindow::MessageReceived(message);
			break;
	}
}
// -------------------------------------------------------------------------------------------------- //

