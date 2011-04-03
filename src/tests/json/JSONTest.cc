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
  const char *str = "{\"test-array\":[\"test-value\",3],\"test-bool-false\":false,\"test-bool-true\":true,\"test-null\":null,\"test-num-float\":23.23,\"test-num-int\":23,\"test-string\":\"test-value\"}";

  pr->parse(str,(CForum::JSON::Element **)&root);

  std::map<UnicodeString,CForum::JSON::Element *> &mp = root->getValue();

  CForum::JSON::Array *ary = (CForum::JSON::Array *)mp[UnicodeString("test-array")];
  std::vector <CForum::JSON::Element *> &vc = ary->getValue();

  CPPUNIT_ASSERT_EQUAL((int)vc.size(),2);
  CPPUNIT_ASSERT_EQUAL(vc[0]->getType(),CForum::JSON::JSONTypeString);

  std::string str_val,str_val1("test-value");
  ((CForum::JSON::String *)vc[0])->getValue().toUTF8String(str_val);

  CPPUNIT_ASSERT_EQUAL(str_val,str_val1);

  CPPUNIT_ASSERT_EQUAL(vc[1]->getType(),CForum::JSON::JSONTypeNumber);
  CPPUNIT_ASSERT_EQUAL((int)((CForum::JSON::Number *)vc[1])->getNumberType(),(int)CForum::JSON::JSONNumberTypeInt);
  CPPUNIT_ASSERT_EQUAL((int)((CForum::JSON::Number *)vc[1])->getIValue(),3);

  CForum::JSON::Boolean *b = (CForum::JSON::Boolean *)mp[UnicodeString("test-bool-false")];
  CPPUNIT_ASSERT(!b->getValue());

  b = (CForum::JSON::Boolean *)mp[UnicodeString("test-bool-true")];
  CPPUNIT_ASSERT(b->getValue());

  CForum::JSON::Number *n = (CForum::JSON::Number *)mp[UnicodeString("test-num-float")];
  CPPUNIT_ASSERT_EQUAL(n->getDValue(),23.23);

  n = (CForum::JSON::Number *)mp[UnicodeString("test-num-int")];
  CPPUNIT_ASSERT_EQUAL((int)n->getIValue(),23);

  CForum::JSON::String *xstr = (CForum::JSON::String *)mp[UnicodeString("test-string")];
  str_val = "";
  xstr->getValue().toUTF8String(str_val);
  str_val1 = "test-value";

  CPPUNIT_ASSERT_EQUAL(str_val1,str_val);

  delete root;
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
  dt[UnicodeString("test-num-int")] = new CForum::JSON::Number((int64_t)23);
  dt[UnicodeString("test-num-float")] = new CForum::JSON::Number((double)23.23);
  dt[UnicodeString("test-bool-true")] = new CForum::JSON::Boolean(true);
  dt[UnicodeString("test-bool-false")] = new CForum::JSON::Boolean(false);
  dt[UnicodeString("test-null")] = new CForum::JSON::Null();
  dt[UnicodeString("test-array")] = ary;

  test = root->toJSON();

  delete root;

  CPPUNIT_ASSERT_EQUAL(test,std::string("{\"test-array\":[\"test-value\",3],\"test-bool-false\":false,\"test-bool-true\":true,\"test-null\":null,\"test-num-float\":23.23,\"test-num-int\":23,\"test-string\":\"test-value\"}"));
}



/* eof */
