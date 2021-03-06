/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSONBoolean interface
 * \package JSON
 *
 * This defines the JSONBoolean interface
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

#ifndef JSON_BOOLEAN_H
#define JSON_BOOLEAN_H

#include "json/json_element.hh"

namespace CForum {
  namespace JSON {
    class Boolean : public Element {
    public:
      Boolean();
      Boolean(bool);
      Boolean(const Boolean &);

      virtual std::string toJSON() const;
      virtual ~Boolean();

      bool getValue();

    protected:
      bool _data;
    };

    inline std::string Boolean::toJSON() const {
      if(_data) {
        return std::string("true");
      }

      return std::string("false");
    }

    inline bool Boolean::getValue() {
      return _data;
    }

  }
}

#endif

/* eof */
