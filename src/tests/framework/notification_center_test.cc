/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief notification interface testing
 * \package unittests
 *
 * notification interface testing
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

#include "notification_center_test.hh"

CPPUNIT_TEST_SUITE_REGISTRATION(NotificationCenterTest);

using namespace CForum;

static boost::shared_ptr<NotificationCenterTest> inst;

void NotificationCenterTest::setUp() {
  setenv("REQUEST_METHOD", "GET", 1);
  setenv("QUERY_STRING", "", 1);
  setenv("PATH_INFO", "/just/a/test", 1);
  setenv("SERVER_NAME", "localhost", 1);
  setenv("SERVER_PROTOCOL", "http", 1);
  setenv("SERVER_PORT", "80", 1);

  inst = boost::make_shared<NotificationCenterTest>(*this);
}

void NotificationCenterTest::receiveNotification(boost::shared_ptr<Request> rq, void *data) {
  (void)rq;
  (void)data;

  called = true;
}

void NotificationCenterTest::testNotify() {
  NotificationCenter ncenter;
  boost::shared_ptr<CGIRequest> request(boost::make_shared<CGIRequest>());

  inst->called = false;

  ncenter.registerNotification("test-notify", inst);
  ncenter.notify("test-notify", request, NULL);

  CPPUNIT_ASSERT(inst->called);
}


void NotificationCenterTest::testNoNotify() {
  NotificationCenter ncenter;
  boost::shared_ptr<CGIRequest> request(boost::make_shared<CGIRequest>());

  inst->called = false;

  ncenter.registerNotification("test-notify", inst);
  ncenter.notify("test-notify-lala", request, NULL);

  CPPUNIT_ASSERT_EQUAL(false, inst->called);
}

void NotificationCenterTest::tearDown() {
  unsetenv("REQUEST_METHOD");
  unsetenv("QUERY_STRING");
  unsetenv("PATH_INFO");
  unsetenv("SERVER_NAME");
  unsetenv("SERVER_PROTOCOL");
  unsetenv("SERVER_PORT");
}

/* eof */
