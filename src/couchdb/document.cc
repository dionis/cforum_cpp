/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CouchDB document interface implementation
 * \package couchdb
 *
 * This implements the CouchDB document interface
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

#include "document.h"

namespace CForum {
  namespace CouchDB {
    Document::Document() : _root(new JSON::Object()) {
    }

    Document::Document(const Document &doc) : _root() {
      _root = boost::shared_ptr<JSON::Object>(new JSON::Object(*(doc._root)));
    }

    Document::Document(const UnicodeString &json_str) : _root() {
      std::string str;
      JSON::Parser *prsr = new JSON::Parser();
      boost::shared_ptr<JSON::Element> el;

      json_str.toUTF8String(str);
      prsr->parse(str.c_str(),el);

      _root = boost::dynamic_pointer_cast<JSON::Object>(el);

      delete prsr;
    }

    Document::Document(const std::string &json_str) : _root() {
      JSON::Parser *prsr = new JSON::Parser();
      boost::shared_ptr<JSON::Element> el;

      prsr->parse(json_str.c_str(),el);

      _root = boost::dynamic_pointer_cast<JSON::Object>(el);

      delete prsr;
    }

    Document::Document(const char *json_str) : _root() {
      JSON::Parser *prsr = new JSON::Parser();
      boost::shared_ptr<JSON::Element> el;

      prsr->parse(json_str,el);

      _root = boost::dynamic_pointer_cast<JSON::Object>(el);

      delete prsr;
    }

    boost::shared_ptr<JSON::Element> Document::getValue(const UnicodeString &key) {
      if(_root) {
        JSON::Object::ObjectType_t &mp = _root->getValue();
        JSON::Object::ObjectType_t::iterator it = mp.find(key);

        if(it != mp.end()) {
          return it->second;
        }
      }

      std::string str;
      key.toUTF8String(str);
      str += " value not found!";
      throw CouchErrorException(str,CouchErrorException::ValueNotFound);

      return boost::shared_ptr<JSON::Element>();
    }

    const boost::shared_ptr<JSON::Element> Document::getValue(const UnicodeString &key) const {
      if(_root) {
        JSON::Object::ObjectType_t &mp = _root->getValue();
        JSON::Object::ObjectType_t::iterator it = mp.find(key);

        if(it != mp.end()) {
          return it->second;
        }
      }

      std::string str;
      key.toUTF8String(str);
      str += " value not found!";
      throw CouchErrorException(str,CouchErrorException::ValueNotFound);

      return boost::shared_ptr<JSON::Element>();
    }

    Document::~Document() {}
  }
}

/* eof */
