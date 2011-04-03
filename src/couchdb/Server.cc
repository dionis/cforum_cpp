/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CouchDB interface implementation
 * \package couchdb
 *
 * This implements the CouchDB interface
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

#include "CouchDBInterface.h"

namespace CForum {
  namespace CouchDB {
    Server::Server() : _host(), _db(), _port(0), _curl(NULL), _connect(1) {}
    Server::Server(const std::string &db) : _host("localhost"), _db(db), _port(5984), _curl(NULL), _connect(1) {}
    Server::Server(const std::string &db,const std::string &host) : _host(host), _db(db), _port(5984), _curl(NULL), _connect(1) {}
    Server::Server(const std::string &db,const std::string &host,const int port) : _host(host), _db(db), _port(port), _curl(NULL), _connect(1) {}

    std::string Server::getHost() {
      return _host;
    }
    std::string Server::setHost(const std::string &host) {
      std::string retval = _host;
      _host = host;
      _connect = 1;
      return retval;
    }

    std::string Server::getDatabase() {
      return _db;
    }
    std::string Server::setDatabase(const std::string &db) {
      std::string retval = _db;
      _db = db;
      return retval;
    }

    int Server::getPort() {
      return _port;
    }
    int Server::setPort(const int port) {
      int retval = _port;
      _port = port;
      _connect = 1;
      return retval;
    }

    void Server::connect() {
      if(_connect) {
        if(_curl) {
          curl_easy_cleanup(_curl);
        }

        _curl = curl_easy_init();
        curl_easy_setopt(_curl,CURLOPT_NOPROGRESS,1);
        curl_easy_setopt(_curl,CURLOPT_FORBID_REUSE,0);
      }
    }

    void Server::putDocument(Document &doc) {
    }

    std::string Server::getDocument(std::string &key) {
      return std::string("");
    }

    void Server::deleteDocument(Document &doc) {
    }

  }
}

int main(void) {
  CForum::CouchDB::Server srv("test");
}

/* eof */
