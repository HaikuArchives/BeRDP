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
#include <Alert.h>
#include <Application.h>
#include <Screen.h>
#include <stdio.h>
#include <Window.h>
#include <View.h>

#include "BeRDPWindows.h"
#include "BeRDPViews.h"
// -------------------------------------------------------------------------------------------------- //

// BeRDPView - Constructor
BeRDPView::BeRDPView (BRect frame) : BView (frame, "BeRDPView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW )
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}
// ------------------------------------------------------------------------------------------------- //

void BeRDPView::Draw(BRect /*updateRect*/)
{
	BRect r;
	r = Bounds();
}
// ------------------------------------------------------------------------------------------------- //


// GeneralView
GeneralView::GeneralView (BRect frame) : BView (frame, "GeneralView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW )
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}
// ---------------------------------------------------------------------------------------------------------- //


// GeneralView - Draw
void GeneralView::Draw(BRect /*updateRect*/)
{
	BRect r;
	r = Bounds();
}
// ---------------------------------------------------------------------------------------------------------- //


// DisplayView
DisplayView::DisplayView (BRect frame) : BView (frame, "DisplayView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW )
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}
// ---------------------------------------------------------------------------------------------------------- //


// AboutView
AboutView::AboutView (BRect frame) : BView (frame, "AboutView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW )
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
}
// ---------------------------------------------------------------------------------------------------------- //

