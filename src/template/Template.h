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

#ifndef TEMPLATE_H
#define TEMPLATE_H

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
    typedef void (*sender_t)(const std::string &,void *userdata);

    Template();

    std::string parseFile(const std::string &);

    std::string parseString(const std::string &);
    std::string parseString(const UnicodeString &);
    std::string parseString(const char *,size_t);

    std::string evaluateFile(const std::string &);
    std::string evaluateString(const std::string &);

    void displayFile(const std::string &);
    void displayString(const std::string &);

    std::string evaluate(const std::string &);
    std::string evaluate(const v8::Handle<v8::Script> &);

    void display(const std::string &);
    void display(const v8::Handle<v8::Script> &);

    v8::Handle<v8::Script> compile(const std::string &);

    void setVariable(const UnicodeString &,v8::Handle<v8::Value>);
    void setVariable(const char *nam,v8::Handle<v8::Value>);

    v8::Handle<v8::Value> getVariable(const UnicodeString &);
    v8::Handle<v8::Value> getVariable(const char *);

    v8::Handle<v8::Value> getVariable(const v8::Handle<v8::Value>);

    void setSender(sender_t);
    sender_t getSender();

    void setUserdata(void *);
    void *getUserdata();

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

    void *_userdata;
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

  inline void Template::setUserdata(void *udata) {
    _userdata = udata;
  }

  inline void *Template::getUserdata() {
    return _userdata;
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

  inline void Template::displayFile(const std::string &fname) {
    return display(parseFile(fname));
  }

  inline std::string Template::evaluateString(const std::string &str) {
    return evaluate(parseString(str));
  }

  inline void Template::displayString(const std::string &str) {
    return display(parseString(str));
  }

  inline std::string Template::evaluate(const std::string &str) {
    return evaluate(compile(str));
  }

  inline void Template::display(const std::string &str) {
    return display(compile(str));
  }

  inline void setVariable(const UnicodeString &nam,v8::Handle<v8::Value> val) {
    std::string name;
    nam.toUTF8String(name);
    setVariable(name.c_str(),val);
  }

  inline v8::Handle<v8::Value> Template::getVariable(const UnicodeString &nam) {
    std::string name;
    nam.toUTF8String(name);
    return getVariable(name.c_str());
  }

  inline v8::Handle<v8::Value> Template::getVariable(const char *nam) {
    v8::Handle<v8::String> str = v8::String::New(nam);
    return getVariable(str);
  }

  inline v8::Handle<v8::Value> Template::getVariable(const v8::Handle<v8::Value> nam) {
    return _vars->Get(nam);
  }

  inline void Template::setVariable(const char *nam,v8::Handle<v8::Value> val) {
    v8::Handle<v8::String> name = v8::String::New(nam);

    _vars->Set(name,val);
  }

}

#endif

/* eof */
