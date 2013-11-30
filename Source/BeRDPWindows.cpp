/*

BeRDPWindow

Author: Sikosis (phil@sikosis.com)

(C)2003-2004 Shell Created using BRIE (http://brie.sf.net/)

*/

// Includes ------------------------------------------------------------------------------------------ //
#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <CheckBox.h>
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Menu.h>
#include <Path.h>
#include <PopUpMenu.h>
#include <RadioButton.h>
#include <Screen.h>
#include <ScrollView.h>
#include <Slider.h>
#include <stdio.h>
#include <stdlib.h>
#include <String.h>
#include <StringView.h>
#include <TabView.h>
#include <TextView.h>
#include <TextControl.h>
#include <Window.h>
#include <View.h>
#include <iostream.h>
#include <fstream.h>

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
BeRDPWindow::BeRDPWindow(BRect frame) : BWindow (frame, "BeRDP v0.5", B_TITLED_WINDOW, B_NORMAL_WINDOW_FEEL , 0)
{
	InitWindow();
	CenterWindowOnScreen(this);

	// Load User Settings 
	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	path.Append("BeRDP/BeRDP_Settings",true);
	BFile file(path.Path(),B_READ_ONLY);
	BMessage msg;
	msg.Unflatten(&file);
	LoadSettings (&msg);
    
    // Set Window Limits
	SetSizeLimits(315,315,205,230);
    
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
	btnConnect = new BButton(BRect (133,150,213,180), 
					"btnConnect","Connect", new BMessage(BTN_CONNECT),
					B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	btnConnect->MakeDefault(true);
	btnConnect->MakeFocus(true);					
	btnClose = new BButton(BRect (225,150,305,180), 
					"btnClose","Close", new BMessage(BTN_CLOSE),
					B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);	
	ptrGeneralView->AddChild(btnConnect);
    ptrGeneralView->AddChild(btnClose);
    
    
    // Create Connection Menu for GeneralView
    BString DefaultIP;
    DefaultIP.SetTo("Default"); // debug
    
    pmnConnection = new BPopUpMenu("", true, true);
    pmnConnection->AddItem(new BMenuItem(" ... ", new BMessage(MENU_CONNECTION_ELLIPSIS)));
    pmnConnection->AddItem(new BMenuItem(DefaultIP.String(), new BMessage(MENU_CONNECTION_DEFAULT)));
    pmnConnection->AddSeparatorItem();
    pmnConnection->AddItem(new BMenuItem("New Connection", new BMessage(MENU_NEW_CONNECTION)));
    pmnConnection->AddItem(mniConnectionDelete = new BMenuItem("Delete Current", new BMessage(MENU_CON_DELETE_CURRENT)));
    mniConnectionDelete->SetEnabled(false);

    mnfConnection = new BMenuField(BRect(5,25,220,40), "connection_menu", "Connection Name:", pmnConnection, true);
	mnfConnection->SetDivider(110);
	mnfConnection->SetFont(be_bold_font);
	mnfConnection->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
    /*conmenufield = new BMenu(" ... ");
    conmenufield->AddItem(new BMenuItem(DefaultIP.String(), new BMessage(MENU_CONNECTION_DEFAULT)));
    conmenufield->AddSeparatorItem();
    conmenufield->AddItem(new BMenuItem("New Connection", new BMessage(MENU_NEW_CONNECTION)));
    conmenufield->AddItem(menucondelete = new BMenuItem("Delete Current", new BMessage(MENU_CON_DELETE_CURRENT)));
    menucondelete->SetEnabled(false);
    connectionmenufield = new BMenuField(BRect (5,25,220,40),"connection_menu","Connection Name:",conmenufield,B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    connectionmenufield->SetDivider(110);	
    connectionmenufield->SetFont(be_bold_font);
    connectionmenufield->SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));*/
    
    // Create TextControls for GeneralView
    txtComputer = new BTextControl(BRect (30,50,220,65), "txtComputer",
      					"Computer: ", "", new BMessage (TXT_COMPUTER),
						B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtComputer->SetDivider(55);
	txtUsername = new BTextControl(BRect (30,75,250,90), "txtUsername",
      					"Username: ", "", new BMessage (TXT_USERNAME),
						B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtUsername->SetDivider(55);					
    
    txtPassword = new BTextControl(BRect (30,100,250,115), "txtPassword",
      					"Password: ", "", new BMessage (TXT_PASSWORD),
						B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtPassword->SetDivider(55);					
    txtPassword->SetEnabled(false);
    
    txtDomain = new BTextControl(BRect (30,125,250,140), "txtDomain",
      					"Domain: ", "", new BMessage (TXT_DOMAIN),
						B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
	txtDomain->SetDivider(55);

	ptrGeneralView->AddChild(mnfConnection);
	ptrGeneralView->AddChild(txtComputer);
	ptrGeneralView->AddChild(txtUsername);					
    ptrGeneralView->AddChild(txtPassword);					
    ptrGeneralView->AddChild(txtDomain);
    
    // Create CheckBox for Display
    chkForceBitmapUpdates = new BCheckBox(BRect (30,69,300,79), "chkForceBitmapUpdates",
      					"Force Bitmap Updates", new BMessage (CHK_FORCE_BITMAP_UPDATES),
      					B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE);
    
    // Create Slider for Display
    sldDisplaySize = new BSlider(BRect (20,15,r.right - 20,30), "sldDisplaySize",
      					"Full Screen", new BMessage (SLD_DISPLAYSIZE), 0, 5,
      					B_BLOCK_THUMB, B_FOLLOW_LEFT | B_FOLLOW_TOP,
						B_FRAME_EVENTS|B_WILL_DRAW | B_NAVIGABLE);
	sldDisplaySize->SetHashMarkCount(1);
	sldDisplaySize->SetKeyIncrementValue(1);					
    sldDisplaySize->SetHashMarks(B_HASH_MARKS_BOTH);
    sldDisplaySize->SetLimitLabels("Less", "More");
    sldDisplaySize->SetValue(3);  
    
   // int wsleft = 175;
    
    // Add them all to Display Tab
    ptrDisplayView->AddChild(sldDisplaySize);
    ptrDisplayView->AddChild(chkForceBitmapUpdates);
          
    // Create StringViews for AboutView
    float fLeftMargin = 8;
    float fRightMargin = 310;
    float fDescTop = 60;
    stvTitle = new BStringView(BRect (fLeftMargin,10,fRightMargin,25), "BeRDP", "BeRDP - BeOS Remote Desktop Client",
      				B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
    stvAuthor1 = new BStringView(BRect (fLeftMargin,30,fRightMargin,45), "Author 1", "Coded by Sikosis",
      				B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
      				
    
	txvDescription = new BTextView(BRect(fLeftMargin, fDescTop, fRightMargin, fDescTop+80),
						 "Description", BRect(0,0,fRightMargin-fLeftMargin,80), B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	txvDescription->SetWordWrap(true);
	txvDescription->MakeEditable(true);
	txvDescription->SetStylable(true);
	txvDescription->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	txvDescription->Insert("BeRDP is a GUI for RDesktop, which is used to connect to Microsoft Remote Desktop Protocol servers.\n");
      				
    stvURL = new BStringView(BRect (fLeftMargin,153,r.right,168), "URL", "Website: https://github.com/HaikuArchives/BeRDP",
      				B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
    ptrAboutView->AddChild(stvAuthor1);
    ptrAboutView->AddChild(stvTitle);
    ptrAboutView->AddChild(txvDescription); 
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
	BString   tmpWSP;
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


// BeRDPWindow::SaveConnectionList -- Saves the List of Conection Names from conmenufield
void BeRDPWindow::SaveConnectionList(void)
{
	BMessage msg;
	int TotalMenuItems, Counter, NumberOfConnections;
	
	TotalMenuItems = pmnConnection->CountItems();
	
	if (TotalMenuItems > 5) {
		NumberOfConnections =  TotalMenuItems - 5;
		msg.AddInt32("NumberOfConnections",NumberOfConnections);
		BString OneItem;
		Counter=0;
		while(Counter<TotalMenuItems) {
			OneItem.SetTo(pmnConnection->ItemAt(Counter)->Label());
			Counter++;
		}
		// Save Connection List
		BPath path;
		status_t result = find_directory(B_USER_SETTINGS_DIRECTORY,&path);
		if (result == B_OK) {
			path.Append("BeRDP/BeRDP_Connection_List",true);
			BFile file(path.Path(),B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);
			msg.Flatten(&file);
		}
	} else {
		// Default Connection List - No Need to Save
	}
}
// -------------------------------------------------------------------------------------------------- //


// BeRDPWindow::SaveConnectionDetails -- Saves the Users settings
void BeRDPWindow::SaveConnectionDetails(const char *cnxname)
{
	BMessage msg;
	msg.AddString("txtComputer",txtComputer->Text());
	msg.AddString("txtUsername",txtUsername->Text());
	msg.AddString("txtPassword",txtPassword->Text());
	msg.AddString("txtDomain",txtDomain->Text());
	msg.AddInt32("chkForceBitmapUpdates",chkForceBitmapUpdates->Value());
	msg.AddInt32("sldDisplaySize",sldDisplaySize->Value());
	
	BPath path;
	status_t result = find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	if (result == B_OK) {
		// Check for BeRDP Dir
		status_t err;
		BString cnxfilename;
		cnxfilename.SetTo(path.Path());
		cnxfilename.Append("/BeRDP");
		BEntry entry(cnxfilename.String());
		if (entry.Exists() == false) {
			// Create Directory
			path.SetTo(cnxfilename.String());
			err = create_directory(path.Path(),777);
			if (result != B_OK) {
				printf("Couldn't create directory &i\n",err);
			}
		}
		cnxfilename.Append("/");
		cnxfilename.Append(cnxname);
		cnxfilename.Append(".berdp");
		
		//BMenuItem *CurrentItem = new BMenuItem(conmenufield);
		//CurrentItem = conmenufield->FindMarked();
		//printf("Save Connection Details Filename: %s / Total Items: %d\n\n",CurrentItem->Label(),
		//	conmenufield->CountItems());
		path.SetTo(cnxfilename.String());
		BFile file(path.Path(),B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);
		msg.Flatten(&file);
	}
}
// -------------------------------------------------------------------------------------------------- //


// BeRDPWindow::SaveSettings -- Saves the Users settings
void BeRDPWindow::SaveSettings(const char *cnxname)
{
	BMessage msg;
	msg.AddRect("windowframe",Frame());
	msg.AddString("cnxname",cnxname);
	
	// these will be moved to the function above shortly
	//msg.AddString("txtComputer",txtComputer->Text());
	//msg.AddString("txtUsername",txtUsername->Text());
	//msg.AddString("txtPassword",txtPassword->Text());
	//msg.AddString("txtDomain",txtDomain->Text());
	//msg.AddInt32("chkForceBitmapUpdates",chkForceBitmapUpdates->Value());
	//msg.AddInt32("sldDisplaySize",sldDisplaySize->Value());
	
	BPath path;
	status_t result = find_directory(B_USER_SETTINGS_DIRECTORY,&path);
	if (result == B_OK) {
		path.Append("BeRDP/BeRDP_Settings",true);
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
			sldDisplaySize->SetLabel("320 by 240 pixels");
			break;
		case 1:
			sldDisplaySize->SetLabel("640 by 480 pixels");
			break;
		case 2:
			sldDisplaySize->SetLabel("800 by 600 pixels");
			break;
		case 3:
			sldDisplaySize->SetLabel("1024 by 768 pixels");
			break;
		case 4:
			sldDisplaySize->SetLabel("1280 by 720 pixels");
			break;
		case 5:
			sldDisplaySize->SetLabel("Full Screen");
			break;			
	}
}
// -------------------------------------------------------------------------------------------------- //


// BeRDPWindow::MessageReceived -- receives messages
void BeRDPWindow::MessageReceived (BMessage *message)
{
	BString cnxname;
	int index;

	BMenuItem *CurrentItem = new BMenuItem(pmnConnection);
	CurrentItem = pmnConnection->FindMarked();
	index = pmnConnection->IndexOf(CurrentItem);
	if (index > -1) {
		cnxname.SetTo(pmnConnection->ItemAt(index)->Label()); // debug
	} else {
		cnxname.SetTo("");
	}	
	
	switch(message->what)
	{
		case BTN_CONNECT:
			{
				SaveConnectionDetails(cnxname.String()); // debug
				SaveConnectionList();
				SaveSettings(cnxname.String());
				
				BString cmdline;
				cmdline.SetTo("rdesktop ");
				switch (sldDisplaySize->Value())
				{
				case 0:
					cmdline.Append("-g 320x240 ");
					break;
				case 1:
					cmdline.Append("-g 640x480 ");
					break;
				case 2:
					cmdline.Append("-g 800x600 ");
					break;
				case 3:
					cmdline.Append("-g 1024x768 ");
					break;
				case 4:
					cmdline.Append("-g 1280x720 ");
					break;
				case 5:
					cmdline.Append("-f ");
					break;			
				}
				
				BString tmpUsername;
				tmpUsername.SetTo(txtUsername->Text());
				if (tmpUsername.CountChars() > 0) {
					cmdline.Append("-u");
					cmdline.Append(txtUsername->Text());
					cmdline.Append(" ");
				}
				cmdline.Append("-d");
				cmdline.Append(txtDomain->Text());
				cmdline.Append(" ");
				if (chkForceBitmapUpdates->Value() == B_CONTROL_ON) {
					cmdline.Append("-b ");
				}
				cmdline.Append(txtComputer->Text());

				// Minimize the Window
				Minimize(true);
				
				// Execute RDesktop
				system(cmdline.String());
				
				// Now Show it Again
				Minimize(false);
			}
			break;
		case MENU_NEW_CONNECTION:
			{
				// Launch Window - Maybe my InputBox Class ?
				
				BMenuItem *marked = pmnConnection->FindItem("New Connection");
				marked->SetMarked(true);
			}
			break;
		case MENU_CONNECTION_ELLIPSIS:
			{
				// debug info for testing 
				// this will eventually stored the last known config
				txtComputer->SetText("61.88.23.139");
				txtUsername->SetText("admin");
				txtDomain->SetText("WENCK");
				sldDisplaySize->SetValue(3);
				chkForceBitmapUpdates->SetValue(B_CONTROL_ON);
			}
			break;		
		case MENU_CONNECTION_DEFAULT:
			{
				// Set the Default Connections Settings
				txtComputer->SetText("");
				txtUsername->SetText("");
				txtPassword->SetText("");
				txtDomain->SetText("");
				sldDisplaySize->SetValue(3);
				chkForceBitmapUpdates->SetValue(B_CONTROL_OFF);
				
				BMenuItem *marked = pmnConnection->FindItem("Default");
				marked->SetMarked(true);
			}
			break;
		case BTN_CLOSE:
			{
				//BString cnxname;
				// debug - should be the current selected item
				//cnxname.SetTo(conmenufield->ItemAt(conmenufield->CountItems())->Label()); // debug
				SaveSettings(cnxname.String());
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
