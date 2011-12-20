/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSONObject interface
 * \package JSON
 *
 * This defines the JSONObject interface
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

#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H

#include "config.h"

#include <sstream>
#include <iostream>
#include <string>

#include <map>
#include <boost/shared_ptr.hpp>

#include <unicode/unistr.h>
#include <unicode/bytestream.h>

#include "hash_map.h"

#include "JSONElement.h"
#include "JSONString.h"

namespace CForum {
  namespace JSON {
    class Object : public Element {
    public:
      typedef std::unordered_map<UnicodeString,boost::shared_ptr<Element>, hash_unicodestring > ObjectType_t;

      Object();
      Object(const Object &);

      const Object &operator=(const Object &);

      virtual std::string toJSON();
      virtual ~Object();

      ObjectType_t &getValue();
      const ObjectType_t &getValue() const;

    private:
      ObjectType_t _data;

    };

    inline Object::ObjectType_t &Object::getValue() {
      return _data;
    }

    inline const Object::ObjectType_t &Object::getValue() const {
      return _data;
    }

  }
}

#endif

/* eof */
