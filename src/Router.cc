/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Router implementation; routes request to the specific modules
 * \package JSEvaluator
 *
 * Routing is done via several criterias: of course via path, via desired
 * media, via POST/GET difference, etc, pp. Each module can specify a path
 * and one to N additional criterias. If all match, the module is called.
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

#include "Router.h"

namespace CForum {
  Router *Router::inst = NULL;

  Router::Router() {}
  Router::Router(const Router &router) {
    (void)router; // For compiler warnings only
  }

  Router &Router::operator=(const Router &r) {
    (void)r;
    return *this;
  }

  Router *Router::instance() {
    if(!inst) {
      inst = new Router();
    }

    return inst;
  }

  bool Router::registerRoute(const std::string &str) {
    return registerRoute(str.c_str());
  }

  bool Router::registerRoute(const UnicodeString &str) {
    std::string val;
    str.toUTF8String(val);

    return registerRoute(val.c_str());
  }

  bool Router::registerRoute(const char *str) {
    register const char *ptr;

    for(ptr=str;*ptr;++ptr) {
    }

    return true;
  }

}


/* eof */
