/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JS evaluator implementation
 * \package JSEvaluator
 *
 * This implements the JS evaluator
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

#include "jsevaluator/js_evaluator.hh"

namespace CForum {
  JSEvaluator::JSEvaluator() : _handle(), _context(v8::Context::New()), _context_scope(_context) {}

  JSEvaluator::JSEvaluator(const JSEvaluator &) : _handle(), _context(v8::Context::New()), _context_scope(_context) {}

  v8::Local<v8::Value> JSEvaluator::evaluateFile(const std::string &filename) {
    v8::Local<v8::Script> script(compileFile(filename));
    return evaluateScript(script);
  }

  v8::Local<v8::Value> JSEvaluator::evaluateString(const std::string &source) {
    v8::Local<v8::Script> script(compileString(source));
    return evaluateScript(script);
  }

  v8::Local<v8::Script> JSEvaluator::compileFile(const std::string &filename) {
    std::ifstream fd(filename.c_str(), std::ifstream::in);
    std::stringstream sst;

    if(!fd) {
      throw JSEvaluatorException("File not found!",CForumException::FileNotFound);
    }

    sst << fd.rdbuf();

    fd.close();

    return compileString(sst.str());
  }

  v8::Local<v8::Script> JSEvaluator::compileString(const std::string &source) {
    v8::Local<v8::String> src = v8::String::New(source.c_str());
    v8::Local<v8::Script> script = v8::Script::Compile(src);

    return script;
  }

  v8::Local<v8::Value> JSEvaluator::evaluateScript(const v8::Local<v8::Script> &script) {
    v8::TryCatch trycatch;
    v8::Local<v8::Value> result = script->Run();

    if(result.IsEmpty()) {
      v8::Local<v8::Value> exception = trycatch.Exception();
      v8::String::AsciiValue exception_str(exception);

      throw JSEvaluatorException(*exception_str,JSEvaluatorException::JSException);
    }

    return result;
  }

  JSEvaluator::~JSEvaluator() {
    _context.Dispose();
  }

}


/* eof */
