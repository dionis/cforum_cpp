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

#ifndef NOTIFICATION_CENTER_H
#define NOTIFICATION_CENTER_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "hash_map.hh"

namespace CForum {
  class NotificationCenter {
  public:
    class NotificationReceiver {
    public:
      virtual void receiveNotification(void *) = 0;
    };

    NotificationCenter();
    NotificationCenter(const NotificationCenter &);

    NotificationCenter &operator=(const NotificationCenter &);

    void registerNotification(const std::string &, boost::shared_ptr<NotificationReceiver>);
    void notify(const std::string &, void *);

  protected:
    std::unordered_map<std::string, std::vector<boost::shared_ptr<NotificationReceiver> > > notifications;

  };


}


#endif

/* eof */
