/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief MongoDB interface enhancements
 * \package framework
 *
 * MongoDB interface enhancements
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


#include "framework/mongodb.hh"

namespace CForum {
  DBClientConnection::DBClientConnection(bool _autoReconnect, mongo::DBClientReplicaSet *cp, double so_timeout) : mongo::DBClientConnection(_autoReconnect, cp, so_timeout) { }
  void DBClientConnection::setDbName(const std::string &db) {
    dbname = db;
  }

  std::auto_ptr<mongo::DBClientCursor> DBClientConnection::query(const std::string &ns, mongo::Query query, int nToReturn, int nToSkip, const mongo::BSONObj *fieldsToReturn, int queryOptions, int batchSize) {
    std::string ns_w_db = dbname + "." + ns;
    return mongo::DBClientConnection::query(ns_w_db, query, nToReturn, nToSkip, fieldsToReturn, queryOptions, batchSize);
  }
}

/* eof */
