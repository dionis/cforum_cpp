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

    Message::Author::Author() : name(), email(), username(), ip(), homepage() { }
    Message::Author::Author(const Author &usr) : name(usr.name), username(usr.username), ip(usr.ip), homepage(usr.homepage) { }

    Message::Author::Author(const mongo::BSONObj &author) : name(), email(), username(), ip(), homepage() {
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

    Message::Author &Message::Author::operator=(const Author &usr) {
      if(this != &usr) {
        name     = usr.name;
        email    = usr.email;
        username = usr.username;
        ip       = usr.ip;
        homepage = usr.homepage;
      }

      return *this;
    }

    v8::Local<v8::Object> Message::Author::toV8() {
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

    Message::Message() : Model::Model(), id(), category(), subject(), content(), author(), date(0), show(true), flags(), messages() { }
    Message::Message(const Message &msg) : Model::Model(msg), id(msg.id), category(msg.subject), subject(msg.subject), content(msg.content), author(msg.author), date(msg.date), show(msg.show), flags(msg.flags), messages(msg.messages) { }

    Message &Message::operator=(const Message &msg) {
      if(this != &msg) {
        id       = msg.id;
        category = msg.category;
        subject  = msg.subject;
        content  = msg.content;
        author   = msg.author;
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

      msg->id      = o.getField("id").String();
      msg->subject = o.getField("subject").String();
      msg->content = o.getField("content").String();
      msg->author  = Message::Author(author);
      msg->date    = (time_t)(o.getField("date").Date() / 1000);
      msg->show    = true;

      if(o.hasField("category")) {
        msg->category = o.getField("category").String();
      }

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

      msg->Set(v8::String::New("id"), v8::String::New(id.c_str()));
      msg->Set(v8::String::New("category"), v8::String::New(category.c_str()));
      msg->Set(v8::String::New("subject"), v8::String::New(subject.c_str()));
      msg->Set(v8::String::New("content"), v8::String::New(content.c_str()));
      msg->Set(v8::String::New("author"), author.toV8());
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
