/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Framework exception, all framework errors throw a child exception
 * \package framework
 *
 * Framework exception, all framework errors throw a child exception
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

#ifndef REDIRECT_EXCEPTION_H
#define REDIRECT_EXCEPTION_H

#include "framework/framework_exception.hh"

namespace CForum {
  class RedirectException : public FrameworkException {
  public:
    RedirectException(const std::string &, int);
    RedirectException(int, const std::string &, int);
    RedirectException(const char *, int, const std::string &, int);
    RedirectException(const std::string &, int, const std::string &, int);

    virtual ~RedirectException() throw();

    virtual int getStatus();
    virtual const std::string &getUrl();

  protected:
    int status;
    std::string url;
  };

  inline int RedirectException::getStatus() {
    return status;
  }

  inline const std::string &RedirectException::getUrl() {
    return url;
  }

}

#endif

/* eof */
