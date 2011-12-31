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


#include "framework/session_file_storage.hh"

namespace CForum {
  FileStorage::FileStorage() : sessionPath("/tmp"), prefix("sess_") { }
  FileStorage::FileStorage(const FileStorage &fs) : sessionPath(fs.sessionPath), prefix(fs.prefix) { }

  bool FileStorage::create(const std::string &sid) {
    std::string fname = getFilename(sid);

    int fd = open(fname.c_str(), O_CREAT|O_EXCL, S_IRUSR|S_IWUSR);

    if(fd == -1) {
      return false;
    }

    close(fd);
    return true;
  }

  bool FileStorage::destroy(const std::string &sid) {
    std::string fname = getFilename(sid);
    return unlink(fname.c_str()) == 0;
  }

  bool FileStorage::loadSession(const std::string &sid, std::unordered_map<std::string, boost::shared_ptr<Session::Value> > &values) {
    std::string fname = getFilename(sid);
    std::ifstream fd(fname.c_str(), std::ifstream::in);
    boost::archive::text_iarchive ar(fd);

    ar & values;

    fd.close();

    return true;
  }

  bool FileStorage::exists(const std::string &sid) {
    std::string fname = getFilename(sid);
    struct stat st;

    if(stat(fname.c_str(), &st) == 0) {
      return true;
    }

    return false;
  }

  bool FileStorage::saveSession(const std::string &sid, const std::unordered_map<std::string, boost::shared_ptr<Session::Value> > &vals) {
    std::string fname = getFilename(sid);
    std::ofstream fd(fname.c_str(), std::ios::trunc | std::ios::out);
    boost::archive::text_oarchive ar(fd);

    ar & vals;

    fd.close();

    return true;
  }

  FileStorage::~FileStorage() { }
}

/* eof */
