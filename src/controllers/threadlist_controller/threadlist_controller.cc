/**
 * \author Christian Kruse <cjk@wwwtech.de>
 * \brief CForum threadlist controller
 *
 * CForum threadlist controller, responds to /
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

#include "controllers/threadlist_controller/threadlist_controller.hh"

static boost::shared_ptr<CForum::ThreadlistController> myself;

namespace CForum {
  ThreadlistController::ThreadlistController() : Controller::Controller() { }
  ThreadlistController::ThreadlistController(const ThreadlistController &) { }

  void ThreadlistController::registerController(Application *app) {
    boost::shared_ptr<Route> route = boost::make_shared<Route>(myself);

    route->addPattern("^/?$");
    app->getRouter()->registerRoute("threadlist-route", route);
  }

  void ThreadlistController::preRoute(boost::shared_ptr<Request> rq) {
    CGI c = rq->getCGI();

    std::string t, m;
    c.getFirstValue("t").toUTF8String(t);
    c.getFirstValue("m").toUTF8String(m);

    if(!t.empty()) {
      std::auto_ptr<mongo::DBClientCursor> cursor = app->getMongo()->query("threads", QUERY("tid" << "t" + t));

      if(!cursor->more()) {
        throw NotFoundException("Thread with tid t" + t + " could not be found!", NotFoundException::ThreadNotFoundError);
      }

      boost::shared_ptr<Models::Thread> t = Models::Thread::fromBSON(cursor->next());
      boost::shared_ptr<Models::Message> msg;
      std::string url;

      if(!m.empty()) {
        if(msg = t->getMessage(m)) {
          url = app->absURL(*t, *msg);
        }
        else {
          throw NotFoundException("Message with tid m" + m + " could not be found!", NotFoundException::MessageNotFoundError);
        }
      }
      else {
        url = app->absURL(*t);
      }

      throw PermanentRedirectException(url);
    }

  }

  const std::string ThreadlistController::handleRequest(boost::shared_ptr<Request> rq, const std::map<std::string, std::string> &vars) {
    std::auto_ptr<mongo::DBClientCursor> cursor = app->getMongo()->query("threads", QUERY("archived" << false).sort("messages.0.date"));
    mongo::BSONObj obj;
    v8::Local<v8::Array> ary = v8::Array::New();
    boost::shared_ptr<Template> tpl = rq->getTemplate();
    uint32_t i = 0;
    boost::shared_ptr<Models::Thread> t;

    while(cursor->more()) {
      obj = cursor->next();
      t = Models::Thread::fromBSON(obj);
      ary->Set(i++, t->toV8());
    }

    tpl->setVariable("threadlist", ary);

    view = "threadlist/threadlist.html";
    return Controller::handleRequest(rq, vars);
  }

  ThreadlistController::~ThreadlistController() {
  }

}

extern "C" {
  boost::shared_ptr<CForum::Controller> cf_init(CForum::Application *) {
    myself = boost::make_shared<CForum::ThreadlistController>();
    return myself;
  }
}

/* eof */
