/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CGI interface implementation: parameter
 * \package cgi
 *
 * This implements the CGI parameter parser interface
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

namespace CForum {
  CGI::Parameter::Parameter() : _key(), _value() {}

  CGI::Parameter::Parameter(const std::string &key,const std::string &value) : _key(key.c_str(),"UTF-8"), _value() {
    UnicodeString str(value.c_str(),"UTF-8");
    _value.push_back(str);
  }

  CGI::Parameter::Parameter(const UnicodeString &key,const UnicodeString &value) : _key(key), _value() {
    _value.push_back(value);
  }

  void CGI::Parameter::addValue(const UnicodeString &str) {
    _value.push_back(str);
  }

  void CGI::Parameter::addValue(const std::string &str) {
    UnicodeString val(str.c_str(),"UTF-8");
    _value.push_back(val);
  }

  const UnicodeString &CGI::Parameter::getValue(size_t pos) const {
    return _value[pos];
  }

  const UnicodeString &CGI::Parameter::operator[](size_t pos) const {
    return getValue(pos);
  }

  void CGI::Parameter::setValue(size_t pos,const UnicodeString &str) {
    _value[pos] = str;
  }

  void CGI::Parameter::setValue(size_t pos,const std::string &str) {
    UnicodeString val(str.c_str(),"UTF-8");
    setValue(pos,val);
  }

}

/* eof */
