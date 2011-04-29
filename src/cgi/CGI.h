/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CGI interface definition
 * \package cgi
 *
 * This defines the CGI parser interface
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

#ifndef CGI_H
#define CGI_H

#include "config.h"

#include <unistd.h>

#include <unicode/unistr.h>
#include <string>
#include <vector>
#include <map>

#include <boost/shared_ptr.hpp>

#include "hash_map.h"

#include "exceptions/ParameterException.h"
#include "exceptions/CGIParserException.h"

namespace CForum {
  class CGI {
  public:
    typedef boost::shared_ptr<std::vector<UnicodeString> > ArgumentListType;

    CGI();
    CGI(const CGI &);

    const CGI &operator=(const CGI &);

    void parseString(const UnicodeString &, const char = 'G');
    void parseString(const std::string &, const char = 'G');
    void parseString(const char *, const char = 'G');

    void parseCookies(const char *);

    void parseHeaderFromCGI(const char *);

    void setCGIVariable(const char *,const char *);
    std::string &getCGIVariable(const char *);
    std::string &getCGIVariable(const std::string &);

    std::string &getHeader(const char *);
    std::string &getHeader(const std::string &);

    std::string &serverName();
    std::string &serverProtocol();
    int serverPort();
    std::string &requestMethod();
    std::string &pathInfo();
    std::string &scriptName();
    std::string &remoteAddress();

    ArgumentListType getValue(const UnicodeString &, const char * = "GPC");
    ArgumentListType getValue(const std::string &, const char * = "GPC");
    ArgumentListType getValue(const char *, const char * = "GPC");

    const UnicodeString getFirstValue(const UnicodeString &, const char * = "GPC");
    const UnicodeString getFirstValue(const std::string &, const char * = "GPC");
    const UnicodeString getFirstValue(const char *,const char * = "GPC");


    static CGI fromCGIEnvironment();

    static std::string encode(const UnicodeString &);
    std::string encode(const std::string &);
    static std::string encode(const char *);

    static UnicodeString decode(const std::string &);
    static UnicodeString decode(const char *,size_t);

  protected:
    typedef std::hash_map<UnicodeString,ArgumentListType,hash_unicodestring > CGIValueContainer_t;

    void saveParam(const UnicodeString &,const UnicodeString &, CGIValueContainer_t *);

    CGIValueContainer_t _get_values;
    CGIValueContainer_t _post_values;
    CGIValueContainer_t _cookie_values;

    std::hash_map<std::string,std::string, hash_str> _cgi_values;
    std::hash_map<std::string,std::string, hash_str> _headers;
  };

  inline const UnicodeString CGI::getFirstValue(const UnicodeString &key, const char *realm) {
    ArgumentListType u = getValue(key,realm);
    if(u) {
      return u->at(0);
    }

    return UnicodeString();
  }

  inline const UnicodeString CGI::getFirstValue(const std::string &key, const char *realm) {
    ArgumentListType u = getValue(key,realm);
    if(u) {
      return u->at(0);
    }

    return UnicodeString();
  }

  inline const UnicodeString CGI::getFirstValue(const char *key, const char *realm) {
    ArgumentListType u = getValue(key,realm);
    if(u) {
      return u->at(0);
    }

    return UnicodeString();
  }

  inline CGI::ArgumentListType CGI::getValue(const std::string &key, const char *realm) {
    UnicodeString str(key.c_str(),"UTF-8");
    return getValue(str,realm);
  }

  inline CGI::ArgumentListType CGI::getValue(const char *key, const char *realm) {
    UnicodeString str(key,"UTF-8");
    return getValue(str,realm);
  }


  inline std::string CGI::encode(const UnicodeString &str) {
    std::string val;
    str.toUTF8String(val);
    return encode(val.c_str());
  }

  inline std::string CGI::encode(const std::string &str) {
    return encode(str.c_str());
  }

  inline UnicodeString CGI::decode(const std::string &str) {
    return decode(str.c_str(),str.length());
  }


  inline void CGI::setCGIVariable(const char *name,const char *value) {
    std::string nam = name, val = value;
    _cgi_values[name] = value;
  }

  inline std::string &CGI::getCGIVariable(const char *str) {
    return getCGIVariable(std::string(str));
  }

  inline std::string &CGI::getCGIVariable(const std::string &name) {
    return _cgi_values[name];
  }

  inline std::string &CGI::serverName() {
    return getCGIVariable("SERVER_NAME");
  }

  inline std::string &CGI::serverProtocol() {
    return getCGIVariable("SERVER_PROTOCOL");
  }

  inline int CGI::serverPort() {
    std::string str = getCGIVariable("SERVER_PORT");
    return atoi(str.c_str());
  }

  inline std::string &CGI::requestMethod() {
    return getCGIVariable("REQUEST_METHOD");
  }

  inline std::string &CGI::pathInfo() {
    return getCGIVariable("PATH_INFO");
  }

  inline std::string &CGI::scriptName() {
    return getCGIVariable("SCRIPT_NAME");
  }

  inline std::string &CGI::remoteAddress() {
    return getCGIVariable("REMOTE_ADDRESS");
  }


  inline std::string &CGI::getHeader(const char *name) {
    return getHeader(std::string(name));
  }
  inline std::string &CGI::getHeader(const std::string &name) {
    return _headers[name];
  }


  inline void CGI::parseString(const UnicodeString &str, const char realm) {
    std::string ustr;
    str.toUTF8String(ustr);

    parseString(ustr,realm);
  }

  inline void CGI::parseString(const std::string &str, const char realm) {
    parseString(str.c_str(),realm);
  }

}


#endif

/* eof */
