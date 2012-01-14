/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JS evaluator interface
 * \package JSON
 *
 * This defines the JS evaluator interface
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

#ifndef JS_EVALUATOR_H
#define JS_EVALUATOR_H

#include <v8.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>

#include "jsevaluator/js_evaluator_exception.hh"

namespace CForum {
  class JSEvaluator {
  public:
    JSEvaluator();

    v8::Handle<v8::Value> evaluateFile(const std::string &);
    v8::Handle<v8::Value> evaluateString(const std::string &);

    v8::Handle<v8::Script> compileString(const std::string &);
    v8::Handle<v8::Script> compileFile(const std::string &);

    v8::Handle<v8::Value> evaluateScript(const v8::Handle<v8::Script> &);

  private:
    JSEvaluator(const JSEvaluator &);

    v8::HandleScope _handle;
    v8::Persistent<v8::Context> _context;
    v8::Context::Scope _context_scope;

  };
}


#endif

/* eof */
