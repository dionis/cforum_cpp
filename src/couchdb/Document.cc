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
    Document::Document() : root(new JSON::Object()) {
    }

    Document::Document(const Document &doc) : root(NULL) {
    }

    Document::Document(const UnicodeString &json_str) : root(NULL) {
      std::string str;
      JSON::Parser *prsr = new JSON::Parser();

      json_str.toUTF8String(str);
      prsr->parse(str.c_str(),(JSON::Element **)&root);

      delete prsr;
    }

    Document::Document(const std::string &json_str) : root(NULL) {
      JSON::Parser *prsr = new JSON::Parser();
      prsr->parse(json_str.c_str(),(JSON::Element **)&root);
    }

    UnicodeString Document::getId() {
      JSON::String *el = (JSON::String *)getValue("_id");
      if(!el) {
        throw std::exception(); // TODO: proper exception
      }

      UnicodeString ustr = el->getValue();
      return ustr;
    }

    const UnicodeString Document::getId() const {
      JSON::String *el = (JSON::String *)getValue("_id");
      if(!el) {
        throw std::exception(); // TODO: proper exception
      }

      UnicodeString ustr = el->getValue();
      return ustr;
    }

    JSON::Element *Document::getValue(const char *key) {
      return getValue(UnicodeString(key,"UTF-8"));
    }

    JSON::Element *Document::getValue(const std::string &key) {
      UnicodeString str(key.c_str());
      return getValue(str);
    }

    JSON::Element *Document::getValue(const UnicodeString &key) {
      if(root) {
        std::map<UnicodeString,JSON::Element *> mp = root->getValue();
        return mp[key];
      }

      return NULL;
    }

    const JSON::Element *Document::getValue(const char *key) const {
      return getValue(UnicodeString(key,"UTF-8"));
    }

    const JSON::Element *Document::getValue(const std::string &key) const {
      UnicodeString str(key.c_str());
      return getValue(str);
    }

    const JSON::Element *Document::getValue(const UnicodeString &key) const {
      if(root) {
        std::map<UnicodeString,JSON::Element *> mp = root->getValue();
        return mp[key];
      }

      return NULL;
    }

    void Document::setValue(const std::string &key,JSON::Element *doc) {
      UnicodeString str(key.c_str());
      setValue(str,doc);
    }

    void Document::setValue(const UnicodeString &key,JSON::Element *doc) {
      std::map<UnicodeString,JSON::Element *> mp = root->getValue();
      mp[key] = doc;
    }

    std::string Document::toJSON() {
      if(root) {
        return root->toJSON();
      }

      return std::string();
    }

    std::string Document::toJSON() const {
      if(root) {
        return root->toJSON();
      }

      return std::string();
    }

    Document::~Document() {
      if(root != NULL) {
        delete root;
      }
    }
  }
}

/* eof */
