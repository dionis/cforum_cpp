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

#include <unicode/unistr.h>
#include <string>
#include <vector>
#include <map>

#include "exceptions/ParameterException.h"
#include "exceptions/CGIParserException.h"

namespace CForum {
  class CGI {
  public:
    class Parameter {
    public:
      Parameter();
      Parameter(const std::string &,const std::string &);
      Parameter(const UnicodeString &,const UnicodeString &);

      void addValue(const UnicodeString &);
      void addValue(const std::string &);

      const UnicodeString &getValue(size_t) const;
      const UnicodeString &operator[](size_t) const;

      void setValue(size_t,const UnicodeString &);
      void setValue(size_t,const std::string &);

    private:
      UnicodeString _key;
      std::vector<UnicodeString> _value;
    };

    CGI();

    void parseString(const UnicodeString &);
    void parseString(const std::string &);
    void parseString(const char *);

    const Parameter *getValue(const UnicodeString &);
    const Parameter *getValue(const std::string &);
    const Parameter *getValue(const char *);

    const UnicodeString &getFirstValue(const UnicodeString &);
    const UnicodeString &getFirstValue(const std::string &);
    const UnicodeString &getFirstValue(const char *);


    static CGI fromCGIEnvironment();

    static std::string encode(const UnicodeString &);
    std::string encode(const std::string &);
    static std::string encode(const char *);

    static UnicodeString decode(const std::string &);
    static UnicodeString decode(const char *,size_t);

  protected:
    void saveParam(const UnicodeString &,const UnicodeString &);

    std::map <const UnicodeString,Parameter * > _values;

  };
}


#endif

/* eof */
