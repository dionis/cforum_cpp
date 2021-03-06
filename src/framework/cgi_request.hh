/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CGI Request information, comes via CGI environment
 * \package framework
 *
 * CGI Request information, comes via CGI environment
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

#ifndef CGI_REQUEST_H
#define CGI_REQUEST_H

#include <iostream>
#include <string>
#include <sstream>

#include <algorithm>
#include <cctype>

#include "framework/request.hh"

namespace CForum {
  class CGIRequest : public Request {
  public:
    CGIRequest();
    CGIRequest(const CGIRequest &);
    virtual ~CGIRequest();

    virtual CGIRequest &operator=(const CGIRequest &);

    virtual const CGI &getCGI() const;

    virtual void output(const std::string &);

  protected:
    CGI cgi;

  };

  inline const CGI &CGIRequest::getCGI() const {
    return cgi;
  }
}

#endif

/* eof */
