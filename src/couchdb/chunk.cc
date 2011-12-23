/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CouchDB chunk interface implementation
 * \package couchdb
 *
 * This implements the CouchDB chunk interface
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

#include "couchdb/server.h"

namespace CForum {
  namespace CouchDB {
    Server::Chunk::Chunk() : _pos(0), _buff() {}
    Server::Chunk::Chunk(const std::string &buff) : _pos(0), _buff(buff) { }
    Server::Chunk::Chunk(const char *buff, size_t len) : _pos(0), _buff(buff, len) { }
    Server::Chunk::Chunk(const Server::Chunk &chk) : _pos(chk._pos), _buff(chk._buff) { }

    Server::Chunk &Server::Chunk::operator=(const Server::Chunk &chk) {
      if(this != &chk) {
        _pos  = chk._pos;
        _buff = chk._buff;
      }

      return *this;
    }


    void Server::Chunk::setPos(size_t pos) {
      _pos = pos;
    }

    size_t Server::Chunk::getPos() const {
      return _pos;
    }

    size_t Server::Chunk::getPos() {
      return _pos;
    }

    const std::string &Server::Chunk::getBuff() const {
      return _buff;
    }

    std::string &Server::Chunk::getBuff() {
      return _buff;
    }

    void Server::Chunk::setBuff(const std::string &buff) {
      _buff = buff;
    }
  }
}
