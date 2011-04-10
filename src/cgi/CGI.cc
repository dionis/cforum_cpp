/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CGI interface implementation
 * \package cgi
 *
 * This implements the CGI parser interface
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

#include "CGI.h"

#ifdef __APPLE__
#include <crt_externs.h>
#endif

namespace CForum {
  CGI::CGI() : _get_values(), _post_values(), _cookie_values(), _cgi_values() {}

  void CGI::parseString(const UnicodeString &str, const char realm) {
    std::string ustr;
    str.toUTF8String(ustr);

    parseString(ustr,realm);
  }

  void CGI::parseString(const std::string &str, const char realm) {
    parseString(str.c_str(),realm);
  }

  void CGI::saveParam(const UnicodeString &name, const UnicodeString &value, std::map<const UnicodeString, Parameter *> *container) {
    CGI::Parameter *val = (*container)[name];
    if(val) {
      val->addValue(value);
    }
    else {
      val = new CGI::Parameter(name,value);
      (*container)[name] = val;
    }
  }

  void CGI::parseString(const char *data, const char realm) {
    const char  *pos = data,*pos1 = data;
    UnicodeString name,value;
    int len = 0,namlen = 0,vallen = 0;
    std::map<const UnicodeString, Parameter *> *container;

    switch(realm) {
      case 'G':
        container = &_get_values;
        break;
      case 'P':
        container = &_post_values;
        break;
      case 'C':
        container = &_cookie_values;
        break;
      default:
        throw ParameterException(); // TODO: proper exception
    }

    while((pos = strstr(pos1,"=")) != NULL) {
      namlen = pos - pos1;
      name   = CGI::decode(pos1,namlen);

      pos1 = strstr(pos,"&");
      if(!pos1) {
        break;
      }

      ++pos;

      vallen  = pos1 - pos;
      value   = CGI::decode(pos,vallen);

      saveParam(name,value,container);

      ++pos1;
    }

    if(pos && *pos) {
      len   = strlen(pos+1);
      value = CGI::decode(pos+1,len);

      saveParam(name,value,container);
    }
  }

  CGI CGI::fromCGIEnvironment() {
    CGI c;
    char *data = getenv("QUERY_STRING");
    const char *clen = getenv("CONTENT_LENGTH"),*rqmeth = getenv("REQUEST_METHOD"),**env;

    #ifdef __APPLE__
    const char **environ = (const char **)(*_NSGetEnviron());
    #endif

    size_t len = 0;

    const char *cgi_variables[] = {
      "SERVER_SOFTWARE","SERVER_NAME","GATEWAY_INTERFACE",
      "SERVER_PROTOCOL","SERVER_PORT","REQUEST_METHOD",
      "PATH_INFO","PATH_TRANSLATED","SCRIPT_NAME",
      "REMOTE_HOST","REMOTE_ADDR","AUTH_TYPE",
      "REMOTE_USER","REMOTE_IDENT","CONTENT_TYPE",
      "CONTENT_LENGTH", NULL
    };

    const char **var;

    if(!rqmeth) {
      throw ParameterException(); // TODO: proper exception
    }

    if(data != NULL && *data) {
      c.parseString(data,'G');
    }

    if(clen && strcmp(rqmeth,"POST") == 0) {
      len = strtol(clen,NULL,10);
      data  = new char[len + 1];

      fread(data,1,len,stdin);
      data[len] = '\0';

      c.parseString(data,'P');

      delete data;
    }

    if((data = getenv("HTTP_COOKIE")) != NULL) {
      c.parseCookies(data);
    }


    for(var=cgi_variables;*var;++var) {
      if((data = getenv(*var)) != NULL) {
        c.setCGIVariable(*var,data);
      }
    }

    for(env = environ; *env; ++env) {
      if(strncmp(*env,"HTTP_",5) == 0 && strcmp(*env,"HTTP_COOKIE") != 0) {
        c.parseHeaderFromCGI(*env);
      }
    }

    return c;
  }

  std::string niceHeaderName(const std::string &str) {
    std::string nname = str.substr(5);
    size_t i;

    for(i=1;i<nname.length();++i) {
      if(nname[i] == '_') {
        nname[i++] = '-';
      }
      else {
        nname[i] = tolower(nname[i]);
      }
    }

    return nname;
  }

  std::string &CGI::getHeader(const char *name) {
    return getHeader(std::string(name));
  }
  std::string &CGI::getHeader(const std::string &name) {
    return _headers[name];
  }

  void CGI::parseHeaderFromCGI(const char *header_val) {
    const char *pos = strstr(header_val,"=");


    std::string name(header_val,pos-header_val);
    std::string value(pos+1);

    name = niceHeaderName(name);

    _headers[name] = value;
  }

  void CGI::setCGIVariable(const char *name,const char *value) {
    std::string nam = name, val = value;
    _cgi_values[name] = value;
  }

  std::string &CGI::getCGIVariable(const char *str) {
    return getCGIVariable(std::string(str));
  }

  std::string &CGI::getCGIVariable(const std::string &name) {
    return _cgi_values[name];
  }

  std::string &CGI::serverName() {
    return getCGIVariable("SERVER_NAME");
  }

  std::string &CGI::serverProtocol() {
    return getCGIVariable("SERVER_PROTOCOL");
  }

  int CGI::serverPort() {
    std::string str = getCGIVariable("SERVER_PORT");
    return atoi(str.c_str());
  }

