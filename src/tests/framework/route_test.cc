/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Route interface testing
 * \package unittests
 *
 * Route interface testing
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

#include "route_test.hh"

CPPUNIT_TEST_SUITE_REGISTRATION(RouteTest);

using namespace CForum;

void RouteTest::testEmpty() {
  boost::shared_ptr<MyController> c(new MyController());
  Route r(c);
  const std::vector<Route::Pattern> patterns = r.getPatterns();

  CPPUNIT_ASSERT_EQUAL(std::string(""), r.getName());
  CPPUNIT_ASSERT_EQUAL((size_t)0, patterns.size());

  boost::shared_ptr<MyController> ptr = boost::dynamic_pointer_cast<MyController>(r.getController());
  CPPUNIT_ASSERT(ptr);
  CPPUNIT_ASSERT_EQUAL(c, ptr);
}

void RouteTest::testParser() {
  boost::shared_ptr<MyController> c(new MyController());
  Route r(c);

  try {
    r.addPattern("^/<lala:[\\w\\>]+>/lulu/lili$");
  }
  catch(RouteSyntaxException &e) {
    std::string msg = "Failed with parser error: " + e.getMessage();
    CPPUNIT_FAIL(msg);
  }

  const std::vector<Route::Pattern> &pats = r.getPatterns();
  std::vector<Route::Pattern>::const_iterator it = pats.begin();

  CPPUNIT_ASSERT_EQUAL((size_t)1, pats.size());
  CPPUNIT_ASSERT_EQUAL(std::string("^/([\\w>]+)/lulu/lili$"), it->getPattern());

  const std::vector<std::string> &names = it->getNames();
  std::vector<std::string>::const_iterator s_it = names.begin();

  CPPUNIT_ASSERT_EQUAL((size_t)1, names.size());
  CPPUNIT_ASSERT_EQUAL(std::string("lala"), *s_it);

  try {
    r.addPattern("^/moehlin/aargau/switzerland/europe/earth/milkyway/universe\\.<media:[a-z]+>$");
  }
  catch(RouteSyntaxException &e) {
    std::string msg = "Failed with parser error: " + e.getMessage();
    CPPUNIT_FAIL(msg);
  }

  CPPUNIT_ASSERT_EQUAL((size_t)2, pats.size());
}

void RouteTest::testParserFail() {
  boost::shared_ptr<MyController> c(new MyController());
  Route r(c);

  try {
    r.addPattern("^/<lala-wefwef:\\w+>/lulu/lili$");
    CPPUNIT_FAIL("Pattern should have failed!");
  }
  catch(RouteSyntaxException &e) {
  }

  CPPUNIT_ASSERT_EQUAL((size_t)0, r.getPatterns().size());


  try {
    r.addPattern("^/<lala:\\w+/lulu/lili$");
    CPPUNIT_FAIL("Pattern should have failed!");
  }
  catch(RouteSyntaxException &e) {
  }

  CPPUNIT_ASSERT_EQUAL((size_t)0, r.getPatterns().size());

  try {
    r.addPattern("^/<lala\\w+>/lulu/lili$");
    CPPUNIT_FAIL("Pattern should have failed!");
  }
  catch(RouteSyntaxException &e) {
  }

  CPPUNIT_ASSERT_EQUAL((size_t)0, r.getPatterns().size());
}

/* eof */
