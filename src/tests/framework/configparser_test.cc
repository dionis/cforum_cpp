/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Config parser interface testing
 * \package unittests
 *
 * Config parser interface testing
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

#include "configparser_test.hh"

CPPUNIT_TEST_SUITE_REGISTRATION(ConfigParserTest);

using namespace CForum;


void ConfigParserTest::setUp() {
  setenv("CF_CFORUM_JS", "../../../src/tests/framework/cforum.js", 1);
  configParser.parse();
}

void ConfigParserTest::testGetValue() {
  v8::Handle<v8::Value> v = configParser.getValue("yabba");

  v8::String::Utf8Value utf8str(v);

  CPPUNIT_ASSERT_EQUAL(std::string("May the force be with you"), std::string(*utf8str));
}

void ConfigParserTest::testGetObject() {
  v8::Handle<v8::Value> v = configParser.getValue("ano");

  CPPUNIT_ASSERT(v->IsObject());

  v8::Handle<v8::Object> o = v->ToObject();

  v8::Handle<v8::String> nam = v8::String::New("nymous");
  v8::Handle<v8::Value> s = o->GetRealNamedProperty(nam);
  CPPUNIT_ASSERT(s->IsArray());
}

void ConfigParserTest::testGetString() {
  std::string v = configParser.getStrValue("yabba");
  CPPUNIT_ASSERT_EQUAL(std::string("May the force be with you"), v);
}

void ConfigParserTest::testGetByPath() {
  v8::Handle<v8::Value> val = configParser.getByPath("/ano/xyz");
  v8::String::Utf8Value sval(val);
  CPPUNIT_ASSERT_EQUAL(std::string(*sval), std::string("zyx"));

  val = configParser.getByPath("/ano/nymous/0");
  v8::String::Utf8Value sval1(val);
  CPPUNIT_ASSERT_EQUAL(std::string(*sval1), std::string("Ano"));
}

/* eof */
