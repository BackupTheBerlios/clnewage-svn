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

#include "XMLParser.h"  //XMLDocument already included there
#include "XMLNodeParser.h"

XMLDocument::XMLDocument()
{
    int currentLine=0;
    int currentColumn=0;
    char * buffer=0;
    char * bufferPos=0;
    char currentChar=0;
    hasError=0;
}

XMLDocument::~XMLDocument()
{
    for (list<XMLNode*>::const_iterator iter=Childs.begin(); iter!=Childs.end(); ++iter)
    {
        XMLNode* Node=*iter;
        delete Node;
    }
}
int XMLDocument::ParseFromBuffer(const char * buf)
{
    if (buffer!=NULL) 
    {
        hasError=true;
        Error="Already parsed";
        return hasError;
    }
    buffer=(char*) buf;
    bufferPos=buffer;
    currentChar=*bufferPos;
    currentLine=1;
    currentColumn=1;
    return ParseDocument();
}
int XMLDocument::ParseDocument()
{
    char Ch;
    SkipWhiteSpaces();
    while (currentChar!='\0' && !hasError)
    {
        XMLNodeParser * Node=new XMLNodeParser(this);
        Ch=GetCh();
        if (Ch=='<')
        {
            NextChar();
            if (Node->ParseNode())
            {
                if (Node->type!=Type_Unknown) Childs.push_back(Node);
                SkipWhiteSpaces();
            }
            else
            {
                delete Node;
                break;
            }
        }
        else 
        {
            hasError=true;
            Error="Document should consist only from nodes rounded with <name /> or <name></name> tags";
            break;
        }
    }
    return (int)hasError;
}

char XMLDocument::GetCh()
{
    return currentChar;
}

char XMLDocument::NextChar()
{
    if (buffer!=NULL)  //read data from static buffer
    {
        bufferPos++;
        if (*bufferPos=='\n') 
        {
            currentLine++;
            currentColumn=1;
        }
        else if (*bufferPos=='\t') currentColumn+=4;  //tabs assumed 4 spaces width
        else if (*bufferPos!='\r') currentColumn++;
        currentChar=*bufferPos;
        return currentChar;
    }
    else
    {
        //read char from file
    }
    return 0;
}

void XMLDocument::SkipWhiteSpaces()
{
    while(strchr(" \t\r\n", (int)currentChar) && currentChar!='\0') NextChar();
}

