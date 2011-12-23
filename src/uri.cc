/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief HTTP URI parser, parses HTTP URIs to its parts
 * \package framework
 *
 * HTTP URI parser, parses HTTP URIs to its parts
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

#include "uri.h"

namespace CForum {
  URI::URI() {}

  URI::URI(const std::string &uri) {
    parseUri(uri.c_str());
  }

  URI::URI(const char *uri) {
    parseUri(uri);
  }

  URI::URI(const URI &uri) : scheme(uri.scheme), host(uri.host), path(uri.path),
                             pathWoSuffix(uri.pathWoSuffix), method(uri.method),
                             media(uri.media), queryString(uri.queryString),
                             fragment(uri.fragment), port(uri.port) {
  }

  URI &URI::operator=(const URI &uri) {
    if(this != &uri) {
      scheme       = uri.scheme;
      host         = uri.host;
      path         = uri.path;
      pathWoSuffix = uri.pathWoSuffix;
      method       = uri.method;
      media        = uri.media;
      queryString  = uri.queryString;
      fragment     = uri.fragment;
      port         = uri.port;
    }

    return *this;
  }

  void URI::parseUri(const char *uri) {
    const char *ptr, *start;

    /* set default values */
    media       = "html";
    port        = 80;
    queryString = "";
    fragment    = "";

    for(start = uri, ptr = uri; *ptr; ++ptr) {
      if(strncmp(ptr, "://", 3) == 0) {
        scheme = std::string(start, ptr - start);
        start  = ptr + 3;
        ptr   += 3;
        break;
      }
    }

    if(*ptr == '\0') {
      throw URIException("Error in URI syntax: could not find scheme", URIException::NoSchemeFound);
    }

    for(; *ptr && *ptr != '/' && *ptr != ':'; ++ptr) ;

    if(ptr == start) {
      throw URIException("Error in URI syntax: could not find host namme", URIException::NoHostnameFound);
    }

    host = std::string(start, ptr - start);

    /* we got a string in the form of http://hostname */
    if(*ptr == '\0') {
      path  = "/";
      pathWoSuffix = "/";
      return;
    }

    /* did we get a port? */
    if(*ptr == ':') {
      if(isdigit(*(ptr + 1))) {
        port = strtol(ptr + 1, (char **)&ptr, 10);
      }
      else {
        throw URIException("Error in URI: expecting port number after colon after hostname!", URIException::InvalidPortNumber);
      }
    }
    else {
      port = 80;
    }

    for(start = ptr; *ptr && *ptr != '#' && *ptr != '?'; ++ptr) ;

    if(start == ptr) {
      path = "/";
    }
    else {
      path = std::string(start, ptr - start);
    }

    parsePath();

    if(*ptr == '\0') {
      return;
    }

    if(*ptr == '?') {
      for(start = ptr; *ptr && *ptr != '#'; ++ptr) ;
      queryString = std::string(start + 1, ptr - start - 1);
    }

    if(*ptr == '#') {
      fragment = std::string(ptr);
    }
  }

  void URI::parsePath() {
    size_t len = path.length();
    const char *first = path.c_str(), *last = first + len - 1;

    for(; last >= first && *last != '.' && isalnum(*last) && islower(*last); --last) ;

    if(*last == '.') {
      media        = std::string(last + 1);
      pathWoSuffix = std::string(first, last - first);
    }
    else {
      pathWoSuffix = first;
    }
  }

  std::string URI::toString() {
    return ((const URI *)this)->toString();
  }

  std::string URI::toString() const {
    std::ostringstream ostr;

    ostr << scheme << "://" << host;

    if((scheme == "http" && port != 80) || (scheme == "https" && port != 443)) {
      ostr << ":" << port;
    }

    ostr << path;

    if(queryString.length()) {
      ostr << "?" << queryString;
    }

    if(fragment.length()) {
      ostr << fragment;
    }

    return ostr.str();
  }

  URI::~URI() { }

}

/* eof */
