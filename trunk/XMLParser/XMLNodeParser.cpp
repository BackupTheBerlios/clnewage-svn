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

#include "XMLParser.h"
#include "XMLNodeParser.h"

enum PARSER_STATES
{
    State_Start=0,
    State_OpenBracket,          //we are on '<'
    State_PossibleComment,      //we are on '!--'
    State_Comment,              //we are inside '<!--'
    State_PossibleCommentEnd,   //we are on -->
    State_Declaration,
    State_OnName,               //we are on node name start
    State_WaitAttribute,        //inside node opening
    State_OnAttribName,         //
    State_ReadEqualMark,
    State_ReadAttribute,
    State_ReadEntity,
    State_InsideNode,
    State_NodeClosure,
    State_NodeDone

};



XMLNodeParser::XMLNodeParser(XMLDocument * doc)
{
    type=Type_Unknown;
    document=doc;
}


XMLNodeParser::~XMLNodeParser()
{
    for (list<XMLNode*>::const_iterator iter=Childs.begin(); iter!=Childs.end(); ++iter)
    {
        XMLNode* Node=*iter;
        delete Node;
    }
}


bool XMLNodeParser::isAlpha(unsigned char Ch)
{ return ((Ch>='A' && Ch<='Z') || (Ch>='a' && Ch<='z') ); }

bool XMLNodeParser::isAlphaNum(unsigned char Ch)
{ return ( (Ch>='A' && Ch<='Z') || (Ch>='a' && Ch<='z') || (Ch>='0' && Ch<='9') ); }

char XMLNodeParser::getEntity(const char * Entity)
{
    static struct  
    {
        char* entity; 
        char value;
    } EntityTable[]=
    {
        { "&amp;",      '&'     },
        { "&quot;",     '\"'    },
        { "&lt;",       '<'     },
        { "&gt;",       '>'     },        
        { "&apos;",     '\''    }
    };
    for (int i=0; i<sizeof(EntityTable)/sizeof(EntityTable[0]); i++)
        if (!stricmp(Entity,EntityTable[i].entity)) return EntityTable[i].value;
    //not found may be like '&#x10;' ?
    if (Entity[1]=='#')
    {
        long code=0;
        char *last=NULL;
        if (Entity[2]=='x' || Entity[2]=='X')
        {
            code=strtol(Entity+3,&last,16);  // decode hex
        }
        else if (Entity[2]!='\0' && Entity[2]!=';' )
        {
            code=strtol(Entity+2,&last,10);  // decode decimal
        }
        if (last && *last==';' && code<256) return (char)code;            
    }
    return (char)0;
}

