/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief JSON parser interface testing
 * \package unittests
 *
 * Testing the JSON parser interface
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

#include "JSONTest.h"
#include "json/JSONParser.h"

CPPUNIT_TEST_SUITE_REGISTRATION(JSONTest);

void JSONTest::testParser() {
  CForum::JSON::Parser *pr = new CForum::JSON::Parser();
  CForum::JSON::Object *root;
  const char *str = "{\"test-array\":[\"test-value\",3],\"test-bool-false\":false,\"test-bool-true\":true,\"test-null\":null,\"test-num-float\":10.3,\"test-num-int\":10,\"test-string\":\"test-value\"}";

  try {
    pr->parse(str,(CForum::JSON::Element **)&root);
  }
  catch(CForum::JSON::JSONException &e) {
    printf("error: %s\n",e.getMessage().c_str());
    throw e;
  }



}

void JSONTest::testGenerator() {
  CForum::JSON::Object *root = new CForum::JSON::Object();
  std::map<UnicodeString,CForum::JSON::Element *> &dt = root->getValue();
  UnicodeString str("test-value");
  std::string test;

  CForum::JSON::Array *ary = new CForum::JSON::Array();
  std::vector<CForum::JSON::Element *> &ary_val = ary->getValue();

  ary_val.push_back((CForum::JSON::Element *)new CForum::JSON::String(str));
  ary_val.push_back((CForum::JSON::Element *)new CForum::JSON::Number((int64_t)3));


  dt[UnicodeString("test-string")] = new CForum::JSON::String(str);
  dt[UnicodeString("test-num-int")] = new CForum::JSON::Number((int64_t)10);
  dt[UnicodeString("test-num-float")] = new CForum::JSON::Number((double)10.3);
  dt[UnicodeString("test-bool-true")] = new CForum::JSON::Boolean(true);
  dt[UnicodeString("test-bool-false")] = new CForum::JSON::Boolean(false);
  dt[UnicodeString("test-null")] = new CForum::JSON::Null();
  dt[UnicodeString("test-array")] = ary;

  test = root->toJSON();

  delete root;

  CPPUNIT_ASSERT_EQUAL(test,std::string("{\"test-array\":[\"test-value\",3],\"test-bool-false\":false,\"test-bool-true\":true,\"test-null\":null,\"test-num-float\":10.3,\"test-num-int\":10,\"test-string\":\"test-value\"}"));
}



/* eof */
