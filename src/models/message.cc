/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CForum message model implementation
 *
 * CForum message model implementation
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

#include "models/message.hh"

namespace CForum {
  namespace Models {

    Message::User::User() : name(), email(), username(), ip(), homepage() { }
    Message::User::User(const User &usr) : name(usr.name), username(usr.username), ip(usr.ip), homepage(usr.homepage) { }

    Message::User::User(const mongo::BSONObj &author) : name(), email(), username(), ip(), homepage() {
      name     = author.getField("name").String();
      email    = author.getField("email").String();
      username = author.getField("username").String();
      ip       = author.getField("ip").String();
      homepage = author.getField("homepage").String();
    }

    Message::User &Message::User::operator=(const User &usr) {
      if(this != &usr) {
        name     = usr.name;
        email    = usr.email;
        username = usr.username;
        ip       = usr.ip;
        homepage = usr.homepage;
      }

      return *this;
    }


    Message::Flag::Flag() : name(), value() { }
    Message::Flag::Flag(const Flag &flag) : name(flag.name), value(flag.value) { }
    Message::Flag &Message::Flag::operator=(const Flag &flag) {
      if(this != &flag) {
        name  = flag.name;
        value = flag.value;
      }

      return *this;
    }

    Message::Message() : Model::Model(), subject(), content(), user(), date(0), show(true), flags(), messages() { }
    Message::Message(const Message &msg) : Model::Model(msg), subject(msg.subject), content(msg.content), user(msg.user), date(msg.date), show(msg.show), flags(msg.flags), messages(msg.messages) { }

    Message &Message::operator=(const Message &msg) {
      if(this != &msg) {
        subject  = msg.subject;
        content  = msg.content;
        user     = msg.user;
        date     = msg.date;
        show     = msg.show;
        flags    = msg.flags;
        messages = msg.messages;
      }

      return *this;
    }


    boost::shared_ptr<Message> Message::fromJSON(const JSON::Object &) { /* TODO: implement! */
      return boost::make_shared<Message>();
    }

    boost::shared_ptr<Message> Message::fromBSON(const mongo::BSONObj &o) {
      boost::shared_ptr<Message> msg = boost::make_shared<Message>();

      const mongo::BSONObj author = o.getField("author").Obj();
      const std::vector<mongo::BSONElement> flags = o.getField("flags").Array();
      const std::vector<mongo::BSONElement> msgs = o.getField("messages").Array();

      msg->subject = o.getField("subject").String();
      msg->content = o.getField("content").String();
      msg->user    = Message::User(author);
      msg->date    = (time_t)o.getField("date").Date();
      msg->show    = true;

      msg->flags   = std::vector<Message::Flag>();
      std::vector<mongo::BSONElement>::const_iterator it, end = flags.end();
      for(it = flags.begin(); it != end; ++it) {
        msg->flags.push_back(Message::Flag(it->Obj()));
      }

      msg->messages = std::vector<boost::shared_ptr<Message> >();
      if(msgs.size() > 0) {
        end = msgs.end();
        for(it = msgs.begin(); it != end; ++it) {
          msg->messages.push_back(Message::fromBSON(it->Obj()));
        }
      }

      return msg;
    }

    boost::shared_ptr<JSON::Element> Message::toJSON() {
      return boost::shared_ptr<JSON::Object>();
    }

    boost::shared_ptr<mongo::BSONObj> Message::toBSON() {
      return boost::shared_ptr<mongo::BSONObj>();
    }
  }
}


/* eof */
