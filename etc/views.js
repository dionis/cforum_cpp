function out(val) {
  val = val.replace(/&/, '&amp;');
  val = val.replace(/</, '&lt;');
  val = val.replace(/>/, '&gt;');

  _e(val);
}

function absURL(thread, message, method, query) {
  var base = configparser.getByPath("system/urls/base", false);
  var url = base + thread.id;
  if(message) {
    url += "/" + message.id;
  }
  if(method) {
    url += ";" + method;
  }
  if(query) {
    url += "?" + url;
  }

  return url;
}


