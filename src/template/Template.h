/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief The template class
 * \package templates
 *
 * Contains the basic template class
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

#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <fstream>
#include <iterator>

#include <unicode/unistr.h>
#include <unicode/uchriter.h>

#include <string>

#include "exceptions/CForumException.h"

#include "jsevaluator/JSEvaluator.h"


namespace CForum {
  class Template {
  public:
    typedef void (*sender_t)(const std::string &);

    Template();

    std::string parseFile(const std::string &);

    std::string parseString(const std::string &);
    std::string parseString(const UnicodeString &);
    std::string parseString(const char *,size_t);

    void setSender(sender_t);
    sender_t getSender();

  private:
    enum {
      TemplateParseModeString,
      TemplateParseModeInJS
    } TemplateParseMode;

    sender_t _sender;
  };

  inline void Template::setSender(Template::sender_t sender) {
    _sender = sender;
  }

  inline Template::sender_t Template::getSender() {
    return _sender;
  }

  inline std::string Template::parseString(const std::string &str) {
    return parseString(str.c_str(),str.length());
  }

  inline std::string Template::parseString(const UnicodeString &str) {
    std::string utf8str;
    str.toUTF8String(utf8str);

    return parseString(utf8str);
  }

}

#endif

/* eof */
