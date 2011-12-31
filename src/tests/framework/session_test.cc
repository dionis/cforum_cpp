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

#include "session_test.hh"

CPPUNIT_TEST_SUITE_REGISTRATION(SessionTest);

using namespace CForum;

class MyVal : public Session::Value {
public:
  MyVal() : x(3), y(10) { }

  int x, y;

private:
  friend class boost::serialization::access;

  template<class Archive> void serialize(Archive &ar, const unsigned int) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Session::Value);

    ar & x;
    ar & y;
  }
};

BOOST_CLASS_EXPORT(MyVal)

static std::string sid;

void SessionTest::testWriteSession() {
  Session s;
  MyVal val1,val2;

  sid = s.getSessionId();

  val1.x = 100;
  val1.y = 75;

  val2.x = 103;
  val2.y = 104;

  boost::shared_ptr<MyVal> val_ptr = boost::make_shared<MyVal>(val1), val_ptr1 = boost::make_shared<MyVal>(val2);

  s["lulu"] = val_ptr;
  s["lala"] = val_ptr1;

  val_ptr = boost::dynamic_pointer_cast<MyVal>(s["lulu"]);
  val_ptr1 = boost::dynamic_pointer_cast<MyVal>(s["lala"]);

  CPPUNIT_ASSERT_EQUAL(val1.x, val_ptr->x);
  CPPUNIT_ASSERT_EQUAL(val1.y, val_ptr->y);

  CPPUNIT_ASSERT_EQUAL(val2.x, val_ptr1->x);
  CPPUNIT_ASSERT_EQUAL(val2.y, val_ptr1->y);

  s.save();
}

void SessionTest::testReadSession() {
  Session s(sid);

  s.load();

  boost::shared_ptr<MyVal> ptr1 = boost::dynamic_pointer_cast<MyVal>(s["lulu"]), ptr2 = boost::dynamic_pointer_cast<MyVal>(s["lala"]);

  CPPUNIT_ASSERT_EQUAL(100, ptr1->x);
  CPPUNIT_ASSERT_EQUAL(75, ptr1->y);

  CPPUNIT_ASSERT_EQUAL(103, ptr2->x);
  CPPUNIT_ASSERT_EQUAL(104, ptr2->y);
}

void SessionTest::testFailSession() {
  try {
    Session s(sid, true);
    CPPUNIT_FAIL("Session created but shouldn't!");
  }
  catch(SessionException &e) {
  }

  std::string fname("/tmp/sess_");
  fname += sid;

  unlink(fname.c_str());
}

/* eof */
