/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Basic exception interface
 * \package Exceptions
 *
 * This defines the basic CForum exception interface
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

#ifndef CFORUM_EXCEPTION_H
#define CFORUM_EXCEPTION_H

#include <exception>
#include <string>

namespace CForum {
  class CForumException : public std::exception {
  public:
    CForumException();
    CForumException(int);
    CForumException(const char *,int);
    CForumException(const std::string &,int);

    virtual int getCode();
    virtual const std::string &getMessage();

    virtual ~CForumException() throw();

  protected:
    std::string _msg;
    int _code;
  };
}

#endif

/* eof */
