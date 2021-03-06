/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief File storage interface
 * \package framework/session
 *
 * File storage interface
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

#ifndef SESSION_FILE_STORAGE_H
#define SESSION_FILE_STORAGE_H

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "framework/session.hh"

namespace CForum {
  class FileStorage : public Session::Storage {
  public:
    FileStorage();
    FileStorage(const FileStorage &);

    virtual bool create(const std::string &);
    virtual bool destroy(const std::string &);
    virtual bool exists(const std::string &);

    virtual bool loadSession(const std::string &, std::unordered_map<std::string, boost::shared_ptr<Session::Value> > &);
    virtual bool saveSession(const std::string &, const std::unordered_map<std::string, boost::shared_ptr<Session::Value> > &);
    virtual ~FileStorage();

    const std::string &getSessionPath() const;
    void setSessionPath(const std::string &);

    const std::string &getPrefix() const;
    void setPrefix(const std::string &);

  private:
    std::string sessionPath, prefix;

    std::string getFilename(const std::string &);

  };

  inline const std::string &FileStorage::getSessionPath() const {
    return sessionPath;
  }
  inline void FileStorage::setSessionPath(const std::string &path) {
    sessionPath = path;
  }

  inline const std::string &FileStorage::getPrefix() const {
    return prefix;
  }
  inline void FileStorage::setPrefix(const std::string &pref) {
    prefix = pref;
  }

  inline std::string FileStorage::getFilename(const std::string &sid) {
    return sessionPath + "/" + (prefix != "" ? prefix : std::string()) + sid;
  }
}


#endif

/* eof */
