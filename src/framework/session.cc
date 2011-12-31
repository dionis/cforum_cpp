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
  inline std::string generateSessionId(boost::shared_ptr<Session::Storage> stor) {
    std::ostringstream sids;
    std::string sid;

    int i, l, maxlen = 30;
    char *remaddr = getenv("REMOTE_ADDR");
    static const char chars[] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789_-";
    struct timeval  tp;

    if(!remaddr) {
      remaddr = (char *)"127.0.0.1";
    }

    if(gettimeofday(&tp,NULL) != 0) {
      throw InternalErrorException("ERROR getting time!", InternalErrorException::CouldNotGetTimeError);
    }

    srand(tp.tv_usec);

    do {
      l = strlen(remaddr);

      if(maxlen <= l) {
        l = maxlen;
      }

      for(i = 0; i < l; ++i) {
        sids << chars[remaddr[i] % 59 + (unsigned int)(rand() % 3)];
      }

      l = rand() % (maxlen - 1);
      for(; i < l; ++i) {
        sids << chars[rand() % 63];
      }

      sid = sids.str();
      sids.str("");
    } while(!stor->create(sid));

    return sid;
  }

  Session::Session() : expires(1800),
                       storage(boost::make_shared<FileStorage>()),
                       sessionId(generateSessionId(storage)),
                       sessionKey("CFORUM_SID"),
                       values(),
                       destroyed(false) { }

  Session::Session(const std::string &id, bool only_new) : expires(1800),
                                            storage(boost::make_shared<FileStorage>()),
                                            sessionId(id),
                                            sessionKey("CFORUM_SID"),
                                            values(),
                                            destroyed(false)
  {
    if(only_new) {
      if(storage->exists(sessionId)) {
        throw SessionException("Session already exists!", 0);
      }

      storage->create(sessionId);
    }
  }

  Session::Session(const boost::shared_ptr<Session::Storage> &stor) : expires(1800),
                                                                      storage(stor),
                                                                      sessionId(generateSessionId(storage)),
                                                                      sessionKey("CFORUM_SID"),
                                                                      values(),
                                                                      destroyed(false) { }

  Session::Session(const std::string &id, const boost::shared_ptr<Session::Storage> &stor, bool only_new) : expires(1800),
                                                                                                            storage(stor),
                                                                                                            sessionId(id),
                                                                                                            sessionKey("CFORUM_SID"),
                                                                                                            values(),
                                                                                                            destroyed(false)
  {
    if(only_new) {
      if(storage->exists(sessionId)) {
        throw SessionException("Session already exists!", 0);
      }

      storage->create(sessionId);
    }
  }
  Session::Session(const Session &sess) : expires(sess.expires),
                                          storage(sess.storage),
                                          sessionId(sess.sessionId),
                                          sessionKey(sess.sessionKey),
                                          values(sess.values),
                                          destroyed(false) { }

  Session &Session::operator=(const Session &s) {
    if(this != &s) {
      expires    = s.expires;
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
    return storage->saveSession(sessionId, values);
    return true;
  }

  bool Session::load() {
    return storage->loadSession(sessionId, values);
  }

  Session::~Session() {
    if(!destroyed) {
      save();
    }
  }



}


/* eof */
