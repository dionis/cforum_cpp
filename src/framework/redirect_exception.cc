/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Framework exception, all framework errors throw a child exception
 * \package framework
 *
 * Framework exception, all framework errors throw a child exception
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


#include "framework/redirect_exception.hh"

namespace CForum {
  RedirectException::RedirectException(const std::string &uri, int stat) : FrameworkException(), status(stat), url(uri) { }
  RedirectException::RedirectException(int code, const std::string &uri, int stat) : FrameworkException(code), status(stat), url(uri) { }
  RedirectException::RedirectException(const char *msg, int code, const std::string &uri, int stat) : FrameworkException(msg, code), status(stat), url(uri) { }
  RedirectException::RedirectException(const std::string &msg, int code, const std::string &uri, int stat) : FrameworkException(msg, code), status(stat), url(uri) { }

  RedirectException::~RedirectException() throw() { }
}


/* eof */
