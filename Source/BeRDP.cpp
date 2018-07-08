/*
 * Copyright 2003-2018. All rights reserved.
 * Distributed under the terms of the BSD license.
 *
 * Author:
 *	Sikosis, phil@sikosis.com
 *
 * BeRDP is a GUI for RDesktop
 */


// Includes ------------------------------------------------------------------------------------------ //
#include <Application.h>
#include <Screen.h>
#include <Window.h>
#include <View.h>

#include "BeRDP.h"
#include "BeRDPWindows.h"
#include "BeRDPViews.h"
#include "BeRDPConstants.h"
// Constants ---------------------------------------------------------------------------------------- //

const char *APP_SIGNATURE = "application/x-vnd.BeRDP";  // Application Signature

// -------------------------------------------------------------------------------------------------- //

BeRDPWindow		*ptrBeRDPWindow;

// BeRDP - Constructor
BeRDP::BeRDP() : BApplication (APP_SIGNATURE)
{
	// Default Window Size - Position doesn't matter as we centre the form to the current screen size
	//BRect	screenFrame = (BScreen(B_MAIN_SCREEN_ID).Frame());

	float FormTopDefault = 0;
	float FormLeftDefault = 0;
	float FormWidthDefault = 315;
	float FormHeightDefault = 205;
	BRect BeRDPWindowRect(FormTopDefault,FormLeftDefault,FormLeftDefault+FormWidthDefault,FormTopDefault+FormHeightDefault);

	ptrBeRDPWindow = new BeRDPWindow(BeRDPWindowRect);
}
// ------------------------------------------------------------------------------------------------- //

// BeRDP::MessageReceived -- handles incoming messages
void BeRDP::MessageReceived (BMessage *message)
{
	switch(message->what)
	{
		default:
			BApplication::MessageReceived(message); // pass it along ...
			break;
	}
}
// ------------------------------------------------------------------------------------------------- //

// BeRDP Main
int main(void)
{
	BeRDP theApp;
	theApp.Run();
	return 0;
}
// end --------------------------------------------------------------------------------------------- //

