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

#include "cgi/CGI.h"
#include "Document.h"

namespace CForum {
  namespace CouchDB {
    class Server {
    public:
      Server(const std::string &);
      Server(const std::string &,const std::string &);
      Server(const std::string &,const std::string &,const std::string &);
      Server(const std::string &,const std::string &,const std::string &,int);

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

      void deleteDocument(const Document &);
      void deleteDocument(const std::string &);

      void setAuth(const std::string &,const std::string &);

      class Chunk {
      public:
        Chunk();
        Chunk(const std::string &);
        Chunk(const char *,size_t);

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
  }
}

#endif

/* eof */
