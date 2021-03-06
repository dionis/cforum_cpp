/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CGI interface definition
 * \package cgi
 *
 * This defines the CGI parser interface
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

#ifndef CGIPARSEREXCEPTION_H
#define CGIPARSEREXCEPTION_H

#include "cgi/cgi_exception.hh"

namespace CForum {
  class CGIParserException : public CGIException {
  public:
    CGIParserException();
    CGIParserException(int);
    CGIParserException(const char *,int);
    CGIParserException(const std::string &,int);

    static const int InvalidCGIEnvironment = 0x4da96289;
    static const int NoCookiesGiven        = 0x4da962d0;
    static const int NoDecodeValueGiven    = 0x4da96346;
    static const int TooManyValues         = 0x4efc39f5;

  };
}

#endif

/* eof */
