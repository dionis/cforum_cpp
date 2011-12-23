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

CPPUNIT_TEST_SUITE_REGISTRATION(JSONTest);

void JSONTest::testParser() {
  CForum::JSON::Parser *pr = new CForum::JSON::Parser();
  boost::shared_ptr<CForum::JSON::Element> e_root;
  const char *str = "{\"test-array\":[\"test-value\",3],\"test-bool-false\":false,\"test-bool-true\":true,\"test-null\":null,\"test-num-float\":23.23,\"test-num-int\":23,\"test-string\":\"test-value\"}";

  pr->parse(str,e_root);
  boost::shared_ptr<CForum::JSON::Object> root = boost::dynamic_pointer_cast<CForum::JSON::Object>(e_root);

  if(!root) {
    CPPUNIT_FAIL("root is NULL");
  }

  CForum::JSON::Object::ObjectType_t &mp = root->getValue();

  boost::shared_ptr<CForum::JSON::Array> ary = boost::dynamic_pointer_cast<CForum::JSON::Array>(mp[UnicodeString("test-array")]);
  CForum::JSON::Array::ArrayType_t &vc = ary->getValue();

  CPPUNIT_ASSERT_EQUAL((int)vc.size(),2);
  CPPUNIT_ASSERT(boost::dynamic_pointer_cast<CForum::JSON::String>(vc[0]));

  std::string str_val,str_val1("test-value");
  boost::dynamic_pointer_cast<CForum::JSON::String>(vc[0])->getValue().toUTF8String(str_val);

  CPPUNIT_ASSERT_EQUAL(str_val,str_val1);

  CPPUNIT_ASSERT(boost::dynamic_pointer_cast<CForum::JSON::Number>(vc[1]));
  CPPUNIT_ASSERT_EQUAL(
    (int)boost::dynamic_pointer_cast<CForum::JSON::Number>(vc[1])->getNumberType(),
    (int)CForum::JSON::JSONNumberTypeInt
  );

  CPPUNIT_ASSERT_EQUAL(3,(int)boost::dynamic_pointer_cast<CForum::JSON::Number>(vc[1])->getIValue());

  boost::shared_ptr<CForum::JSON::Boolean> b = boost::dynamic_pointer_cast<CForum::JSON::Boolean>(mp[UnicodeString("test-bool-false")]);
  CPPUNIT_ASSERT(!b->getValue());

  b = boost::dynamic_pointer_cast<CForum::JSON::Boolean>(mp[UnicodeString("test-bool-true")]);
  CPPUNIT_ASSERT(b->getValue());

  boost::shared_ptr<CForum::JSON::Number> n = boost::dynamic_pointer_cast<CForum::JSON::Number>(mp[UnicodeString("test-num-float")]);
  CPPUNIT_ASSERT_EQUAL(23.23,n->getDValue());

  n = boost::dynamic_pointer_cast<CForum::JSON::Number>(mp[UnicodeString("test-num-int")]);
  CPPUNIT_ASSERT_EQUAL(23,(int)n->getIValue());

  boost::shared_ptr<CForum::JSON::String> xstr = boost::dynamic_pointer_cast<CForum::JSON::String>(mp[UnicodeString("test-string")]);
  str_val = "";
  xstr->getValue().toUTF8String(str_val);
  str_val1 = "test-value";

  CPPUNIT_ASSERT_EQUAL(str_val1,str_val);

  delete pr;
}

void JSONTest::testGenerator() {
  boost::shared_ptr<CForum::JSON::Object> root(new CForum::JSON::Object());
  CForum::JSON::Object::ObjectType_t &dt = root->getValue();
  UnicodeString str("test-value");
  std::string test;

  boost::shared_ptr<CForum::JSON::Array> ary(new CForum::JSON::Array());
  CForum::JSON::Array::ArrayType_t &ary_val = ary->getValue();

  ary_val.push_back(boost::shared_ptr<CForum::JSON::Element>(new CForum::JSON::String(str)));
  ary_val.push_back(boost::shared_ptr<CForum::JSON::Element>(new CForum::JSON::Number((int64_t)3)));


  dt[UnicodeString("test-string")] = boost::shared_ptr<CForum::JSON::Element>(new CForum::JSON::String(str));
  dt[UnicodeString("test-num-int")] = boost::shared_ptr<CForum::JSON::Element>(new CForum::JSON::Number((int64_t)23));
  dt[UnicodeString("test-num-float")] = boost::shared_ptr<CForum::JSON::Element>(new CForum::JSON::Number((double)23.23));
  dt[UnicodeString("test-bool-true")] = boost::shared_ptr<CForum::JSON::Element>(new CForum::JSON::Boolean(true));
  dt[UnicodeString("test-bool-false")] = boost::shared_ptr<CForum::JSON::Element>(new CForum::JSON::Boolean(false));
  dt[UnicodeString("test-null")] = boost::shared_ptr<CForum::JSON::Element>(new CForum::JSON::Null());
  dt[UnicodeString("test-array")] = boost::dynamic_pointer_cast<CForum::JSON::Element>(ary);

  test = root->toJSON();

  // TODO: since objects are unordered (dito for hash tables) comparing the JSON
  // string doesn't make sense. We compare the length for now, but we have to check better
  // if possible
  CPPUNIT_ASSERT_EQUAL(
    std::string("{\"test-array\":[\"test-value\",3],\"test-bool-false\":false,\"test-bool-true\":true,\"test-null\":null,\"test-num-float\":23.23,\"test-num-int\":23,\"test-string\":\"test-value\"}").size(),
    test.size()
  );
}



/* eof */
