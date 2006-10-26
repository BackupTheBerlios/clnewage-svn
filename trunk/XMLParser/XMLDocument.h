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

#include <string>
#include <map>
#include <list>

using namespace std;
class XMLDocument
{  
    friend class XMLNodeParser;
public:
    XMLDocument();
    ~XMLDocument(); 
    //Parsers
    int ParseFromBuffer(const char * buf);    
    int ParseFromFile(const char * filename); 
    int GetError(char * buffer, size_t bufferSize);

    //Node Navigators (it will also provide node interfaces
    class XMLNode * GetNextSiblingNode(class XMLNode *);

private:
    int currentLine;
    int currentColumn;
    char * buffer;
    char * bufferPos;
    char currentChar;
    int ParseDocument();
    void SkipWhiteSpaces();
    char GetCh();
    char NextChar();   
    bool hasError;
    string Error;
    list<class XMLNode*> Childs;
};



