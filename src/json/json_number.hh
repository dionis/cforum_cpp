/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSONNumber interface
 * \package JSON
 *
 * This defines the JSONNumber interface
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

#ifndef JSON_NUMBER_H
#define JSON_NUMBER_H

#include <sstream>
#include <iostream>
#include <string>

#include "json/json_element.hh"

#include <stdint.h>

namespace CForum {
  namespace JSON {
    enum JSONNumberType {
      JSONNumberTypeDouble,
      JSONNumberTypeInt
    };

    class Number : public Element {
    public:
      Number();
      Number(double);
      Number(int64_t);
      Number(const Number &);

      virtual std::string toJSON() const;
      virtual ~Number();

      int64_t getIValue();
      double getDValue();

      enum JSONNumberType getNumberType();

    protected:
      enum JSONNumberType _ntype;
      double _ddata;
      int _idata;
    };

    inline enum JSONNumberType Number::getNumberType() {
      return _ntype;
    }

    inline int64_t Number::getIValue() {
      return _idata;
    }

    inline double Number::getDValue() {
      return _ddata;
    }

  }
}

#endif

/* eof */
