/**************************************************************************\
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

----------------------------------------------------------------------------
Created October 26, 2006

Author Artem Shpynov aka FYR:  ashpynov@gmail.com

\**************************************************************************/

#include "XMLDocument.h"

#include <map>
#include <list>
using namespace std;
enum NODE_TYPES
{
    Type_Unknown=0,
    Type_Comment,
    Type_Declaration,
    Type_Normal,
    Type_Text           //text is node too, without attributes and Name
};

class XMLNode //Interface class
{
public:
    string Text;
    int type;
    list<class XMLNode*> Childs;
    map<string,string> Attributes;
};

