/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSON parser interface
 * \package JSON
 *
 * This defines the JSON parser interface
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

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "config.h"

#include <unicode/unistr.h>
#include <unicode/bytestream.h>

#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>

#include <cstdlib>
#include <cstring>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "json/json_element.h"
#include "json/json_object.h"
#include "json/json_array.h"
#include "json/json_boolean.h"
#include "json/json_string.h"
#include "json/json_number.h"
#include "json/json_null.h"

#include "json/json_syntax_exception.h"

namespace CForum {
  namespace JSON {
    enum JSONTokenType {
      JSONTokenTypeUnknown,

      JSONTokenTypeObjectBegin,
      JSONTokenTypeObjectEnd,

      JSONTokenTypeArrayBegin,
      JSONTokenTypeArrayEnd,

      JSONTokenTypeString,

      JSONTokenTypeNull,

      JSONTokenTypeNumberInt,
      JSONTokenTypeNumberFloat,

      JSONTokenTypeTrue,
      JSONTokenTypeFalse,

      JSONTokenTypeComma,
      JSONTokenTypeColon
    };

    class Parser {
    public:
      Parser();

      void parseFile(const std::string &, boost::shared_ptr<Element> &);

      void parse(const UnicodeString &, boost::shared_ptr<Element> &);
      void parse(const std::string &, boost::shared_ptr<Element> &);
      void parse(const char *, boost::shared_ptr<Element> &);
      void parse(const char *, size_t,boost::shared_ptr<Element> &);

      static void copyObject(const Object &,Object &);
      static void copyArray(const Array &, Array &);

    private:
      class Token;

      const char *readValue(boost::shared_ptr<Element> *, const char *, const char *);

      const char *eatWhitespacesAndComments(const char *,const char *);
      const char *getNextToken(const char *,const char *,Token &);

      const char *readSingleQuoteString(const char *,const char *,Token &);
      const char *readDoubleQuoteString(const char *,const char *,Token &);

      const char *readObject(boost::shared_ptr<Object>,const char *,const char *);
      const char *readArray(boost::shared_ptr<Array>,const char *,const char *);

      UChar parseUChar(const char *, const char *, const char **);
      bool isNext(const char *, const char *, const char);

      class Token {
      public:
        enum JSONTokenType type;
        UnicodeString data;
        double dval;
        int64_t ival;
      };

    };

    inline void Parser::parse(const UnicodeString &json_str, boost::shared_ptr<Element> &root) {
      std::string tmp;

      json_str.toUTF8String(tmp);
      parse(tmp,root);
    }

    inline void Parser::parse(const std::string &json_str, boost::shared_ptr<Element> &root) {
      parse(json_str.c_str(),json_str.length(),root);
    }

    inline void Parser::parse(const char *json_str, boost::shared_ptr<Element> &root) {
      parse(json_str,strlen(json_str),root);
    }

    inline void Parser::parse(const char *json_str, size_t len, boost::shared_ptr<Element> &root) {
      const char *end = readValue(&root,json_str,json_str + len-1);

      if(end != json_str + len) {
        end = eatWhitespacesAndComments(end,json_str+len);

        if(end != json_str + len) {
          throw JSONSyntaxErrorException("Error in syntax: not at end of JSON code after parsing",JSONSyntaxErrorException::NoParseEnd);
        }
      }
    }

  }
}

#endif

/* eof */
