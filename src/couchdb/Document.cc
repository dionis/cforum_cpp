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
    Document::Document() {
    }

    Document::Document(const Document &doc) {
    }

    Document::Document(const UnicodeString &json_str) {
    }

    Document::Document(const std::string &json_str) {
    }

    Document Document::getKey(const std::string &key) {
    }

    Document Document::getKey(const UnicodeString &key) {
    }

    void Document::setKey(const std::string &key,const Document &doc) {
    }

    void Document::setKey(const UnicodeString &key,const Document &doc) {
    }

    UnicodeString Document::toJSON() {
      return UnicodeString();
    }

    Document::~Document() {
    }
  }
}

/* eof */
