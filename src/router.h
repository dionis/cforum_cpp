/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Router interface; routes request to the right modules
 * \package framework
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

#ifndef ROUTER_H
#define ROUTER_H

#include <cstdlib>
#include <string>
#include <vector>

#include <unicode/unistr.h>

namespace CForum {
  class Router {
  public:
    Router();
    Router(const Router &);
    Router &operator=(const Router &);

    bool registerRoute(const std::string &);
    bool registerRoute(const char *);
    bool registerRoute(const UnicodeString &);


  };
}


#endif

/* eof */
