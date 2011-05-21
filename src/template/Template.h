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

#include <v8.h>

#include "exceptions/CForumException.h"

namespace CForum {
  class Template {
  public:
    typedef void (*sender_t)(const std::string &);

    Template();

    std::string parseFile(const std::string &);

    std::string parseString(const std::string &);
    std::string parseString(const UnicodeString &);
    std::string parseString(const char *,size_t);

    std::string evaluateFile(const std::string &);
    std::string evaluateString(const std::string &);

    std::string evaluate(const std::string &);

    std::string evaluate(const v8::Handle<v8::Script> &);

    v8::Handle<v8::Script> compile(const std::string &);

    void setSender(sender_t);
    sender_t getSender();

    ~Template();

  private:
    enum {
      TemplateParseModeString,
      TemplateParseModeInJS
    } TemplateParseMode;

    class Global {
    public:
      v8::Handle<v8::ObjectTemplate> _global;

      Global();
      v8::Handle<v8::ObjectTemplate> &getGlobal();
    };

    sender_t _sender;

    v8::HandleScope _handle_scope;
    Template::Global _global;
    v8::Persistent<v8::Context> _context;
    v8::Context::Scope _scope;
    v8::Handle<v8::Object> _vars;

    friend v8::Handle<v8::Value> _pCallback(const v8::Arguments &);
    friend v8::Handle<v8::Value> _eCallback(const v8::Arguments &);
    friend v8::Handle<v8::Value> _vCallback(const v8::Arguments &);

  };

  inline v8::Handle<v8::ObjectTemplate> &Template::Global::getGlobal() {
    return _global;
  }

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

  inline std::string Template::evaluateFile(const std::string &fname) {
    return evaluate(parseFile(fname));
  }

  inline std::string Template::evaluateString(const std::string &str) {
    return evaluate(parseString(str));
  }

  inline std::string Template::evaluate(const std::string &str) {
    return evaluate(compile(str));
  }

}

#endif

/* eof */
