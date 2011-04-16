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
    size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
      size_t bytes = nmemb * size;
      Server::Chunk *chk = (Server::Chunk *)userp;
      const std::string &buff = chk->getBuff();
      size_t pos = chk->getPos();

      if(buff.size() - pos <= 0) {
        return 0;
      }

      if(buff.size() - pos <= bytes) {
        bytes = buff.size() - pos;
      }

      memcpy(buffer,buff.c_str()+pos,bytes);

      pos += bytes;
      chk->setPos(pos);

      return bytes;
    }

    size_t read_data(char *bufptr, size_t size, size_t nmemb, void *userp) {
      size_t bytes = nmemb * size;
      std::string str(bufptr,bytes);
      Server::Response *rsp = (Server::Response *)userp;

      rsp->addContent(str);

      return bytes;
    }

    size_t read_header(char *bufptr, size_t size, size_t nmemb, void *userp) {
      size_t bytes = nmemb * size;
      Server::Response *rsp = (Server::Response *)userp;
      char *ptr,*pos = NULL;

      if(strncmp(bufptr,"HTTP/",5) == 0) {
        pos = bufptr + 5;
        rsp->setVersion(strtod(pos,&pos));

        for(++pos;pos < bufptr + bytes && isspace(*pos);++pos);
        rsp->setStatus(strtol(pos,&pos,10));

        for(++pos;pos < bufptr + bytes && isspace(*pos);++pos);
        ptr = (char *)(pos + bytes - (char *)(pos - bufptr) - 1);
        for(;ptr > pos && (*ptr == '\015' || *ptr == '\012');--ptr);

        rsp->setMessage(std::string(pos,ptr-pos+1));
        return bytes;
      }

      for(ptr=bufptr;ptr < bufptr+bytes;++ptr) {
        if(*ptr == ':') {
          pos = ptr;
          for(++ptr;ptr < bufptr+bytes && isspace(*ptr);++ptr);
          break;
        }
      }

      if(pos) {
        std::string name(bufptr,pos-bufptr);
        std::string val(ptr,bytes-(size_t)(ptr-bufptr));

        rsp->addHeader(name,val);
      }
      else {
        for(ptr=bufptr;*ptr && ptr < bufptr+bytes;++ptr) {
          if(*ptr != '\012' && *ptr != '\015') {
            return 0;
          }
        }
      }

      return bytes;
    }


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

    void Server::createDatabase() {
      Server::Response rsp;
      CURLcode cd;
      std::string uri = genURI(std::string());

      connect();

      curl_easy_setopt(_curl,CURLOPT_PUT,1);
      curl_easy_setopt(_curl,CURLOPT_URL,uri.c_str());
      curl_easy_setopt(_curl,CURLOPT_INFILESIZE,0);
      curl_easy_setopt(_curl,CURLOPT_HEADERDATA,&rsp);
      curl_easy_setopt(_curl,CURLOPT_WRITEDATA,&rsp);

      if((cd = curl_easy_perform(_curl)) != CURLE_OK) {
        throw CouchErrorException("Error performing HTTP request to CouchDB server!",ErrorCodeCouchDBHttpStatusError,curl_easy_strerror(cd),cd);
      }

      if(rsp.getStatus() != 201) {
        char buff[50];
        snprintf(buff,50,"%d",rsp.getStatus());

        std::string str("Expected HTTP status 201, got ");
        str += buff;
        str += " ";
        str += rsp.getMessage();

        throw CouchErrorException(str,ErrorCodeCouchDBHttpError);
      }
    }

    void Server::deleteDatabase() {
      Server::Response rsp;
      CURLcode cd;
      std::string uri = genURI(std::string());

      connect();

      curl_easy_setopt(_curl,CURLOPT_CUSTOMREQUEST,"DELETE");
      curl_easy_setopt(_curl,CURLOPT_URL,uri.c_str());
      curl_easy_setopt(_curl,CURLOPT_INFILESIZE,0);
      curl_easy_setopt(_curl,CURLOPT_HEADERDATA,&rsp);
      curl_easy_setopt(_curl,CURLOPT_WRITEDATA,&rsp);

      if((cd = curl_easy_perform(_curl)) != CURLE_OK) {
        throw CouchErrorException("Error performing HTTP request to CouchDB server!",ErrorCodeCouchDBHttpStatusError,curl_easy_strerror(cd),cd);
      }

      if(rsp.getStatus() != 200) {
        char buff[50];
        snprintf(buff,50,"%d",rsp.getStatus());

        std::string str("Expected HTTP status 201, got ");
        str += buff;
        str += " ";
        str += rsp.getMessage();

        throw CouchErrorException(str,ErrorCodeCouchDBHttpError);
      }
    }

    std::string Server::getDatabase() {
      return _db;
    }
    std::string Server::setDatabase(const std::string &db,bool create) {
      std::string retval = _db;
      _db = db;

      if(create) {
        createDatabase();
      }

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

        curl_easy_setopt(_curl,CURLOPT_WRITEFUNCTION, CForum::CouchDB::read_data);
        curl_easy_setopt(_curl,CURLOPT_READFUNCTION, CForum::CouchDB::write_data);
        curl_easy_setopt(_curl,CURLOPT_HEADERFUNCTION, CForum::CouchDB::read_header);

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
      char buff[50];

      snprintf(buff,50,"%d",_port);

      url += "://";
      url += _host;
      url += ":";
      url += buff;
      url += "/";
      url += _db;

      if(key.length() > 0) {
        url += "/";
        url += CGI::encode(key.c_str());
      }

      return url;
    }

    void Server::putDocument(const Document &doc) {
      Server::Response rsp;
      Server::Chunk chunk(doc.toJSON());
      std::string &json_str = chunk.getBuff();
      std::string str;
      std::string url;
      CURLcode cd;

      doc.getId().toUTF8String(str);
      url = genURI(str);

      connect();

      curl_easy_setopt(_curl,CURLOPT_PUT,1);
      curl_easy_setopt(_curl,CURLOPT_URL,url.c_str());
      curl_easy_setopt(_curl,CURLOPT_INFILESIZE,json_str.size());
      curl_easy_setopt(_curl,CURLOPT_HEADERDATA,&rsp);
      curl_easy_setopt(_curl,CURLOPT_WRITEDATA,&rsp);
      curl_easy_setopt(_curl,CURLOPT_READDATA,&chunk);

      if((cd = curl_easy_perform(_curl)) != CURLE_OK) {
        throw CouchErrorException("Error performing HTTP request to CouchDB server!",ErrorCodeCouchDBHttpStatusError,curl_easy_strerror(cd),cd);
      }

      if(rsp.getStatus() != 201) {
        char buff[50];
        snprintf(buff,50,"%d",rsp.getStatus());

        std::string str("Expected HTTP status 201, got ");
        str += buff;
        str += " ";
        str += rsp.getMessage();

        throw CouchErrorException(str,ErrorCodeCouchDBHttpError);
      }

    }

    Document Server::getDocument(const std::string &key) {
      std::string url = genURI(key);
      CURLcode cd;
      Server::Response rsp;

      connect();

      curl_easy_setopt(_curl,CURLOPT_HTTPGET,1);
      curl_easy_setopt(_curl,CURLOPT_URL,url.c_str());
      curl_easy_setopt(_curl,CURLOPT_INFILESIZE,0);
      curl_easy_setopt(_curl,CURLOPT_HEADERDATA,&rsp);
      curl_easy_setopt(_curl,CURLOPT_WRITEDATA,&rsp);

      if((cd = curl_easy_perform(_curl)) != CURLE_OK) {
        throw CouchErrorException("Error performing HTTP request to CouchDB server!",ErrorCodeCouchDBHttpStatusError,curl_easy_strerror(cd),cd);
      }

      if(rsp.getStatus() != 200) {
        char buff[50];
        snprintf(buff,50,"%d",rsp.getStatus());

        std::string str("Expected HTTP status 201, got ");
        str += buff;
        str += " ";
        str += rsp.getMessage();

        throw CouchErrorException(str,ErrorCodeCouchDBHttpError);
      }

      return Document(rsp.getContent());
    }

    Document Server::getDocument(const UnicodeString &key) {
      std::string str;
      key.toUTF8String(str);
      return getDocument(str);
    }

    Document Server::getDocument(const char *key) {
      return getDocument(std::string(key));
    }

    void Server::deleteDocument(const Document &doc) {
      std::string str;
      std::string url;
      Server::Response rsp;
      CURLcode cd;

      doc.getId().toUTF8String(str);

      url = genURI(str);

      connect();

      curl_easy_setopt(_curl,CURLOPT_CUSTOMREQUEST,"DELETE");
      curl_easy_setopt(_curl,CURLOPT_URL,url.c_str());
      curl_easy_setopt(_curl,CURLOPT_INFILESIZE,0);
      curl_easy_setopt(_curl,CURLOPT_HEADERDATA,&rsp);
      curl_easy_setopt(_curl,CURLOPT_WRITEDATA,&rsp);

      if((cd = curl_easy_perform(_curl)) != CURLE_OK) {
        throw CouchErrorException("Error performing HTTP request to CouchDB server!",ErrorCodeCouchDBHttpStatusError,curl_easy_strerror(cd),cd);
      }

      if(rsp.getStatus() != 200) {
        char buff[50];
        snprintf(buff,50,"%d",rsp.getStatus());

        std::string str("Expected HTTP status 201, got ");
        str += buff;
        str += " ";
        str += rsp.getMessage();

        throw CouchErrorException(str,ErrorCodeCouchDBHttpError);
      }
    }

  }
}


/* eof */
