/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Session interface; you may store user specific values in a session
 * \package framework
 *
 * Session interface; you may store user specific values in a session
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

#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <sstream>
#include <sys/time.h>
#include <cstring>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include "hash_map.hh"

#include "session_exception.hh"
#include "framework/internal_error_exception.hh"

namespace CForum {
  class Session {
  public:
    class Value {
    public:
      virtual ~Value();

    private:
      friend class boost::serialization::access;
      template<class Archive> void serialize(Archive &, const unsigned int);
    };

    BOOST_SERIALIZATION_ASSUME_ABSTRACT(Session::Value)


    class Storage {
    public:
      virtual bool loadSession(const std::string &, std::unordered_map<std::string, boost::shared_ptr<Session::Value> > &) = 0;
      virtual bool saveSession(const std::string &, const std::unordered_map<std::string, boost::shared_ptr<Session::Value> > &) = 0;

      virtual bool create(const std::string &) = 0;
      virtual bool destroy(const std::string &) = 0;
      virtual bool exists(const std::string &) = 0;

      virtual ~Storage();
    };

    Session();
    Session(const std::string &, bool = false);
    Session(const boost::shared_ptr<Session::Storage> &);
    Session(const std::string &, const boost::shared_ptr<Session::Storage> &, bool = false);
    Session(const Session &);

    Session &operator=(const Session &);
    boost::shared_ptr<Session::Value> &operator[](const std::string &);

    void setExpiry(int);
    int getExpiry();

    void setSessionId(const std::string &);
    const std::string &getSessionId();

    void setSessionKey(const std::string &);
    const std::string &getSessionKey();

    boost::shared_ptr<Session::Storage> getStorage();
    void setStorage(boost::shared_ptr<Session::Storage>);

    bool save();
    bool load();
    bool destroy();

    ~Session();

  protected:
    int expires;
    boost::shared_ptr<Session::Storage> storage;
    std::string sessionId, sessionKey;
    std::unordered_map<std::string, boost::shared_ptr<Session::Value> > values;
    bool destroyed;

  };

  inline void Session::setExpiry(int expiry) {
    expires = expiry;
  }
  inline int Session::getExpiry() {
    return expires;
  }

  inline void Session::setSessionId(const std::string &id) {
    sessionId = id;
  }
  inline const std::string &Session::getSessionId() {
    return sessionId;
  }

  inline void Session::setSessionKey(const std::string &k) {
    sessionKey = k;
  }
  inline const std::string &Session::getSessionKey() {
    return sessionKey;
  }

  inline boost::shared_ptr<Session::Storage> Session::getStorage() {
    return storage;
  }
  inline void Session::setStorage(boost::shared_ptr<Session::Storage> stor) {
    storage = stor;
  }

  inline bool Session::destroy() {
    return destroyed = storage->destroy(sessionId);
  }


  template<class Archive> inline void Session::Value::serialize(Archive &, const unsigned int) { }

}

#include "framework/session_file_storage.hh"

#endif

/* eof */
