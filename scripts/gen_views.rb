#!/usr/bin/ruby -wKu
# -*- coding: utf-8 -*-

require 'couchrest'
require 'json'

views_file = "./etc/views.js"
db_uri = "http://127.0.0.1:5984/cforum"

views_file = ARGV[0] if ARGV.length > 0
db_uri = ARGV[1] if ARGV.length > 1

db = CouchRest.database!(db_uri)
views = JSON.parse(File.read(views_file))

begin
  db.save_doc(views)
rescue
  puts "Error creating views: " + $!.to_s
end


# eof
