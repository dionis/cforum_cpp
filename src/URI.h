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

#ifndef URI_H
#define URI_H

#include <cstring>
#include <string>

#include "exceptions/URIException.h"

namespace CForum {
  class URI {
  public:
    URI();
    URI(const std::string &);
    URI(const char *);
    URI(const URI &);

    const std::string &getScheme();
    const std::string &getHost();
    int getPort();
    const std::string &getPath();
    const std::string &getMethod();
    const std::string &getQueryString();
    const std::string &getFragment();
    const std::string &getMedia();

    URI &operator=(const URI &);

  private:
    std::string scheme, host, path, method, media, queryString, fragment;
    int port;

    void parseUri(const char *);
    void parsePath();
  };


  inline const std::string &URI::getScheme() {
    return scheme;
  }

  inline const std::string &URI::getHost() {
    return host;
  }

  inline int URI::getPort() {
    return port;
  }

  inline const std::string &URI::getPath() {
    return path;
  }

  inline const std::string &URI::getMethod() {
    return method;
  }

  inline const std::string &URI::getQueryString() {
    return queryString;
  }

  inline const std::string &URI::getFragment() {
    return fragment;
  }

  inline const std::string &URI::getMedia() {
    return media;
  }
}

#endif

/* eof */
