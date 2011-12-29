/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Router interface testing
 * \package unittests
 *
 * Router interface testing
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

#include "router_test.hh"

CPPUNIT_TEST_SUITE_REGISTRATION(RouterTest);

using namespace CForum;

class MyAcl : public Route::ACL {
public:
  virtual bool check(boost::shared_ptr<Request> rq, const std::map<std::string, std::string> &vars) {
    (void)rq;
    (void)vars;
    return false;
  }
};

class MyAcl1 : public Route::ACL {
public:
  virtual bool check(boost::shared_ptr<Request> rq, const std::map<std::string, std::string> &vars) {
    (void)rq;
    (void)vars;
    return true;
  }
};



void RouterTest::setUp() {
  setenv("REQUEST_METHOD", "GET", 1);
  setenv("QUERY_STRING", "", 1);
  setenv("PATH_INFO", "/just/a/test", 1);
  setenv("SERVER_NAME", "localhost", 1);
  setenv("SERVER_PROTOCOL", "http", 1);
  setenv("SERVER_PORT", "80", 1);
}

void RouterTest::testMatching() {
  boost::shared_ptr<MyController> c(new MyController());
  boost::shared_ptr<Route> route(boost::make_shared<Route>(c));
  Router router;
  boost::shared_ptr<CGIRequest> request(boost::make_shared<CGIRequest>());

  route->addPattern("^/just/a/<action:test>$");

  router.registerRoute("test-route", route);

  std::string rslt = router.dispatch(request);
  CPPUNIT_ASSERT_EQUAL(std::string("MyController::handleRequest"), rslt);
}


void RouterTest::testNoMatch() {
  boost::shared_ptr<MyController> c(new MyController());
  boost::shared_ptr<Route> route(boost::make_shared<Route>(c));
  Router router;
  boost::shared_ptr<CGIRequest> request(boost::make_shared<CGIRequest>());

  route->addPattern("^/wefwef/just/a/<action:test>$");

  router.registerRoute("test-route", route);

  try {
    std::string rslt = router.dispatch(request);
    CPPUNIT_FAIL("Fail: route matched but shouldn't");
  }
  catch(NotFoundException &e) {
  }
}

void RouterTest::testAclFalse() {
  boost::shared_ptr<MyController> c(new MyController());
  boost::shared_ptr<Route> route(boost::make_shared<Route>(c));
  Router router;
  boost::shared_ptr<CGIRequest> request(boost::make_shared<CGIRequest>());
  boost::shared_ptr<MyAcl1> acl(boost::make_shared<MyAcl1>());

  route->addPattern("^/just/a/<action:test>$");
  route->setAcl(acl);

  router.registerRoute("test-route", route);

  try {
    std::string rslt = router.dispatch(request);
  }
  catch(NotFoundException &e) {
    CPPUNIT_FAIL("Fail: route didn't match but shouldt!");
  }
}

void RouterTest::testAclTrue() {
  boost::shared_ptr<MyController> c(new MyController());
  boost::shared_ptr<Route> route(boost::make_shared<Route>(c));
  Router router;
  boost::shared_ptr<CGIRequest> request(boost::make_shared<CGIRequest>());
  boost::shared_ptr<MyAcl> acl(boost::make_shared<MyAcl>());

  route->addPattern("^/just/a/<action:test>$");
  route->setAcl(acl);

  router.registerRoute("test-route", route);

  try {
    std::string rslt = router.dispatch(request);
    CPPUNIT_FAIL("Fail: route matched but shouldn't");
  }
  catch(NotFoundException &e) {
  }
}

void RouterTest::testEmpty() {
  boost::shared_ptr<MyControllerEmpty> c(boost::make_shared<MyControllerEmpty>());
  boost::shared_ptr<Route> route(boost::make_shared<Route>(c));
  Router router;
  boost::shared_ptr<CGIRequest> request(boost::make_shared<CGIRequest>());

  route->addPattern("^/just/a/<action:test>$");

  router.registerRoute("test-route", route);

  try {
    std::string rslt = router.dispatch(request);
    CPPUNIT_FAIL("Fail: route returned something but shouldn't");
  }
  catch(InternalErrorException &e) {
  }
}

void RouterTest::tearDown() {
  unsetenv("REQUEST_METHOD");
  unsetenv("QUERY_STRING");
  unsetenv("PATH_INFO");
  unsetenv("SERVER_NAME");
  unsetenv("SERVER_PROTOCOL");
  unsetenv("SERVER_PORT");
}


/* eof */
