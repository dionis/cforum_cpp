/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSONBoolean interface
 * \package JSON
 *
 * This implements the JSONBoolean interface
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

#include "JSONBoolean.h"

namespace CForum {
  namespace JSON {
    Boolean::Boolean() : Element(JSONTypeBoolean), _data(false) {}
    Boolean::Boolean(bool val) : Element(JSONTypeBoolean), _data(val) {}
    Boolean::Boolean(const Boolean &b) : Element(JSONTypeBoolean), _data(b._data) {}

    std::string Boolean::toJSON() {
      if(_data) {
        return std::string("true");
      }

      return std::string("false");
    }

    bool Boolean::getValue() {
      return _data;
    }

    Boolean::~Boolean() {
    }

  }
}

/* eof */
