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

#include "Server.h"

namespace CForum {
  namespace CouchDB {
    Server::Server() : _host(), _db(), _protocol("http"), _user(), _pass(), _authed(false), _port(5984), _curl(NULL), _connect(1) {}
    Server::Server(const std::string &db) : _host("localhost"), _db(db), _protocol("http"), _user(), _pass(), _authed(false), _port(5984), _curl(NULL), _connect(1) {}
    Server::Server(const std::string &db,const std::string &host) : _host(host), _db(db), _protocol("http"), _user(), _pass(), _authed(false), _port(5984), _curl(NULL), _connect(1) {}
    Server::Server(const std::string &db,const std::string &host,const std::string &protocol) : _host(host), _db(db), _protocol(protocol), _user(), _pass(), _authed(false), _port(5984), _curl(NULL), _connect(1) {}
    Server::Server(const std::string &db,const std::string &host,const std::string &protocol,int port) : _host(host), _db(db), _protocol(protocol), _user(), _pass(), _authed(false), _port(port), _curl(NULL), _connect(1) {}


    void Server::setAuth(const std::string &user,const std::string &pass) {
      _user = user;
      _pass = pass;
      _authed = true;
    }

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
        curl_easy_setopt(_curl,CURLOPT_PROTOCOLS,CURLPROTO_HTTP|CURLPROTO_HTTPS);
        curl_easy_setopt(_curl,CURLOPT_REDIR_PROTOCOLS,CURLPROTO_HTTP|CURLPROTO_HTTPS);
        curl_easy_setopt(_curl,CURLOPT_FOLLOWLOCATION,1);

        curl_easy_setopt(_curl,CURLOPT_PORT,_port);
        curl_easy_setopt(_curl,CURLOPT_USERAGENT,"CForum/" CF_VERSION);

        if(_authed) {
          curl_easy_setopt(_curl,CURLOPT_HTTPAUTH,CURLAUTH_BASIC);
          curl_easy_setopt(_curl,CURLOPT_USERNAME,_user.c_str());
          curl_easy_setopt(_curl,CURLOPT_PASSWORD,_pass.c_str());
        }

      }
    }

    std::string Server::genURI(const std::string &key) const {
      std::string url = _protocol;

      url += "://";
      url += _host;
      url += ":";
      url += _port;

      if(key[0] != '/') {
        url += "/";
      }

      url += key;

      return url;
    }

    void Server::putDocument(const Document &doc) {
    }

    Document Server::getDocument(const std::string &key) {
      std::string url = genURI(key);
      CURLcode cd;

      connect();

      curl_easy_setopt(_curl,CURLOPT_HTTPGET,1);
      curl_easy_setopt(_curl,CURLOPT_URL,url.c_str());

      if((cd = curl_easy_perform(_curl)) != CURLE_OK) {
        throw CouchErrorException(curl_easy_strerror(cd),cd); // TODO: proper exception
      }

      return Document();
    }

    Document Server::getDocument(const UnicodeString &key) {
      std::string str;
      key.toUTF8String(str);
      return getDocument(str);
    }

    void Server::deleteDocument(const Document &doc) {
    }

  }
}


/* eof */
