/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSON syntax error exception
 * \package JSON
 *
 * This defines the JSON syntax error exception interface
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

#ifndef JSON_SYNTAX_EXCEPTION_H
#define JSON_SYNTAX_EXCEPTION_H

#include "config.h"

#include "json_exception.h"

namespace CForum {
  namespace JSON {

    class JSONSyntaxErrorException : public JSONException {
    public:
      JSONSyntaxErrorException();
      JSONSyntaxErrorException(int);
      JSONSyntaxErrorException(const char *,int);
      JSONSyntaxErrorException(const std::string &,int);

      static const int NoParseEnd               = 0x4d8dfcb5;
      static const int CommentNotEnded          = 0x4d8dfcbc;
      static const int StringNotTerminated      = 0x4d8dfcc2;
      static const int FloatNumberError         = 0x4d986b4c;
      static const int ArraySyntaxError         = 0x4d986b70;
      static const int ObjectKeyMustBeString    = 0x4d8dfcc6;
      static const int ObjectColonMustFollowKey = 0x4d8dfccc;
      static const int ObjectCommaOrEOOMissing  = 0x4d8dfcd2;
      static const int UnknownTokenType         = 0x4d8e06a1;
    };

  }
}

#endif

/* eof */
