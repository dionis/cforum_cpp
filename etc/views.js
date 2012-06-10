function out(val) {
  val = val.replace(/&/, '&amp;');
  val = val.replace(/</, '&lt;');
  val = val.replace(/>/, '&gt;');

  _e(val);
}



