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
	File contains routines to register/unregister Buddy list plugin
    services and event handlers	

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

int RegisterServicesOnStart()
{ 
	return 0;
}

int UnregisterServicesOnExit()
{
	return 0;
}

int RegisterEventsOnStart()
{
	return 0;
}

int UnegisterEventsOnExit()
{
	return 0;
}

int RegisterEventHooksOnStart()
{
	return 0;
}

int UnegisterEventHooksOnExit()
{
	return 0;
}