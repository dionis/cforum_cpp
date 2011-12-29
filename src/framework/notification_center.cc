/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief Notification interface
 * \package framework
 *
 * Notification interface: every object can register for a notification and
 * will get notified when the event occures connected to this notification
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

#include "framework/notification_center.hh"

namespace CForum {
  NotificationCenter::NotificationCenter() : notifications() { }
  NotificationCenter::NotificationCenter(const NotificationCenter &ncenter) : notifications(ncenter.notifications) { }

  NotificationCenter &NotificationCenter::operator=(const NotificationCenter &ncenter) {
    if(this != &ncenter) {
      notifications = ncenter.notifications;
    }

    return *this;
  }

  void NotificationCenter::registerNotification(const std::string &name, boost::shared_ptr<NotificationReceiver> receiver) {
    notifications[name].push_back(receiver);
  }

  void NotificationCenter::notify(const std::string &name, boost::shared_ptr<Request> rq, void *arg) {
    std::vector<boost::shared_ptr<NotificationReceiver> > receivers = notifications[name];
    std::vector<boost::shared_ptr<NotificationReceiver> >::iterator it, end;

    if(receivers.size() > 0) {
      for(end = receivers.end(), it = receivers.begin(); it != end; ++it) {
        (*it)->receiveNotification(rq, arg);
      }
    }
  }

  NotificationCenter::NotificationReceiver::~NotificationReceiver() { }
}

/* eof */
