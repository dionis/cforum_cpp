/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CForum message model
 *
 * CForum message model
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

#ifndef MODELS_MESSAGE_H
#define MODELS_MESSAGE_H

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "json/json_parser.hh"
#include "framework/model.hh"

namespace CForum {
  namespace Models {
    class Message : public Model {
    public:
      class User {
      public:
        User();
        User(const User &);
        User(const mongo::BSONObj &);

        User &operator=(const User &);

        std::string name, email, username, ip, homepage;
      };

      class Flag {
      public:
        Flag();
        Flag(const Flag &);
        Flag(const mongo::BSONObj &);

        Flag &operator=(const Flag &);

        std::string name, value;
      };

      Message();
      Message(const Message &);

      virtual Message &operator=(const Message &);

      static boost::shared_ptr<Message> fromJSON(const JSON::Object &);
      static boost::shared_ptr<Message> fromBSON(const mongo::BSONObj &);

      virtual boost::shared_ptr<JSON::Element> toJSON();
      virtual boost::shared_ptr<mongo::BSONObj> toBSON();

      std::string subject, content;

      Message::User user;

      time_t date;
      bool show;

      std::vector<Message::Flag> flags;
      std::vector<boost::shared_ptr<Message> > messages;
    };
  }
}

#endif

/* eof */
