/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CouchDB error exception
 * \package couchdb
 *
 * This implements the CouchDB error exception interface
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


#include "couch_error_exception.h"

namespace CForum {
  namespace CouchDB {
    CouchErrorException::CouchErrorException() : CForumException(), _error_msg(), _error_code(0) {}
    CouchErrorException::CouchErrorException(int code) : CForumException(code), _error_msg(), _error_code(0) {}
    CouchErrorException::CouchErrorException(const char *msg,int code) : CForumException(msg,code), _error_msg(), _error_code(0) {}
    CouchErrorException::CouchErrorException(const std::string &msg,int code) : CForumException(msg,code), _error_msg(), _error_code(0) {}

    CouchErrorException::CouchErrorException(const char *msg, int code, const char *errmsg, int errcode) : CForumException(msg,code), _error_msg(errmsg), _error_code(errcode) {}

    int CouchErrorException::getErrorCode() {
      return _error_code;
    }

    const std::string &CouchErrorException::getErrorMessage() {
      return _error_msg;
    }

    CouchErrorException::~CouchErrorException() throw() {}
  }
}

/* eof */
