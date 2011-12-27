/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief URI parser interface testing
 * \package unittests
 *
 * Testing the URI parser interface
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

#include "uri_test.hh"

CPPUNIT_TEST_SUITE_REGISTRATION(URITest);

void URITest::testParser() {
  CForum::URI uri("http://wwwtech.de");
  CPPUNIT_ASSERT_EQUAL(std::string("http"), uri.getScheme());
  CPPUNIT_ASSERT_EQUAL(80, uri.getPort());
  CPPUNIT_ASSERT_EQUAL(std::string("/"), uri.getPath());
  CPPUNIT_ASSERT_EQUAL(std::string("wwwtech.de"), uri.getHost());
  CPPUNIT_ASSERT_EQUAL(std::string(""), uri.getFragment());
  CPPUNIT_ASSERT_EQUAL(std::string(""), uri.getQueryString());
  CPPUNIT_ASSERT_EQUAL(std::string("html"), uri.getMedia());
  CPPUNIT_ASSERT_EQUAL(std::string("http://wwwtech.de/"), uri.toString());

  CForum::URI uri1("https://wwwtech.de:8080#lala");
  CPPUNIT_ASSERT_EQUAL(std::string("https"), uri1.getScheme());
  CPPUNIT_ASSERT_EQUAL(8080, uri1.getPort());
  CPPUNIT_ASSERT_EQUAL(std::string("/"), uri1.getPath());
  CPPUNIT_ASSERT_EQUAL(std::string("wwwtech.de"), uri1.getHost());
  CPPUNIT_ASSERT_EQUAL(std::string("#lala"), uri1.getFragment());
  CPPUNIT_ASSERT_EQUAL(std::string(""), uri1.getQueryString());
  CPPUNIT_ASSERT_EQUAL(std::string("html"), uri1.getMedia());
  CPPUNIT_ASSERT_EQUAL(std::string("https://wwwtech.de:8080/#lala"), uri1.toString());

  CForum::URI uri2("https://wwwtech.de:8080/lala/lulu#lala");
  CPPUNIT_ASSERT_EQUAL(std::string("https"), uri2.getScheme());
  CPPUNIT_ASSERT_EQUAL(8080, uri2.getPort());
  CPPUNIT_ASSERT_EQUAL(std::string("/lala/lulu"), uri2.getPath());
  CPPUNIT_ASSERT_EQUAL(std::string("wwwtech.de"), uri2.getHost());
  CPPUNIT_ASSERT_EQUAL(std::string("#lala"), uri2.getFragment());
  CPPUNIT_ASSERT_EQUAL(std::string(""), uri2.getQueryString());
  CPPUNIT_ASSERT_EQUAL(std::string("html"), uri2.getMedia());
  CPPUNIT_ASSERT_EQUAL(std::string("https://wwwtech.de:8080/lala/lulu#lala"), uri2.toString());

  CForum::URI uri3("https://wwwtech.de:8080/lala/lulu.php?abc=def#lala");
  CPPUNIT_ASSERT_EQUAL(std::string("https"), uri3.getScheme());
  CPPUNIT_ASSERT_EQUAL(8080, uri3.getPort());
  CPPUNIT_ASSERT_EQUAL(std::string("/lala/lulu.php"), uri3.getPath());
  CPPUNIT_ASSERT_EQUAL(std::string("/lala/lulu"), uri3.getPathWoSuffix());
  CPPUNIT_ASSERT_EQUAL(std::string("wwwtech.de"), uri3.getHost());
  CPPUNIT_ASSERT_EQUAL(std::string("#lala"), uri3.getFragment());
  CPPUNIT_ASSERT_EQUAL(std::string("abc=def"), uri3.getQueryString());
  CPPUNIT_ASSERT_EQUAL(std::string("php"), uri3.getMedia());
  CPPUNIT_ASSERT_EQUAL(std::string("https://wwwtech.de:8080/lala/lulu.php?abc=def#lala"), uri3.toString());

}

void URITest::testParserGarbage() {
  try {
    CForum::URI uri("He’s the brains, sweetheart!");
    CPPUNIT_FAIL("Exception expected!");
  }
  catch(CForum::URIException &e) {
  }

  try {
    CForum::URI uri("http://:80");
    CPPUNIT_FAIL("Exception expected!");
  }
  catch(CForum::URIException &e) {
  }

  try {
    CForum::URI uri("http://wwwtech.de:abc");
    CPPUNIT_FAIL("Exception expected!");
  }
  catch(CForum::URIException &e) {
  }

}

/* eof */
