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
      name = author.getField("name").String();

      if(author.hasField("email")) {
        email = author.getField("email").String();
      }

      if(author.hasField("username")) {
        username = author.getField("username").String();
      }

      if(author.hasField("ip")) {
        ip = author.getField("ip").String();
      }

      if(author.hasField("homepage")) {
        homepage = author.getField("homepage").String();
      }
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

    v8::Local<v8::Object> Message::User::toV8() {
      v8::Local<v8::Object> u = v8::Object::New();
      u->Set(v8::String::New("name"), v8::String::New(name.c_str()));

      if(email != "") {
        u->Set(v8::String::New("email"), v8::String::New(email.c_str()));
      }

      if(username != "") {
        u->Set(v8::String::New("username"), v8::String::New(username.c_str()));
      }

      if(ip != "") {
        u->Set(v8::String::New("ip"), v8::String::New(ip.c_str()));
      }

      if(homepage != "") {
        u->Set(v8::String::New("homepage"), v8::String::New(homepage.c_str()));
      }

      return u;
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
      const std::vector<mongo::BSONElement> msgs = o.getField("messages").Array();

      msg->subject = o.getField("subject").String();
      msg->content = o.getField("content").String();
      msg->user    = Message::User(author);
      msg->date    = (time_t)o.getField("date").Date();
      msg->show    = true;

      if(o.hasField("flags")) {
        mongo::BSONObj flags = o.getField("flags").Obj();
        std::set<std::string> fields;

        flags.getFieldNames(fields);

        std::set<std::string>::const_iterator it, end = fields.end();

        for(it = fields.begin(); it != end; ++it) {
          Flag f;
          f.name = *it;
          f.value = flags.getField(*it).String();

          msg->flags.push_back(f);
        }
      }

      if(msgs.size() > 0) {
        std::vector<mongo::BSONElement>::const_iterator it, end = msgs.end();
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

    v8::Local<v8::Object> Message::toV8() {
      v8::Local<v8::Object> msg = v8::Object::New();

      msg->Set(v8::String::New("subject"), v8::String::New(subject.c_str()));
      msg->Set(v8::String::New("content"), v8::String::New(content.c_str()));
      msg->Set(v8::String::New("user"), user.toV8());
      msg->Set(v8::String::New("date"), v8::Date::New((uint64_t)date * 1000));
      msg->Set(v8::String::New("show"), v8::Boolean::New(show));

      v8::Local<v8::Object> v8flags = v8::Object::New();
      std::vector<Message::Flag>::iterator it, end = flags.end();
      for(it = flags.begin(); it != end; ++it) {
        v8flags->Set(v8::String::New(it->name.c_str()), v8::String::New(it->value.c_str()));
      }
      msg->Set(v8::String::New("flags"), v8flags);

      v8::Local<v8::Array> v8msgs = v8::Array::New(messages.size());
      std::vector<boost::shared_ptr<Message> >::iterator mit, mend = messages.end();
      uint32_t i;
      for(i = 0, mit = messages.begin(); mit != mend; ++mit, ++i) {
        v8msgs->Set(i, (*mit)->toV8());
      }
      msg->Set(v8::String::New("messages"), v8msgs);

      return msg;
    }
  }
}


/* eof */