XMLNodeParser * XMLNodeParser::ParseNode()
{
    //int curpos=0;
    char Ch;
    char ValueBraket=' ';
    string AttribBuffer;
    string ValueBuffer;
    string EntityBuffer;
    string NameBuffer;
    char last;
    int state=State_OpenBracket;
    int returnstate=State_Start;
    int stateiteration=0;
    document->SkipWhiteSpaces();
    Ch=document->GetCh();


    while (Ch!='\0' && !document->hasError && state!=State_NodeDone)
    {
        switch (state)
        {
        case State_OpenBracket:
            {
                //we should be always after '<' at this point
                if (Ch=='!') 
                {
                    state=State_PossibleComment;
                    stateiteration=1;
                    continue;
                }
                else if (Ch=='?')
                {
                    state=State_Declaration;
                    type=Type_Declaration;
                    document->NextChar();  //eat "?"
                    continue;
                }
                else if (isAlpha(Ch))
                {
                    state=State_OnName;
                    type=Type_Normal;
                    continue;
                }
                else 
                {
                    document->hasError=true;
                    document->Error="Error: '";
                    document->Error+=Ch;
                    document->Error+="' is not valid start of node name";
                }
            }
            break; /* case State_OpenBracket */
        case State_PossibleComment:
            {
                Ch=document->NextChar();
                if (Ch=='-' && stateiteration< 3) 
                { 
                    stateiteration++; 
                    continue; 
                }
                else if (stateiteration==3) 
                {
                    state=State_Comment;
                    type=Type_Comment;
                    continue;
                }
                else 
                {
                    document->hasError=true;
                    document->Error="Error: '<!--' only valid comment tag, overs '<! not supported";
                    break;
                }

            }
            break; /* case State_PossibleComment */
        case State_Comment:
            {
                Text+=Ch;
                Ch=document->NextChar();
                if (Ch=='-')
                {
                    state=State_PossibleCommentEnd;
                    stateiteration=1; 
                    continue;
                }
                else
                    continue;
            }
            break; /* case State_Comment */
        case State_PossibleCommentEnd:
            {                
                Ch=document->NextChar();
                if (Ch=='-' && stateiteration<2)
                {
                    stateiteration++; 
                    continue;
                }
                else if (stateiteration==2 && Ch=='>')
                {
                    state=State_NodeDone;
                    document->NextChar(); //eat '>' at the end
                    continue;
                }
                else 
                {
                    state=State_Comment;
                    if (stateiteration==1) Text+="-";
                    else if (stateiteration==2) Text+="--";
                    continue;
                }
            }
            break; /* case State_PossibleCommentEnd */
        case State_Declaration:
            {
                char oldch=document->GetCh();
                Ch=document->NextChar();               
                if (oldch=='?' && Ch=='>')
                {
                    document->NextChar();
                    state=State_NodeDone;
                    break;
                }
                else 
                {
                    Text+=oldch;
                    continue;
                }
            }
            break; /* case State_Declaration */
        case State_OnName:
            {
                char oldch=document->GetCh();                
                Ch=document->NextChar();
                if (oldch=='/' && Ch=='>')
                {
                    state=State_NodeDone;
                    document->NextChar();
                    break;
                }
                Text+=oldch;
                if (isAlphaNum(Ch) || strchr("_-.:",(int)Ch)) 
                {
                    continue;
                }
                else if (Ch=='>')
                {
                    state=State_InsideNode;
                    document->NextChar();           //eat '>'
                    document->SkipWhiteSpaces();    //skip white between text
                    continue;
                } 
                else if (Ch==' ') //name finished
                {
                    state=State_WaitAttribute;
                    continue;                    
                }
                else
                {
                    document->hasError=true;
                    document->Error="Error: '";
                    document->Error+=Ch;
                    document->Error+="' is not valid in node name";
                }
            }
            break; /* case State_OnName */
        case State_WaitAttribute:
            {
                document->SkipWhiteSpaces();    //skip white before attribute
                Ch=document->GetCh();
                if (isAlpha(Ch))
                {
                    state=State_OnAttribName;
                    AttribBuffer="";
                    ValueBuffer="";
                    continue;
                }
                else if (Ch=='/' && document->NextChar()=='>')
                {
                    //end node
                    state=State_NodeDone;
                    document->NextChar();
                    break;
                }
                else if (Ch=='>')
                {
                    state=State_InsideNode;
                    ValueBuffer="";
                    document->NextChar();           //eat '>'
                    document->SkipWhiteSpaces();    //skip white between text
                    continue;
                }
                else 
                {
                    document->hasError=true;
                    document->hasError=true;
                    document->Error="Error: '";
                    document->Error+=Ch;
                    document->Error+="' is not valid in attribute name";
                    break;
                }
            }
            break; /* State_WaitAttribute */
        case State_OnAttribName:
            {
                Ch=document->GetCh();
                if (isAlphaNum(Ch) || strchr("_-.:",(int)Ch))
                {
                    AttribBuffer+=Ch;
                    document->NextChar();
                    continue;
                }
                else if (Ch=='=' || Ch==' ')
                {
                    state=State_ReadEqualMark;
                    continue;
                }
                else
                {
                    document->hasError=true;
                    document->Error="Error: '";
                    document->Error+=Ch;
                    document->Error+="' is not valid in attribute name";
                    break;
                }

            }
            break; /* State_OnAttribName */
        case State_ReadEqualMark:
            {
                document->SkipWhiteSpaces();
                Ch=document->GetCh();
                if (Ch !='=') 
                {
                    document->hasError=true;
                    document->Error="Error: '";
                    document->Error+=Ch;
                    document->Error+="' is not valid here (should be '='";
                    break;
                }
                else
                {
                    document->NextChar();
                    document->SkipWhiteSpaces();
                    Ch=document->GetCh();
                    if (Ch=='\'' || Ch=='\"') 
                    {
                        ValueBraket=Ch;
                        document->NextChar();
                    }
                    else 
                    {
                        // real XML does not support attribute in form: x=10 
                        // it should be x="10" or x='10' but we will support it on reading
                        ValueBraket=' ';  
                    }
                    state=State_ReadAttribute;
                    continue;
                }
            }
            break; /* case State_ReadEqualMark */
        case State_ReadAttribute:
            {
                Ch=document->GetCh();
                if (Ch!=ValueBraket && !(ValueBraket==' ' && strchr("\\> \t\r\n",(int)Ch)))
                {
                    if (Ch=='\n' || Ch=='\r')
                    {
                        document->hasError=true;
                        document->Error="Error new line in value";
                        break;
                    }
                    else if (Ch=='&')
                    {
                        //read &amp; or smth
                        returnstate=State_ReadAttribute;
                        state=State_ReadEntity;
                        EntityBuffer=Ch;
                        document->NextChar(); //eat '&'
                        continue;
                    }
                    else
                    {
                        ValueBuffer+=Ch;
                        Ch=document->NextChar();
                        continue;
                    }
                }
                else //if (Ch==ValueBraket || (ValueBraket==" " && strchr("\\> \t\r\n",(int)Ch)))
                {
                    //end of attribute
                    Attributes[AttribBuffer]=ValueBuffer;                    
                    if (Ch==ValueBraket) document->NextChar(); //eat last char
                    state=State_WaitAttribute;
                    continue;
                }
            }
            break; /* State_ReadAttribute */
        case State_ReadEntity:
            {
                Ch=document->GetCh();
                if (isAlphaNum(Ch) || (Ch=='#' && EntityBuffer.length()==1)) 
                {
                    EntityBuffer+=Ch;
                    document->NextChar();
                    continue;
                }
                else if (Ch==';')
                {
                    EntityBuffer+=Ch;
                    char ent=getEntity(EntityBuffer.c_str());
                    if (ent=='\0') 
                        ValueBuffer+=EntityBuffer;
                    else
                        ValueBuffer+=ent;
                    document->NextChar();
                    state=returnstate;
                    continue;
                }
                else 
                {
                    document->hasError=true;
                    document->Error="Error: '";
                    document->Error+=Ch;
                    document->Error+="' is not valid in entity";
                    break;
                }
            }
            break; /* State_ReadEntity */
        case State_InsideNode:
            {
                last=document->GetCh();
                Ch=document->NextChar();
                if (last=='<')                        
                {
                    if (ValueBuffer.length()>0)
                    {
                        XMLNodeParser * child=new XMLNodeParser(document);
                        child->Text=ValueBuffer;
                        child->type=Type_Text;
                        ValueBuffer="";
                        this->Childs.push_back(child);
                    }
                    if (Ch=='/') 
                    {                   
                        NameBuffer="";
                        state=State_NodeClosure;
                        break;
                    }
                    else //read child
                    {
                        //open node
                        XMLNodeParser * child=new XMLNodeParser(document);
                        child->ParseNode();
                        if (!document->hasError)
                        {
                            this->Childs.push_back(child);
                            document->SkipWhiteSpaces();
                            continue;
                        }
                        else
                        {
                            delete child;
                            break;
                        }                            
                    }
                } 
                else if (last=='&')
                {
                    //read &amp; or smth
                    returnstate=State_InsideNode;
                    state=State_ReadEntity;
                    EntityBuffer=last;
                    continue;
                }
                else
                {
                    ValueBuffer+=last;
                    continue;
                }
            }
            break; /* case State_InsideNode */
        case State_NodeClosure:
            {
                Ch=document->NextChar();
                if (isAlphaNum(Ch))
                {
                    NameBuffer+=Ch;
                }
                else 
                {
                    if (!stricmp(NameBuffer.c_str(),Text.c_str()))
                    {
                        //ok
                        state=State_NodeDone;
                        document->NextChar();
                        break;
                    }
                    else
                    {
                        document->hasError=true;
                        document->Error="Error:";
                        document->Error+="</";
                        document->Error+=NameBuffer;
                        document->Error+="> is not valid close tag for <";
                        document->Error+=Text;
                        document->Error+="> open tag";
                        break;
                    }
                }
            }
            break; /* State_NodeClosure */
        }
    }
    return this;
}

