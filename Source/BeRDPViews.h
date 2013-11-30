/*

BeRDP Views Header

Author: BeRDP

(C)2003 Created using BRIE (http://brie.sf.net/)

*/

#ifndef __BeRDPVIEWS_H__
#define __BeRDPVIEWS_H__

#include "BeRDP.h"
#include "BeRDPWindows.h"

class BeRDPView : public BView
{
	public:
		BeRDPView(BRect frame);
		virtual void Draw(BRect frame);
};

class GeneralView : public BView
{
	public:
		GeneralView(BRect frame);
		virtual void Draw(BRect frame);
};

class DisplayView : public BView
{
	public:
		DisplayView(BRect frame);
};

class XWindowsView : public BView
{
	public:
		XWindowsView(BRect frame);
};

class AboutView : public BView
{
	public:
		AboutView(BRect frame);
};

#endif



