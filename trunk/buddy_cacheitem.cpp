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
	File class implements the item of cache.

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
#include "buddy_common.h"
#include "buddy_cacheitem.h"

//internally used to initialize variables
void cacheItem::init()
{
	m_hItem = 0;
	m_lpName = NULL;
	m_lpProtocol = NULL;
	m_nStatus = 0;
}

cacheItem::cacheItem()
{
	init();
}

cacheItem::cacheItem(const cacheItem *from)
{ 
	init();
	copy(from);
}

cacheItem::~cacheItem()
{
	if (m_lpName) delete m_lpName;
}

void cacheItem::copy(const cacheItem *fromItem)
{
	if (!fromItem) 
	{   // if null item is given copy of data of new item object
		// LOG("cacheItem::copy: Empty fromItem is given")
		cacheItem *tempItem=new cacheItem();
		copy(tempItem);
		delete tempItem;
		return;
	}
	//  else
	this->m_hItem=fromItem->m_hItem;
	if (m_lpName) 
	{
		delete m_lpName;
		m_lpName = NULL;
	}
	if (fromItem->m_lpName)
		m_lpName = _tcsdup(fromItem->m_lpName);
	
	m_lpProtocol = fromItem->m_lpProtocol;
	m_nStatus = fromItem->m_nStatus;
	return;
}


