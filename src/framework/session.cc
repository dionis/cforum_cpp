/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief basic session interface
 * \package framework/session
 *
 * basic session interface
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

#include "framework/session.hh"

namespace CForum {
  Session::Session() : expires(1800),
                       path("/tmp/"),
                       sessionId(),
                       sessionKey("CFORUM_SID"),
                       values(),
                       storage(boost::make_shared<FileStorage>()) { }

  Session::Session(const std::string &id) : expires(1800),
                                   path("/tmp/"),
                                   sessionId(id),
                                   sessionKey("CFORUM_SID"),
                                   values(),
                                   storage(boost::make_shared<FileStorage>()) { }

  Session::Session(const boost::shared_ptr<Session::Storage> &stor) : expires(1800),
                                                                      path("/tmp/"),
                                                                      sessionId(),
                                                                      sessionKey("CFORUM_SID"),
                                                                      values(),
                                                                      storage(stor) { }

  Session::Session(const std::string &id, const boost::shared_ptr<Session::Storage> &stor) : expires(1800),
                                                                                             path("/tmp/"),
                                                                                             sessionId(id),
                                                                                             sessionKey("CFORUM_SID"),
                                                                                             values(),
                                                                                             storage(stor) { }
  Session::Session(const Session &sess) : expires(sess.expires),
                                          path(sess.path),
                                          sessionId(sess.sessionId),
                                          sessionKey(sess.sessionKey),
                                          values(sess.values),
                                          storage(sess.storage) { }

  Session &Session::operator=(const Session &s) {
    if(this != &s) {
      expires    = s.expires;
      path       = s.path;
      sessionId  = s.sessionId;
      sessionKey = s.sessionKey;
      values     = s.values;
      storage    = s.storage;
    }

    return *this;
  }

  boost::shared_ptr<Session::Value> &Session::operator[](const std::string &key) {
    return values[key];
  }

  bool Session::save() {
    return true;
  }

  bool Session::load() {
    return true;
  }

  Session::~Session() {
    save();
  }



}


/* eof */
