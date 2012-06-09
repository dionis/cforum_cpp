/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CForum model base class
 *
 * CForum model base class
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

#ifndef MODEL_H
#define MODEL_H

#include <boost/shared_ptr.hpp>

#include "framework/mongodb.hh"
#include "json/json_parser.hh"

namespace CForum {
  namespace Models {
    class Model {
    public:
      Model();
      Model(const Model &);

      virtual Model &operator=(const Model &);

      virtual ~Model();

      virtual boost::shared_ptr<JSON::Element> toJSON() = 0;
      virtual boost::shared_ptr<mongo::BSONObj> toBSON() = 0;
    };
  }
}

#endif

/* eof */
