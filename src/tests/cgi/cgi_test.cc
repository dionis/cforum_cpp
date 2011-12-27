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

#include "cgi_test.hh"

CPPUNIT_TEST_SUITE_REGISTRATION(CGITest);

void CGITest::testParser() {
  setenv("REQUEST_METHOD","GET",1);
  // äöü=äöü&a=b&a=b1&a=b2
  setenv("QUERY_STRING","%C3%A4%C3%B6%C3%BC=%C3%A4%C3%B6%C3%BC&a=b&a=b1&a=b2",1);
  setenv("HTTP_COOKIE","test-cookie=tc1; test-cookie2=tc2",1);
  setenv("SERVER_NAME","CForum 1.0 UnitTest",1);

  setenv("HTTP_USER_AGENT","CForum/" CF_VERSION,1);

  CForum::CGI c = CForum::CGI::fromCGIEnvironment();

  CForum::CGI::ArgumentListType p = c.getValue("äöü","G");
  CPPUNIT_ASSERT(p);
  CPPUNIT_ASSERT_EQUAL(1,(int)p->size());

  UnicodeString val = p->at(0);
  std::string str;
  val.toUTF8String(str);
  CPPUNIT_ASSERT_EQUAL(std::string("äöü"),str);

  p = c.getValue("a","G");
  CPPUNIT_ASSERT(p);
  CPPUNIT_ASSERT_EQUAL(3,(int)p->size());

  val = p->at(0);
  str = "";
  val.toUTF8String(str);
  CPPUNIT_ASSERT_EQUAL(std::string("b"),str);

  val = p->at(1);
  str = "";
  val.toUTF8String(str);
  CPPUNIT_ASSERT_EQUAL(std::string("b1"),str);

  val = p->at(2);
  str = "";
  val.toUTF8String(str);
  CPPUNIT_ASSERT_EQUAL(std::string("b2"),str);


  p = c.getValue("test-cookie","C");
  CPPUNIT_ASSERT(p != NULL);
  CPPUNIT_ASSERT_EQUAL(1,(int)p->size());

  val = p->at(0);
  str = "";
  val.toUTF8String(str);
  CPPUNIT_ASSERT_EQUAL(std::string("tc1"),str);

  p = c.getValue("test-cookie2","C");
  CPPUNIT_ASSERT(p);
  CPPUNIT_ASSERT_EQUAL(1,(int)p->size());

  val = p->at(0);
  str = "";
  val.toUTF8String(str);
  CPPUNIT_ASSERT_EQUAL(std::string("tc2"),str);

  CPPUNIT_ASSERT_EQUAL(std::string("CForum 1.0 UnitTest"),c.serverName());

  CPPUNIT_ASSERT_EQUAL(std::string("CForum/" CF_VERSION), c.getHeader("User-Agent"));

}



/* eof */
