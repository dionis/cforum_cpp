/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CouchDB interface definition
 * \package CouchDB
 *
 * This defines the CouchDB interface
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

#ifndef COUCH_INTERFACE_H
#define COUCH_INTERFACE_H

#include "config.h"

#include <unicode/unistr.h>
#include <string>

#include <curl/curl.h>

#include "cgi/cgi.hh"
#include "couchdb/document.hh"

namespace CForum {
  namespace CouchDB {
    class Server {
    public:
      Server(const std::string &);
      Server(const std::string &, const std::string &);
      Server(const std::string &, const std::string &, const std::string &);
      Server(const std::string &, const std::string &, const std::string &,int);
      Server(const Server &);

      Server &operator=(const Server &);

      ~Server();

      std::string getHost();
      std::string setHost(const std::string &);

      std::string getDatabase();
      std::string setDatabase(const std::string &,bool = false);

      void createDatabase();
      void deleteDatabase();

      int getPort();
      int setPort(int);

      void putDocument(const Document &);

      Document getDocument(const std::string &);
      Document getDocument(const UnicodeString &);
      Document getDocument(const char *);

      void deleteDocument(const Document &);
      void deleteDocument(const std::string &);

      void setAuth(const std::string &,const std::string &);

      class Chunk {
      public:
        Chunk();
        Chunk(const std::string &);
        Chunk(const char *,size_t);
        Chunk(const Chunk &);

        Chunk &operator=(const Chunk &);

        void setPos(size_t);
        size_t getPos() const;
        size_t getPos();

        const std::string &getBuff() const;
        std::string &getBuff();
        void setBuff(const std::string &);

      private:
        size_t _pos;
        std::string _buff;
      };


      class Response {
      public:
        Response();
        Response(const Response &);

        Response &operator=(const Response &);

        void addContent(const std::string &);

        std::string &getContent();
        const std::string &getContent() const;

        void addHeader(const std::string &,const std::string &);

        std::string &getHeader(const std::string &);
        const std::string &getHeader(const std::string &) const;

        void setMessage(const std::string &);
        const std::string &getMessage() const;
        std::string &getMessage();

        void setVersion(float);
        float getVersion();

        void setStatus(int);
        int getStatus();

      private:
        std::map<std::string,std::string> _headers;
        std::string _body,_message;
        int _status;
        float _version;
      };

    private:
      Server();

      void connect();
      std::string genURI(const std::string &) const;

    protected:
      std::string _host,_db,_protocol,_user,_pass;
      bool _authed;
      int _port;
      CURL *_curl;
      int _connect;

    };

    inline void Server::setAuth(const std::string &user,const std::string &pass) {
      _user = user;
      _pass = pass;
      _authed = true;
    }

    inline std::string Server::getHost() {
      return _host;
    }
    inline std::string Server::setHost(const std::string &host) {
      std::string retval = _host;
      _host = host;
      _connect = 1;
      return retval;
    }

    inline std::string Server::getDatabase() {
      return _db;
    }

    inline std::string Server::setDatabase(const std::string &db,bool create) {
      std::string retval = _db;
      _db = db;

      if(create) {
        createDatabase();
      }

      return retval;
    }

    inline int Server::getPort() {
      return _port;
    }
    inline int Server::setPort(const int port) {
      int retval = _port;
      _port = port;
      _connect = 1;
      return retval;
    }

    inline Document Server::getDocument(const UnicodeString &key) {
      std::string str;
      key.toUTF8String(str);
      return getDocument(str);
    }

    inline Document Server::getDocument(const char *key) {
      return getDocument(std::string(key));
    }

  }
}

#endif

/* eof */
