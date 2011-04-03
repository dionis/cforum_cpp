/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSONNumber interface
 * \package JSON
 *
 * This implements the JSONNumber interface
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

#include "JSONNumber.h"

namespace CForum {
  namespace JSON {
    Number::Number() : Element(JSONTypeNumber), _ntype(JSONNumberTypeInt), _ddata(0), _idata(0) {
    }

    Number::Number(double dval) : Element(JSONTypeNumber), _ntype(JSONNumberTypeDouble), _ddata(dval), _idata(0) {
    }

    Number::Number(int64_t ival) : Element(JSONTypeNumber), _ntype(JSONNumberTypeInt), _ddata(0), _idata(ival) {
    }

    enum JSONNumberType Number::getNumberType() {
      return _ntype;
    }

    std::string Number::toJSON() {
      std::ostringstream ostr;


      if(_ntype == JSONNumberTypeDouble) {
        ostr << _ddata;
      }
      else {
        ostr << _idata;
      }

      return ostr.str();
    }

    int64_t Number::getIValue() {
      return _idata;
    }

    double Number::getDValue() {
      return _ddata;
    }

    Number::~Number() {
    }
  }
}

/* eof */
