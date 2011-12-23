/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CouchDB interface definition
 * \package couchdb
 *
 * This defines the CouchDB interface
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

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "config.h"

#include <unicode/unistr.h>
#include <string>

#include "json/json_parser.h"
#include "exceptions/CouchErrorException.h"

namespace CForum {
  namespace CouchDB {

    class Document {
    public:
      Document();
      Document(const Document &);
      Document(const UnicodeString &);
      Document(const std::string &);
      Document(const char *);

      UnicodeString getId();
      const UnicodeString getId() const;

      boost::shared_ptr<JSON::Element> getValue(const char *);
      boost::shared_ptr<JSON::Element> getValue(const std::string &);
      boost::shared_ptr<JSON::Element> getValue(const UnicodeString &);

      const boost::shared_ptr<JSON::Element> getValue(const char *) const;
      const boost::shared_ptr<JSON::Element> getValue(const std::string &) const;
      const boost::shared_ptr<JSON::Element> getValue(const UnicodeString &) const;

      void setValue(const std::string &,boost::shared_ptr<JSON::Element>);
      void setValue(const UnicodeString &,boost::shared_ptr<JSON::Element>);

      std::string toJSON();
      std::string toJSON() const;

      ~Document();

    private:
      boost::shared_ptr<JSON::Object> _root;
    };

    inline UnicodeString Document::getId() {
      boost::shared_ptr<JSON::String> el = boost::dynamic_pointer_cast<JSON::String>(getValue("_id"));

      if(!el) {
        throw CouchErrorException("_id value not found!",CouchErrorException::ValueNotFound);
      }

      UnicodeString ustr = el->getValue();
      return ustr;
    }

    inline const UnicodeString Document::getId() const {
      boost::shared_ptr<JSON::String> el = boost::dynamic_pointer_cast<JSON::String>(getValue("_id"));
      if(!el) {
        throw CouchErrorException("_id value not found!",CouchErrorException::ValueNotFound);
      }

      UnicodeString ustr = el->getValue();
      return ustr;
    }

    inline boost::shared_ptr<JSON::Element> Document::getValue(const char *key) {
      return getValue(UnicodeString(key,"UTF-8"));
    }

    inline boost::shared_ptr<JSON::Element> Document::getValue(const std::string &key) {
      UnicodeString str(key.c_str());
      return getValue(str);
    }

    inline const boost::shared_ptr<JSON::Element> Document::getValue(const char *key) const {
      return getValue(UnicodeString(key,"UTF-8"));
    }

    inline const boost::shared_ptr<JSON::Element> Document::getValue(const std::string &key) const {
      UnicodeString str(key.c_str());
      return getValue(str);
    }

    inline void Document::setValue(const std::string &key,boost::shared_ptr<JSON::Element> doc) {
      UnicodeString str(key.c_str());
      setValue(str,doc);
    }

    inline void Document::setValue(const UnicodeString &key,boost::shared_ptr<JSON::Element> doc) {
      JSON::Object::ObjectType_t &mp = _root->getValue();
      mp[key] = doc;
    }

    inline std::string Document::toJSON() {
      if(_root) {
        return _root->toJSON();
      }

      return std::string();
    }

    inline std::string Document::toJSON() const {
      if(_root) {
        return _root->toJSON();
      }

      return std::string();
    }

  }
}


#endif

/* eof */
