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
	File contains declaration of class implements the item of cache.

    Created by Artem Shpynov mailto:ashpynov@gmail.com August 21,2006
*/

/*
    ------------------------------------------------------------------------
			                      FILE HISTORY
	------------------------------------------------------------------------
	Author				DATE		Comment
	------------------------------------------------------------------------
	A. Shpynov (FYR)	08/21/06	Initial Release

*/
#pragma once

#include "buddy_common.h"

typedef unsigned int HCITEM;

class cacheItem
{
//C'tor & D'tor
public:
	cacheItem();
	cacheItem(const cacheItem *from);
	~cacheItem();

//Internal functions
public:
	void init();
	void copy(const cacheItem *fromItem);		// copy all fields of fromItem to this item.

//member variables
public:
	HCITEM			m_hItem;				// unique indentifier of item
	TCHAR			*m_lpName;				// buddy display name (should be copied)
	const TCHAR		*m_lpProtocol;			// buddy protocol (pointer to constant)
	int				m_nStatus;				// current status of contact
};

