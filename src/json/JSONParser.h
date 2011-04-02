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

#include <cstdlib>
#include <cstring>

#include "JSONElement.h"
#include "JSONObject.h"
#include "JSONArray.h"
#include "JSONBoolean.h"
#include "JSONString.h"
#include "JSONNumber.h"
#include "JSONNull.h"

#include "exceptions/JSONSyntaxException.h"

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

      void parseFile(const std::string &, Element **);

      void parse(const UnicodeString &, Element **);
      void parse(const std::string &, Element **);
      void parse(const char *, Element **);
      void parse(const char *, size_t,Element **);

    private:
      class Token;

      const char *readValue(Element **, const char *, const char *);

      const char *eatWhitespacesAndComments(const char *,const char *);
      const char *getNextToken(const char *,const char *,Token &);

      const char *readSingleQuoteString(const char *,const char *,Token &);
      const char *readDoubleQuoteString(const char *,const char *,Token &);

      const char *readObject(Object *,const char *,const char *);
      const char *readArray(Array *,const char *,const char *);

      class Token {
      public:
        enum JSONTokenType type;
        UnicodeString data;
        double dval;
        int64_t ival;
      };

    };
  }
}

#endif

/* eof */