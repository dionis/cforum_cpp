/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Request information, including CGI parameters
 * \package framework
 *
 * Request information, including route, CGI parameters, etc, pp
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

#ifndef REQUEST_H
#define REQUEST_H

#include <string>

#include "cgi/cgi.hh"
#include "framework/uri.hh"
#include "framework/user.hh"

namespace CForum {
  class Request {
  public:
    Request();
    Request(const Request &);

    virtual Request &operator=(const Request &);

    virtual const URI &getUri() const;
    virtual const CGI &getCGI() const = 0;

    virtual void setUser(const User &);
    virtual const User &getUser() const;


    virtual ~Request() = 0;

  protected:
    URI requestUri;
    User user;

  };

  inline const URI &Request::getUri() const {
    return requestUri;
  }

  inline void Request::setUser(const User &usr) {
    user = usr;
  }
  inline const User &Request::getUser() const {
    return user;
  }

}

#endif

/* eof */
