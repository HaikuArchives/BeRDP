/*

BeRDP Header

Author: BeRDP

(C)2003 Created using BRIE (http://brie.sf.net/)

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
