/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Configuration error interface definition
 * \package Exceptions
 *
 * This defines the configuration error exception
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

#ifndef CONFIGERROR_EXCEPTION_H
#define CONFIGERROR_EXCEPTION_H

#include "framework/framework_error_exception.hh"

namespace CForum {
  class ConfigErrorException : public FrameworkErrorException {
  public:
    ConfigErrorException();
    ConfigErrorException(int);
    ConfigErrorException(const char *,int);
    ConfigErrorException(const std::string &,int);

    static const int EmptyConfigError      = 0x4f11aa6e;
    static const int JSError               = 0x4f11c6b3;
    static const int StringConversionError = 0x4f11ddd8;
    static const int NotAnObjectError      = 0x4f11e9a9;
    static const int NotExistantOrNull     = 0x4fd0d47c;
  };
}

#endif

/* eof */
