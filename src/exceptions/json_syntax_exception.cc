/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSON syntax error exception
 * \package JSON
 *
 * This implements the JSON syntax error exception interface
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


#include "json_syntax_exception.h"

namespace CForum {
  namespace JSON {
    JSONSyntaxErrorException::JSONSyntaxErrorException() : JSONException() {}
    JSONSyntaxErrorException::JSONSyntaxErrorException(int code) : JSONException(code) {}
    JSONSyntaxErrorException::JSONSyntaxErrorException(const char *msg,int code) : JSONException(msg,code) {}
    JSONSyntaxErrorException::JSONSyntaxErrorException(const std::string &msg,int code) : JSONException(msg,code) {}
  }
}

/* eof */
