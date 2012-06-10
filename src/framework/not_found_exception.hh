/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief when a ressource could not be found this exception gets throwed
 * \package framework
 *
 * when a ressource could not be found this exception gets throwed
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

#ifndef NOT_FOUND_EXCEPTION_H
#define NOT_FOUND_EXCEPTION_H

#include "framework/framework_error_exception.hh"

namespace CForum {
  class NotFoundException : public FrameworkErrorException {
  public:
    NotFoundException();
    NotFoundException(int);
    NotFoundException(const char *, int);
    NotFoundException(const std::string &, int);

    static const int NoRouteMatchedError = 0x4efb11e9;
    static const int ThreadNotFoundError = 0x4fd3925a;
    static const int MessageNotFoundError = 0x4fd46582;

  };

}

#endif

/* eof */
