/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSONArray interface
 * \package JSON
 *
 * This defines the JSONArray interface
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

#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H

#include "config.h"

#include <sstream>
#include <iostream>
#include <string>

#include <vector>
#include <boost/shared_ptr.hpp>

#include "JSONElement.h"

namespace CForum {
  namespace JSON {
    class Array : public Element {
    public:
      typedef std::vector<boost::shared_ptr<Element> > ArrayType_t;

      Array();
      Array(const Array &);

      const Array &operator=(const Array &);

      virtual std::string toJSON();
      virtual ~Array();

      ArrayType_t &getValue();
      const ArrayType_t &getValue() const;

    protected:
      ArrayType_t _data;
    };

    inline Array::ArrayType_t &Array::getValue() {
      return _data;
    }

    inline const Array::ArrayType_t &Array::getValue() const {
      return _data;
    }

  }
}

#endif

/* eof */
