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

class XMLNodeParser : public XMLNode
{
    friend class XMLDocument;
public:
    XMLNodeParser(XMLDocument * doc);
    ~XMLNodeParser();  //should remove all child nodes   

private:
    XMLNodeParser * ParseNode();
    bool isAlpha(unsigned char Ch);
    bool isAlphaNum(unsigned char Ch);
    char getEntity(const char * Entity);

    XMLDocument *document;

};
