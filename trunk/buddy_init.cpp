/*
Miranda IM: the free IM client for Microsoft* Windows*

Copyright 2000-2006 Miranda ICQ/IM project, 
all portions of this codebase are copyrighted to the people 
listed in contributors.txt.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*
	File contains Miranda plugin API functions.

    Created by Artem Shpynov mailto:ashpynov@gmail.com July 28,2006
*/

/*
    ------------------------------------------------------------------------
			                      FILE HISTORY
	------------------------------------------------------------------------
	Author				DATE		Comment
	------------------------------------------------------------------------
	A. Shpynov (FYR)	07/28/06	Initial Release

*/

#include "buddy_common.h"
#include <m_clist.h>
#include <m_skin.h>

/*		Local Prototypes	 */

// None

/* ------------------------- */

/*		Global Prototypes	 */

int RegisterServicesOnStart();
int RegisterEventsOnStart();
int RegisterEventHooksOnStart();

int UnregisterServicesOnExit();
int UnegisterEventsOnExit();
int UnegisterEventHooksOnExit();

/* ------------------------- */

// EXTERNAL VARIABLES
HINSTANCE gl_hInst = NULL;
PLUGINLINK *pluginLink = NULL;
BYTE gl_bBuddyListState = 0;

PLUGININFO pluginInfo = {
	sizeof(PLUGININFO),
#if defined( _UNICODE )
	"Buddy List (Unicode)",
#else
	"Buddy List",
#endif
	PLUGIN_MAKE_VERSION(0,0,0,1),
	"New generation of contact list plugin",
	"Artem Shpynov",
	"ashpynov@gmail.ru",
	"© 2006 Artem Shpynov",
	"http://modern.saopp.info/",
	0,		//not transient
	0		//doesn't replace anything built-in
};

//DEB_START
#include "buddy_cacheitem.h"
#include "buddy_cacheitemlist.h"
//DEB_END

static int PluginMenuCommand(WPARAM wParam,LPARAM lParam)
{
	MessageBox(NULL,_TEXT("Just groovy, baby!"),_T("Plugin-o-rama"),MB_OK);
	return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	gl_hInst = hinstDLL;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
//       MirandaPluginInfo - initialize the plugin instance                  //
///////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) PLUGININFO* MirandaPluginInfo(DWORD mirandaVersion)
{
	if ( mirandaVersion < PLUGIN_MAKE_VERSION( 0,4,0,4 )) {
		MessageBoxA( NULL, "The Buddy list plugin cannot be loaded. It requires Miranda IM 0.4.0.3 or later.", "Buddy List Plugin", MB_OK|MB_ICONWARNING|MB_SETFOREGROUND|MB_TOPMOST );
		return NULL;
	}
	return &pluginInfo;
}
///////////////////////////////////////////////////////////////////////////////
//                Load - initialize the plugin instance                      //
///////////////////////////////////////////////////////////////////////////////
extern "C" int __declspec(dllexport) Load(PLUGINLINK *link)
{
	pluginLink=link;

	RegisterServicesOnStart();
	RegisterEventsOnStart();
	RegisterEventHooksOnStart();

// TODO REMOVE:
	cacheItemList *list = new cacheItemList();
	
	cacheItem *it = new cacheItem();
	it->m_hItem = 12345;
	it->m_lpName = _tcsdup(_T("Test"));
	it->m_nStatus = 2000;

	cacheItem *it2 = new cacheItem(it);
	it2->m_nStatus++;
	delete it2;

	list->setItem(it);

	delete it;
	delete list;
	{
		CreateServiceFunction("buddy/MenuCommand", PluginMenuCommand);
		CLISTMENUITEM mi;
		ZeroMemory(&mi, sizeof(mi));
		mi.cbSize = sizeof(mi);
		mi.position = -0x7FFFFFFF;
		mi.flags = 0;
		mi.hIcon = LoadSkinnedIcon(SKINICON_OTHER_MIRANDA);
		mi.pszName = "&Test Plugin...";
		mi.pszService = "buddy/MenuCommand";
		CallService(MS_CLIST_ADDMAINMENUITEM, 0, (LPARAM)&mi);
	}
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//            Unload - deinitialize the plugin instance                      //
///////////////////////////////////////////////////////////////////////////////
extern "C" int __declspec(dllexport) Unload(void)
{
	UnregisterServicesOnExit();
	UnegisterEventsOnExit();
	UnegisterEventHooksOnExit();
	return 0;
}