  std::string &CGI::requestMethod() {
    return getCGIVariable("REQUEST_METHOD");
  }

  std::string &CGI::pathInfo() {
    return getCGIVariable("PATH_INFO");
  }

  std::string &CGI::scriptName() {
    return getCGIVariable("SCRIPT_NAME");
  }

  std::string &CGI::remoteAddress() {
    return getCGIVariable("REMOTE_ADDRESS");
  }


  void CGI::parseCookies(const char *cookies) {
    const char *pos = cookies,*pos1 = cookies;
    UnicodeString name,value;
    int len = 0,namlen = 0,vallen = 0;

    if(!cookies) {
      throw ParameterException(); // TODO: proper exception
    }

    while((pos = strstr(pos1,"=")) != NULL) {
      for(;*pos1 && (isspace(*pos1) || *pos1 == ';');++pos1);

      namlen = pos - pos1;
      name   = CGI::decode(pos1,namlen);

      pos1 = strstr(pos,";");
      if(!pos1) {
        break;
      }

      vallen  = pos1 - pos;
      value   = CGI::decode(pos+1,vallen-1);

      saveParam(name,value,&_cookie_values);
    }

    if(pos && *pos) {
      len   = strlen(pos+1);
      value = CGI::decode(pos+1,len);

      saveParam(name,value,&_cookie_values);
    }
  }

  const CGI::Parameter *CGI::getValue(const UnicodeString &key, const char *realm) {
    std::map<const UnicodeString,CGI::Parameter *> *m;

    for(const char *ptr = realm;*ptr;++ptr) {
      switch(*ptr) {
        case 'G':
          m = &_get_values;
          break;
        case 'P':
          m = &_post_values;
          break;
        case 'C':
          m = &_cookie_values;
          break;
        default:
          throw ParameterException(); // TODO: proper exception
      }

      return (*m)[key];
    }

    return NULL;
  }

  const CGI::Parameter *CGI::getValue(const std::string &key, const char *realm) {
    UnicodeString str(key.c_str(),"UTF-8");
    return getValue(str,realm);
  }

  const CGI::Parameter *CGI::getValue(const char *key, const char *realm) {
    UnicodeString str(key,"UTF-8");
    return getValue(str,realm);
  }

  const UnicodeString &CGI::getFirstValue(const UnicodeString &key, const char *realm) {
    const CGI::Parameter *p = getValue(key,realm);
    if(p) {
      return p->getValue(0);
    }

    return UnicodeString();
  }

  const UnicodeString &CGI::getFirstValue(const std::string &key, const char *realm) {
    const CGI::Parameter *p = getValue(key,realm);
    if(p) {
      return p->getValue(0);
    }

    return UnicodeString();
  }

  const UnicodeString &CGI::getFirstValue(const char *key, const char *realm) {
    const CGI::Parameter *p = getValue(key,realm);
    if(p) {
      return p->getValue(0);
    }

    return UnicodeString();
  }




  std::string CGI::encode(const UnicodeString &str) {
    std::string val;
    str.toUTF8String(val);
    return encode(val.c_str());
  }

  std::string CGI::encode(const std::string &str) {
    return encode(str.c_str());
  }

  std::string CGI::encode(const char *str) {
    std::string ret;
    const char *ptr;
    char buff[10];

    for(ptr=str;*ptr;++ptr) {
      if((*ptr >= 48 && *ptr <= 122) || (*ptr == '_' || *ptr == '.' || *ptr == '-')) {
        ret += *ptr;
      }
      else {
        if(*ptr == ' ') {
          ret += '+';
        }
        else {
          snprintf(buff,10,"%%%02X",(int)*ptr);
          ret += buff;
        }
      }
    }

    return ret;
  }

  UnicodeString CGI::decode(const std::string &str) {
    return decode(str.c_str(),str.length());
  }

  UnicodeString CGI::decode(const char *str,size_t len) {
    std::string ustr;

    register const char *ptr;
    char ptr3[3] = { '\0','\0','\0' };

    if(!str) {
      throw ParameterException(); // TODO: proper exception
    }

    for(ptr=str;(size_t)(ptr-str) < len;++ptr) {
      switch(*ptr) {
        case '+':
          ustr += ' ';
          break;

        case '%':
          /* string is to short; copy remaining bytes and return */
          if((size_t)(ptr+2 - str) >= len) {
            ustr += '%';
            if((size_t)(ptr+1 - str) < len) {
              ustr += *(ptr+1);
            }

            return UnicodeString(ustr.c_str(),"UTF-8");
          }

          memcpy(ptr3,ptr+1,2);
          ustr += (char)strtol(ptr3,NULL,16);
          ptr += 2;
          break;

        default:
          ustr += (UChar)*ptr;
      }
    }


    return UnicodeString(ustr.c_str(),"UTF-8");
  }

  CGI::~CGI() {
    std::map<const UnicodeString,CGI::Parameter * >::iterator end = _get_values.end(), it;
    for(it = _get_values.begin(); it != end; ++it) {
      delete it->second;
    }

    end = _post_values.end();
    for(it = _post_values.begin(); it != end; ++it) {
      delete it->second;
    }

    end = _cookie_values.end();
    for(it = _cookie_values.begin(); it != end; ++it) {
      delete it->second;
    }
  }

}

/* eof */
