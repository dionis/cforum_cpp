{
  "_id": "_design/cforum",
  "language": "javascript",
  "views": {
    "by_tid": {
      "map": "function(doc) { if(doc.tid) emit(doc.tid); }"
    },
    "by_month": {
      "map": "function(doc) { emit(doc.messages[0].date.substr(0,7)); }"
    },
    "threadlist": {
      "map": "function(doc) { if(!doc.archived) emit(doc.messages[0].date); }"
    }
  }
}
