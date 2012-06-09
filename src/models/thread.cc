/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CForum thread model implementation
 *
 * CForum thread model implementation
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

#include "models/thread.hh"

namespace CForum {
  namespace Models {
    Thread::Thread() : Model(), id(), tid(), messages(), archived(false) { }
    Thread::Thread(const Thread &t) : Model(), id(t.id), tid(t.tid), messages(t.messages), archived(t.archived) { } // TODO: create a COPY of t.messages

    Thread &Thread::operator=(const Thread &t) {
      if(this != &t) {
        tid      = t.tid;
        id       = t.id;
        messages = t.messages;  // TODO: create a COPY of messages
        archived = t.archived;
      }

      return *this;
    }

    Thread::~Thread() { }

    boost::shared_ptr<Thread> Thread::fromJSON(const JSON::Object &) { // TODO: implement
      return boost::shared_ptr<Thread>();
    }

    boost::shared_ptr<Thread> Thread::fromBSON(const mongo::BSONObj &o) {
      boost::shared_ptr<Thread> t = boost::make_shared<Thread>();

      t->tid      = o.getField("tid").String();
      t->id       = o.getField("_id").String();
      t->archived = o.getField("archived").Bool();

      const std::vector<mongo::BSONElement> msgs = o.getField("messages").Array();
      std::vector<mongo::BSONElement>::const_iterator it, end = msgs.end();
      for(it = msgs.begin(); it != end; ++it) {
        t->messages.push_back(Message::fromBSON(it->Obj()));
      }

      return t;
    }

    boost::shared_ptr<JSON::Element> Thread::toJSON() {
      return boost::shared_ptr<JSON::Object>();
    }

    boost::shared_ptr<mongo::BSONObj> Thread::toBSON() {
      return boost::shared_ptr<mongo::BSONObj>();
    }

    v8::Local<v8::Object> Thread::toV8() {
      v8::Local<v8::Object> t = v8::Object::New();

      t->Set(v8::String::New("id"), v8::String::New(id.c_str()));
      t->Set(v8::String::New("tid"), v8::String::New(tid.c_str()));
      t->Set(v8::String::New("archived"), v8::Boolean::New(archived));

      v8::Local<v8::Array> ary = v8::Array::New();
      std::vector<boost::shared_ptr<Message> >::iterator it, end = messages.end();
      uint32_t i;
      for(i = 0, it = messages.begin(); it != end; ++it, ++i) {
        ary->Set(i, (*it)->toV8());
      }

      t->Set(v8::String::New("messages"), ary);

      return t;
    }
  }
}

/* eof */
