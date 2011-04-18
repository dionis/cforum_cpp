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

#include "json/JSONParser.h"
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

      JSON::Element *getValue(const char *);
      JSON::Element *getValue(const std::string &);
      JSON::Element *getValue(const UnicodeString &);

      const JSON::Element *getValue(const char *) const;
      const JSON::Element *getValue(const std::string &) const;
      const JSON::Element *getValue(const UnicodeString &) const;

      void setValue(const std::string &,JSON::Element *);
      void setValue(const UnicodeString &,JSON::Element *);

      std::string toJSON();
      std::string toJSON() const;

      ~Document();

    private:
      JSON::Object *root;
    };

  }
}


#endif

/* eof */
