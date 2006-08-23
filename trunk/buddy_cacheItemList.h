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
	File contains declaration of class implements the list of item of cache.

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

#include <map>

typedef std::map<HCITEM, cacheItem*> MAPLIST;
typedef MAPLIST::iterator MAPITER;
typedef MAPLIST::value_type MAPVALUE;

enum ADDFLAG {NOADD=0, ADD};
enum COPYFLAG {NOCOPY=0, COPY};


class cacheItemList 
{
//C'tor & D'tor
public:
	cacheItemList(void);
	~cacheItemList(void);

public:
	inline void lock();						// enter critical section to lock list modification
	inline void unlock();					// leave critical section to unlock list modification
	
	HRESULT addItem(cacheItem *pItem);		// adding copy of item to list, if pItem->hItem 
											// exist already - it will be modified
											// pItem->hItem should be valid
	
	HRESULT setItem(cacheItem *pItem,		    // modify the existed item with handle equal to pItem->hItem 
				    COPYFLAG copyflag = COPY,   // if it is not present 
				    ADDFLAG  addflag = ADD);    // it will be added (add flag), if copy is true - existed item will be 																			
											    // replaced (pItem object should NOT be deleted outside)
											
	
	cacheItem *findItem(HCITEM hItem);		// return pointer to existed item with hItem (Thread UNSAFE) 
	bool getItem(HCITEM hItem, cacheItem *pDestItem);
											// copy existing item with hItem from list to pDestItem 
											// return true if it is ok, if No then pDestItem will be kept unchanged

//Member variables
private:
	CRITICAL_SECTION m_cs;					// critical section for locking
	MAPLIST *m_list;						// list
	bool m_bIsLocked;						// using in unsafe futction to control locking
	HCITEM m_hLastAccessItem;				// internal L1 cache
	cacheItem *m_pLastAccessItem;			// internal L1 cache finding
};
