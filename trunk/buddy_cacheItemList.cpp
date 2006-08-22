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

#include "buddy_common.h"
#include "buddy_cacheitem.h"
#include "buddy_cacheitemlist.h"

cacheItemList::cacheItemList(void)
{
	m_list = new MAPLIST();
	InitializeCriticalSection(&m_cs);
	m_bIsLocked = false;

	m_hLastAccessItem = NULL;
	m_pLastAccessItem = NULL;
}

cacheItemList::~cacheItemList(void)
{
	if (m_bIsLocked) LOG("~cacheItemList: List is locked/n");
	lock();

	for (MAPITER mit=m_list->begin(); mit!=m_list->end(); mit++)
		delete mit->second;
	delete m_list;

	unlock();
	DeleteCriticalSection(&m_cs);
}

inline void cacheItemList::lock()
{
	EnterCriticalSection(&m_cs);
	m_bIsLocked = true;
}

inline void cacheItemList::unlock()
{
	LeaveCriticalSection(&m_cs);
	m_bIsLocked = false;
}

HRESULT cacheItemList::setItem(cacheItem *pItem, COPYFLAG copyflag, ADDFLAG addflag)
{
	//first check to see that pItem is a valid pointer
	if (!pItem) return E_INVALIDARG;

	lock();

	MAPITER mit;
	mit = m_list->find(pItem->m_hItem);
	if ((mit == m_list->end()) && (addflag == ADD))
	{
		cacheItem *item(NULL);
		if (copyflag==COPY) item = new cacheItem(pItem);
		else item = pItem;
		//pair<MAPITER, bool> ret = list->insert(MAPVALUE(pItem->m_hItem, item));
		//if (!ret.second) (*m_list)[pItem->m_hItem] = item;
		//**** artem, using the subscription is better because insert will only work if there 
		//is no such key already in the map [tom]
		(*m_list)[pItem->m_hItem] = item;
	}
	else
	{
		cacheItem *item(NULL);	
		item = mit->second;
		if (copyflag == COPY) item->copy(pItem);
		else
		{
			delete item;
			mit->second = pItem;
		}		
	}	

	unlock();
	return S_OK;
}