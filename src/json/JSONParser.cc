/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSON parser interface
 * \package JSON
 *
 * This implements the JSON parser interface
 */

/*
 * Copyright (C) 2011 by Christian Kruse <cjk@wwwtech.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "JSONParser.h"


namespace CForum {
  namespace JSON {
    Parser::Parser() {
    }

    void Parser::parseFile(const std::string &filename, Element **root) {
      std::ifstream fd(filename.c_str(), std::ifstream::in);
      std::string str;

      if(!fd) {
        throw JSONException("File not found!",ErrorCodeFileNotFound);
      }

      str.reserve(1024);

      std::istream_iterator<std::string> begin(fd);
      std::istream_iterator<std::string> end;

      while(begin != end) {
        str += *begin;
        ++begin;
      }

      fd.close();

      parse(str,root);
    }

    void Parser::parse(const UnicodeString &json_str, Element **root) {
      std::string tmp;

      json_str.toUTF8String(tmp);
      parse(tmp,root);
    }

    void Parser::parse(const std::string &json_str, Element **root) {
      parse(json_str.c_str(),json_str.length(),root);
    }

    void Parser::parse(const char *json_str, Element **root) {
      parse(json_str,strlen(json_str),root);
    }

    void Parser::parse(const char *json_str, size_t len,Element **root) {
      const char *end = readValue(root,json_str,json_str + len-1);

      if(end != json_str + len) {
        end = eatWhitespacesAndComments(end,json_str+len-1);

        if(end != json_str + len) {
          throw JSONSyntaxErrorException("Error in syntax: not at end of JSON code after parsing",ErrorCodeJSONNoParseEnd);
        }
      }
    }

    const char *Parser::eatWhitespacesAndComments(const char *str,const char *end) {
      const char *ptr;
      bool found;

      for(ptr=str;ptr < end;++ptr) {
        switch(*ptr) {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
          continue;

        case '/':
          if(*(ptr+1) == '*') {
            found = false;

            for(;ptr < end;++ptr) {
              if(*ptr == '*' && ptr < end && *(ptr+1) == '/') {
                found = true;
                ++ptr;
                break;
              }
            }
          }
          else if(*(ptr+1) == '/') {
            for(;ptr < end;++ptr) {
              if(*ptr == '\n') {
                found = true;
                break;
              }
            }
          }

          if(!found) {
            throw JSONSyntaxErrorException("End of comment could not be found!",ErrorCodeJSONCommentNotEnded);
          }
          break;

        default:
          return ptr;
        }
      }

      return ptr;
    }

    const char *Parser::readDoubleQuoteString(const char *str,const char *end,Parser::Token &tok) {
      const char *ptr;
      tok.data = UnicodeString();

      for(ptr=str;ptr < end;++ptr) {
        switch(*ptr) {
        case '\\':
          if(*(ptr+1) == '\\') { /* backslash itself */
            ++ptr;
            tok.data += "\\";
          }
          else if(*(ptr+1) == '\"') { /* escaped double quotes */
            ++ptr;
            tok.data += "'";
          }
          else if(*(ptr+1) == 'n') { /* newline */
            ++ptr;
            tok.data += (UChar)0xA;
          }
          else if(*(ptr+1) == 'r') { /* carrige return */
            ++ptr;
            tok.data += (UChar)0xD;
          }
          else if(*(ptr+1) == 't') { /* tabulator */
            ++ptr;
            tok.data += (UChar)'\t';
          }
          else if(*(ptr+1) == 'f') { /* form feed */
            ++ptr;
            tok.data += (UChar)'\f';
          }
          else if(*(ptr+1) == 'b') { /* backspace */
            ++ptr;
            tok.data += (UChar)'\b';
          }
          else if(*(ptr+1) == 'u') { /* unicode escape sequence */
            ptr += 2;
            tok.data += (UChar)strtoll(ptr,(char **)&ptr,16);
            --ptr;
          }
          else { /* everything else */
            tok.data += *ptr;
          }

          break;

        case '"':
          return ptr + 1;

        default:
          tok.data += *ptr;
        }
      }

      throw JSONSyntaxErrorException("String not terminated",ErrorCodeJSONStringNotTerminated);
      return NULL;
    }

    const char *Parser::getNextToken(const char *str,const char *end,Parser::Token &tok) {
      str = eatWhitespacesAndComments(str,end);

      switch(*str) {
      case '{':
        tok.type = JSONTokenTypeObjectBegin;
        return str + 1;

      case '}':
        tok.type = JSONTokenTypeObjectEnd;
        return str + 1;

      case '[':
        tok.type = JSONTokenTypeArrayBegin;
        return str + 1;

      case ']':
        tok.type = JSONTokenTypeArrayEnd;
        return str + 1;

      case '"':
        tok.type = JSONTokenTypeString;
        str = readDoubleQuoteString(str+1,end,tok);
        return str;

      case ':':
        tok.type = JSONTokenTypeColon;
        return str + 1;

      case ',':
        tok.type = JSONTokenTypeComma;
        return str + 1;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if(*(str+1) == '.') {
          tok.type = JSONTokenTypeNumberFloat;
          tok.dval = strtod(str,(char **)&str);
        }
        else {
          tok.type = JSONTokenTypeNumberInt;
          tok.ival = strtoll(str,(char **)&str,0);
        }

        return str;

      default:
        if(strncmp(str,"true",4) == 0) {
          tok.type = JSONTokenTypeTrue;
          return str + 4;
        }
        else if(strncmp(str,"false",5) == 0) {
          tok.type = JSONTokenTypeFalse;
          return str + 5;
        }
        else if(strncmp(str,"null",4) == 0) {
          tok.type = JSONTokenTypeNull;
          return str + 4;
        }
      }

      return str;
    }

    const char *Parser::readArray(Array *elem,const char *str,const char *end) {
      Token tok;
      Element *data;

      std::vector<Element *> &vec = elem->getValue();

      do {
        getNextToken(str,end,tok);

        if(tok.type != JSONTokenTypeArrayEnd) {
          str = readValue(&data,str,end);
          vec.push_back(data);
          str = getNextToken(str,end,tok);

          if(tok.type != JSONTokenTypeComma && tok.type != JSONTokenTypeArrayEnd) {
            throw JSONSyntaxErrorException("Syntax error in array",0); // TODO: proper exception
          }
        }
        else {
          ++str;
        }
      } while(tok.type != JSONTokenTypeArrayEnd);

      return str;
    }

    const char *Parser::readObject(Object *elem,const char *str,const char *end) {
      Token tok;
      UnicodeString key;
      Element *data;

      std::map<UnicodeString,Element *> &mp = elem->getValue();

      do {
        str = getNextToken(str,end,tok);

        if(tok.type != JSONTokenTypeObjectEnd) {
          if(tok.type != JSONTokenTypeString) {
            throw JSONSyntaxErrorException("Object key has to be a string",ErrorCodeJSONObjectKeyMustBeString);
          }

          key = tok.data;
          str = getNextToken(str,end,tok);

          if(tok.type != JSONTokenTypeColon) {
            throw JSONSyntaxErrorException("A colon must follow a key",ErrorCodeJSONObjectColonMustFollowKey);
          }

          str = readValue(&data,str,end);
          mp[key] = data;

          str = getNextToken(str,end,tok);
          if(tok.type != JSONTokenTypeComma && tok.type != JSONTokenTypeObjectEnd) {
            throw JSONSyntaxErrorException("Comma or object end must follow",ErrorCodeJSONObjectCommaOrEOOMissing);
          }
        }
      } while(tok.type != JSONTokenTypeObjectEnd);

      return str;
    }

    const char *Parser::readValue(Element **root, const char *ptr, const char *end) {
      Parser::Token tok;

      Object *obj;
      Array *ary;

      tok.type = JSONTokenTypeUnknown;
      tok.dval = 0.0;
      tok.ival = 0;
      tok.data = UnicodeString();

      ptr = getNextToken(ptr,end,tok);

      switch(tok.type) {
      case JSONTokenTypeObjectBegin:
        obj = new Object();
        *root = obj;
        ptr = readObject(obj,ptr,end);
        return ptr;

      case JSONTokenTypeArrayBegin:
        ary = new Array();
        *root = ary;
        ptr = readArray(ary,ptr,end);
        return ptr;

        /* we should have a better handling of array and object */
      case JSONTokenTypeArrayEnd:
      case JSONTokenTypeObjectEnd:
        return ptr;

      case JSONTokenTypeString:
        *root = new String(tok.data);
        return ptr;

      case JSONTokenTypeNumberInt:
        *root = new Number(tok.ival);
        return ptr;

      case JSONTokenTypeNumberFloat:
        *root = new Number(tok.dval);
        return ptr;

      case JSONTokenTypeFalse:
        *root = new Boolean(false);
        return ptr;

      case JSONTokenTypeTrue:
        *root = new Boolean(true);
        return ptr;

      case JSONTokenTypeNull:
        *root = new Null();
        return ptr;

      default:
        throw JSONSyntaxErrorException("unknown token type",ErrorCodeJSONUnknownTokenType);
      }

    }


  }
}

/* eof */
