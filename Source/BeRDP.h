/*
 * Copyright 2003-2018. All rights reserved.
 * Distributed under the terms of the BSD license.
 *
 * Author:
 *	Sikosis, phil@sikosis.com
 *
 * BeRDP is a GUI for RDesktop
 */

#ifndef __BeRDP_H__
#define __BeRDP_H__

extern const char *APP_SIGNATURE;

class BeRDP : public BApplication
{
	public:
		BeRDP();
		virtual void MessageReceived(BMessage *message);
	private:

};

#endif
