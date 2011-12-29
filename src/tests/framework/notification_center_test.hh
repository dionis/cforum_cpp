/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief notification center interface testing
 * \package unittests
 *
 * notification center interface testing
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

#ifndef NOTIFICATION_CENTER_TEST_H
#define NOTIFICATION_CENTER_TEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "framework/notification_center.hh"
#include "framework/cgi_request.hh"

#include <boost/make_shared.hpp>

class NotificationCenterTest : public CppUnit::TestFixture, public CForum::NotificationCenter::NotificationReceiver {
  CPPUNIT_TEST_SUITE(NotificationCenterTest);
  CPPUNIT_TEST(testNotify);
  CPPUNIT_TEST(testNoNotify);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void testNotify();
  void testNoNotify();
  void tearDown();

  virtual void receiveNotification(boost::shared_ptr<CForum::Request>, void *);

private:
  bool called;
  boost::shared_ptr<NotificationCenterTest> myself;
};

#endif

/* eof */
