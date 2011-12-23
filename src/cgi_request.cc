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

#include "cgi_request.h"

namespace CForum {
  CGIRequest::CGIRequest() : Request::Request() {
    std::string path_info = cgi.pathInfo(),
      hostname = cgi.serverName(),
      proto = cgi.serverProtocol(),
      uri;

    int port = cgi.serverPort();

    /* TODO: check if SERVER_PROTOCOL contains http/https or if we have to do more */
    uri = proto + "://" + hostname;
    if((proto == "https" && port != 443) || (proto == "http" && port != 80)) {
      uri += ":" + port;
    }

    uri += path_info.length() ? path_info : std::string("/");

    requestUri = URI(uri);
  }

  CGIRequest::~CGIRequest() { }

}

/* eof */

