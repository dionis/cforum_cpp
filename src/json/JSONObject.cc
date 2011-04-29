/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSONObject interface
 * \package JSON
 *
 * This implements the JSONObject interface
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

#include "JSONObject.h"

#include "JSONParser.h"

namespace CForum {
  namespace JSON {
    Object::Object() : Element(JSONTypeObject), _data() {}
    Object::Object(const Object &obj) : Element(obj.getType()), _data(obj._data) {}

    const Object &Object::operator=(const Object &obj) {
      if(this != &obj) {
        _data = obj._data;
      }

      return *this;
    }

    std::string Object::toJSON() {
      std::ostringstream ostr;

      bool isfirst = true;

      ObjectType_t::iterator end = _data.end();
      ObjectType_t::iterator it;

      ostr << "{";

      for(it = _data.begin(); it != end; ++it) {
        if(!isfirst) {
          ostr << ",";
        }

        isfirst = false;

        ostr << String::toJSONString(it->first);
        ostr << ":";
        ostr << it->second->toJSON();
      }

      ostr << "}";

      return ostr.str();
    }

    Object::~Object() {}

  }
}

/* eof */
