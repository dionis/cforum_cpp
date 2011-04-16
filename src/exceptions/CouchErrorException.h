/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CouchDB error exception
 * \package couchdb
 *
 * This defines the CouchDB error exception interface
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

#ifndef COUCH_ERROR_EXCEPTION_H
#define COUCH_ERROR_EXCEPTION_H

#include "config.h"

#include "CForumException.h"

namespace CForum {
  namespace CouchDB {

    class CouchErrorException : public CForumException {
    public:
      CouchErrorException();
      CouchErrorException(int);
      CouchErrorException(const char *, int);
      CouchErrorException(const std::string &, int);

      CouchErrorException(const char *, int, const char *, int);

      int getErrorCode();
      const std::string &getErrorMessage();

      virtual ~CouchErrorException() throw();

    protected:
      std::string _error_msg;
      int _error_code;
    };
  }
}

#endif

/* eof */
