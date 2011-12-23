/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief response object, saving headers and so
 * \package couchdb
 *
 * This is the response class, saving the response from a
 * CURL request
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

#include "server.h"

namespace CForum {
  namespace CouchDB {
    Server::Response::Response() : _headers(), _body(), _message(), _status(0), _version(0) {}

    void Server::Response::addContent(const std::string &cnt) {
      _body += cnt;
    }

    std::string &Server::Response::getContent() {
      return _body;
    }
    const std::string &Server::Response::getContent() const {
      return _body;
    }

    void Server::Response::addHeader(const std::string &name,const std::string &value) {
      _headers[name] = value;
    }

    std::string &Server::Response::getHeader(const std::string &name) {
      return _headers[name];
    }
    const std::string &Server::Response::getHeader(const std::string &name) const {
      static std::string emptyString;
      std::map<std::string,std::string>::const_iterator it = _headers.find(name);

      if(it == _headers.end()) {
        return emptyString;
      }

      return it->second;
    }

    void Server::Response::setMessage(const std::string &msg) {
      _message = msg;
    }

    const std::string &Server::Response::getMessage() const {
      return _message;
    }
    std::string &Server::Response::getMessage() {
      return _message;
    }

    void Server::Response::setVersion(float v) {
      _version = v;
    }
    float Server::Response::getVersion() {
      return _version;
    }

    void Server::Response::setStatus(int status) {
      _status = status;
    }
    int Server::Response::getStatus() {
      return _status;
    }

  }
}

/* eof */
