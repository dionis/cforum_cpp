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

#include "json/json_parser.hh"

namespace CForum {
  namespace JSON {
    Parser::Parser() { }

    void Parser::parseFile(const std::string &filename, boost::shared_ptr<Element> &root) {
      std::ifstream fd(filename.c_str(), std::ifstream::in);
      std::stringstream sst;

      if(!fd) {
        throw JSONException("File not found!",CForumErrorException::FileNotFound);
      }

      sst << fd.rdbuf();

      fd.close();

      parse(sst.str(), root);
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
            throw JSONSyntaxErrorException("End of comment could not be found!",JSONSyntaxErrorException::CommentNotEnded);
          }
          break;

        default:
          return ptr;
        }
      }

      return ptr;
    }

    UChar Parser::parseUChar(const char *ptr, const char *end, const char **cont) {
      char buff[10];
      size_t i = 0;

      while(ptr < end && isxdigit(*ptr) && i < 4) {
        buff[i++] = *ptr++;
      }

      buff[i] = 0;
      *cont = ptr - 1;

      return strtol(buff, NULL, 16);
    }

    inline bool Parser::isNext(const char *ptr, const char *end, const char c) {
      return ptr + 1 < end && *(ptr+1) == c;
    }

    const char *Parser::readDoubleQuoteString(const char *str,const char *end,Parser::Token &tok) {
      const char *ptr;
      tok.data = UnicodeString();

      for(ptr=str;ptr < end;++ptr) {
        switch(*ptr) {
          case '\\':
            if(isNext(ptr, end, '\\')) { /* backslash itself */
              ++ptr;
              tok.data += "\\";
            }
            else if(isNext(ptr, end, '\"')) { /* escaped double quotes */
              ++ptr;
              tok.data += "'";
            }
            else if(isNext(ptr, end, 'n')) { /* newline */
              ++ptr;
              tok.data += (UChar)0xA;
            }
            else if(isNext(ptr, end, 'r')) { /* carrige return */
              ++ptr;
              tok.data += (UChar)0xD;
            }
            else if(isNext(ptr, end, 't')) { /* tabulator */
              ++ptr;
              tok.data += (UChar)'\t';
            }
            else if(isNext(ptr, end, 'f')) { /* form feed */
              ++ptr;
              tok.data += (UChar)'\f';
            }
            else if(isNext(ptr, end, 'b')) { /* backspace */
              ++ptr;
              tok.data += (UChar)'\b';
            }
            else if(isNext(ptr, end, 'u')) { /* unicode escape sequence */
              tok.data += parseUChar(ptr + 2, end, &ptr);
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

      throw JSONSyntaxErrorException("String not terminated",JSONSyntaxErrorException::StringNotTerminated);
      return NULL;
    }

    const char *Parser::getNextToken(const char *str,const char *end,Parser::Token &tok) {
      const char *ptr;

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
        for(ptr=str;ptr<end && isdigit(*ptr);++ptr) {}

        if(*ptr == '.') {
          if(!isdigit(*(ptr+1))) {
            throw JSONSyntaxErrorException("Syntax error in float: a digit must follow the dot",JSONSyntaxErrorException::FloatNumberError);
          }

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

    const char *Parser::readArray(boost::shared_ptr<Array> elem,const char *str,const char *end) {
      Token tok;
      boost::shared_ptr<Element> data;

      Array::ArrayType_t &vec = elem->getValue();

      do {
        getNextToken(str,end,tok);

        if(tok.type != JSONTokenTypeArrayEnd) {
          str = readValue(&data,str,end);
          vec.push_back(data);
          str = getNextToken(str,end,tok);

          if(tok.type != JSONTokenTypeComma && tok.type != JSONTokenTypeArrayEnd) {
            throw JSONSyntaxErrorException("Syntax error in array",JSONSyntaxErrorException::ArraySyntaxError);
          }
        }
        else {
          ++str;
        }
      } while(tok.type != JSONTokenTypeArrayEnd);

      return str;
    }

    const char *Parser::readObject(boost::shared_ptr<Object> elem,const char *str,const char *end) {
      Token tok;
      UnicodeString key;
      boost::shared_ptr<Element> data;

      Object::ObjectType_t &mp = elem->getValue();

      do {
        str = getNextToken(str,end,tok);

        if(tok.type != JSONTokenTypeObjectEnd) {
          if(tok.type != JSONTokenTypeString) {
            throw JSONSyntaxErrorException("Object key has to be a string",JSONSyntaxErrorException::ObjectKeyMustBeString);
          }

          key = tok.data;
          str = getNextToken(str,end,tok);

          if(tok.type != JSONTokenTypeColon) {
            throw JSONSyntaxErrorException("A colon must follow a key",JSONSyntaxErrorException::ObjectColonMustFollowKey);
          }

          str = readValue(&data,str,end);
          mp[key] = data;

          str = getNextToken(str,end,tok);
          if(tok.type != JSONTokenTypeComma && tok.type != JSONTokenTypeObjectEnd) {
            throw JSONSyntaxErrorException("Comma or object end must follow",JSONSyntaxErrorException::ObjectCommaOrEOOMissing);
          }
        }
      } while(tok.type != JSONTokenTypeObjectEnd);

      return str;
    }

    const char *Parser::readValue(boost::shared_ptr<Element> *root, const char *ptr, const char *end) {
      Parser::Token tok;

      tok.type = JSONTokenTypeUnknown;
      tok.dval = 0.0;
      tok.ival = 0;
      tok.data = UnicodeString();

      ptr = getNextToken(ptr,end,tok);

      switch(tok.type) {
      case JSONTokenTypeObjectBegin:
        *root = boost::make_shared<Object>();
        ptr = readObject(boost::dynamic_pointer_cast<Object>(*root),ptr,end);
        return ptr;

      case JSONTokenTypeArrayBegin:
        *root = boost::make_shared<Array>();
        ptr = readArray(boost::dynamic_pointer_cast<Array>(*root),ptr,end);
        return ptr;

        /* we should have a better handling of array and object */
      case JSONTokenTypeArrayEnd:
      case JSONTokenTypeObjectEnd:
        return ptr;

      case JSONTokenTypeString:
        *root = boost::make_shared<String>(tok.data);
        return ptr;

      case JSONTokenTypeNumberInt:
        *root = boost::make_shared<Number>(tok.ival);
        return ptr;

      case JSONTokenTypeNumberFloat:
        *root = boost::make_shared<Number>(tok.dval);
        return ptr;

      case JSONTokenTypeFalse:
        *root = boost::make_shared<Boolean>(false);
        return ptr;

      case JSONTokenTypeTrue:
        *root = boost::make_shared<Boolean>(true);
        return ptr;

      case JSONTokenTypeNull:
        *root = boost::make_shared<Null>();
        return ptr;

      default:
        throw JSONSyntaxErrorException("unknown token type",JSONSyntaxErrorException::UnknownTokenType);
      }

    }


  }
}

/* eof */
