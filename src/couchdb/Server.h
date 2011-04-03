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

#include <unicode/unistr.h>
#include <string>

#include <curl/curl.h>

#include "Document.h"

namespace CForum {
  namespace CouchDB {
    class Server {
    public:
      Server(const std::string &);
      Server(const std::string &,const std::string &);
      Server(const std::string &,const std::string &,int);

      std::string getHost();
      std::string setHost(const std::string &);

      std::string getDatabase();
      std::string setDatabase(const std::string &);

      int getPort();
      int setPort(int);

      void putDocument(Document &);
      std::string getDocument(std::string &);
      void deleteDocument(Document &);

    private:
      Server();

      void connect();

    protected:
      std::string _host,_db;
      int _port;
      CURL *_curl;
      int _connect;

    };
  }
}

#endif

/* eof */
