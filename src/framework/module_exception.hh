/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Module exception, all module related exceptions are derived from this one
 * \package framework
 *
 * Module exception, all module related exceptions are derived from this one
 */

/*
 * Copyright (C) 2011-2012 by Christian Kruse <cjk@wwwtech.de>
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

#ifndef MODULE_EXCEPTION_H
#define MODULE_EXCEPTION_H

#include "framework/framework_error_exception.hh"

namespace CForum {
  class ModuleException : public FrameworkErrorException {
  public:
    ModuleException();
    ModuleException(int);
    ModuleException(const char *, int);
    ModuleException(const std::string &, int);

    const static int ModuleNotFoundError      = 0x4f1c2cd6;
    const static int InitFunNotFoundError     = 0x4f1c2d86;
    const static int InitFunReturnedNullError = 0x4f1db307;

  };

}

#endif

/* eof */
