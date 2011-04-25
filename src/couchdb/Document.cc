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

#include "Document.h"

namespace CForum {
  namespace CouchDB {
    Document::Document() : _root(new JSON::Object()) {
    }

    Document::Document(const Document &doc) : _root() {
      (void)doc; /* TODO: write copy constructor */
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

    UnicodeString Document::getId() {
      boost::shared_ptr<JSON::String> el = boost::dynamic_pointer_cast<JSON::String>(getValue("_id"));

      if(!el) {
        throw CouchErrorException("_id value not found!",CouchErrorException::ValueNotFound);
      }

      UnicodeString ustr = el->getValue();
      return ustr;
    }

    const UnicodeString Document::getId() const {
      boost::shared_ptr<JSON::String> el = boost::dynamic_pointer_cast<JSON::String>(getValue("_id"));
      if(!el) {
        throw CouchErrorException("_id value not found!",CouchErrorException::ValueNotFound);
      }

      UnicodeString ustr = el->getValue();
      return ustr;
    }

    boost::shared_ptr<JSON::Element> Document::getValue(const char *key) {
      return getValue(UnicodeString(key,"UTF-8"));
    }

    boost::shared_ptr<JSON::Element> Document::getValue(const std::string &key) {
      UnicodeString str(key.c_str());
      return getValue(str);
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

    const boost::shared_ptr<JSON::Element> Document::getValue(const char *key) const {
      return getValue(UnicodeString(key,"UTF-8"));
    }

    const boost::shared_ptr<JSON::Element> Document::getValue(const std::string &key) const {
      UnicodeString str(key.c_str());
      return getValue(str);
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

    void Document::setValue(const std::string &key,boost::shared_ptr<JSON::Element> doc) {
      UnicodeString str(key.c_str());
      setValue(str,doc);
    }

    void Document::setValue(const UnicodeString &key,boost::shared_ptr<JSON::Element> doc) {
      JSON::Object::ObjectType_t &mp = _root->getValue();
      mp[key] = doc;
    }

    std::string Document::toJSON() {
      if(_root) {
        return _root->toJSON();
      }

      return std::string();
    }

    std::string Document::toJSON() const {
      if(_root) {
        return _root->toJSON();
      }

      return std::string();
    }

    Document::~Document() {}
  }
}

/* eof */
