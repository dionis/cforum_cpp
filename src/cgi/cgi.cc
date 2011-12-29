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

#include "cgi/cgi.hh"

namespace CForum {
  CGI::CGI() : _get_values(), _post_values(), _cookie_values(), _cgi_values() {}

  CGI::CGI(const CGI &c) : _get_values(c._get_values), _post_values(c._post_values), _cookie_values(c._cookie_values), _cgi_values(c._cgi_values) {
  }

  const CGI &CGI::operator=(const CGI &to_copy) {
    if(this != &to_copy) {
      _get_values    = to_copy._get_values;
      _post_values   = to_copy._post_values;
      _cookie_values = to_copy._cookie_values;
      _cgi_values    = to_copy._cgi_values;
    }

    return *this;
  }

  void CGI::saveParam(const UnicodeString &name, const UnicodeString &value, CGIValueContainer_t *container) {
    CGIValueContainer_t::iterator it = (*container).find(name);

    if(it != container->end()) {
      it->second->push_back(value);
    }
    else {
      ArgumentListType vec(new std::vector<UnicodeString>());
      vec->push_back(value);
      (*container)[name] = vec;
    }
  }

  void CGI::parseString(const char *data, const char realm) {
    const char  *pos = data, *pos1 = data;
    UnicodeString name,value;
    int len = 0, namlen = 0, vallen = 0, num = 0;
    CGIValueContainer_t *container;

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
        std::string str("Unknown parameter realm ");
        str += realm;
        str += ". Valid are G (GET), P (POST) and C (Cookie)";
        throw ParameterException(str,ParameterException::InvalidValue);
    }

    while((pos = strstr(pos1,"=")) != NULL) {
      if(num > 1024) { /* hardcoded maximum number of parameters to parse to prevent hash based DoS */
        throw CGIParserException("Too many values to decode!", CGIParserException::TooManyValues);
      }

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

    ++num;
  }

  CGI CGI::fromCGIEnvironment() {
    CGI c;
    char *data = getenv("QUERY_STRING");
    const char *clen = getenv("CONTENT_LENGTH"),*rqmeth = getenv("REQUEST_METHOD"),**env;

    #ifdef __APPLE__
    const char **environ = const_cast<const char **>(*_NSGetEnviron());
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
      throw CGIParserException("Invalid CGI environment: REQUEST_METHOD missing",CGIParserException::InvalidCGIEnvironment);
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

    for(env = const_cast<const char **>(environ); *env; ++env) {
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

  void CGI::parseHeaderFromCGI(const char *header_val) {
    const char *pos = strstr(header_val,"=");


    std::string name(header_val,pos-header_val);
    std::string value(pos+1);

    name = niceHeaderName(name);

    _headers[name] = value;
  }


  void CGI::parseCookies(const char *cookies) {
    const char *pos = cookies,*pos1 = cookies;
    UnicodeString name,value;
    int len = 0,namlen = 0,vallen = 0;

    if(!cookies) {
      throw CGIParserException("Invalid parameter: no cookie value given!",CGIParserException::NoCookiesGiven);
    }

    while((pos = strstr(pos1,"=")) != NULL) {
      for(;*pos1 && (isspace(*pos1) || *pos1 == ';');++pos1) {}

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

  CGI::ArgumentListType CGI::getValue(const UnicodeString &key, const char *realm) {
    CGIValueContainer_t *m;
    CGIValueContainer_t::iterator it;

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
          std::string str("Unknown parameter realm ");
          str += *ptr;
          str += ". Valid realms are G (GET), P (POST) and C (Cookies).";

          throw ParameterException(str,ParameterException::InvalidValue);
      }

      if((it = m->find(key)) != m->end()) {
        return it->second;
      }
    }

    return ArgumentListType();
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

  UnicodeString CGI::decode(const char *str,size_t len) {
    std::string ustr;

    register const char *ptr;
    char ptr3[3] = { '\0','\0','\0' };

    if(!str) {
      throw CGIParserException("Invalid parameter: string to decode is NULL",CGIParserException::NoDecodeValueGiven);
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

}

/* eof */
