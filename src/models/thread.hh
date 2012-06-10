/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CForum thread model
 *
 * CForum thread model
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

#ifndef THREAD_H
#define THREAD_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "json/json_parser.hh"
#include "framework/model.hh"
#include "framework/mongodb.hh"

#include "models/message.hh"

namespace CForum {
  namespace Models {
    class Thread : public Model {
    public:
      Thread();
      Thread(const Thread &);

      virtual Thread &operator=(const Thread &);

      virtual ~Thread();

      virtual boost::shared_ptr<Message> getMessage(const std::string &);
      virtual boost::shared_ptr<Message> getMessage(const std::string &, boost::shared_ptr<Message>);

      static boost::shared_ptr<Thread> fromJSON(const JSON::Object &);
      static boost::shared_ptr<Thread> fromBSON(const mongo::BSONObj &);

      virtual boost::shared_ptr<JSON::Element> toJSON();
      virtual boost::shared_ptr<mongo::BSONObj> toBSON();
      virtual v8::Local<v8::Object> toV8();

      std::string id, tid;
      std::vector<boost::shared_ptr<Message> > messages;
      bool archived;
    };
  }
}

#endif

/* eof */
