#!/usr/bin/ruby -wKu
# -*- coding: utf-8 -*-

require 'mongo'
require 'json'

db = "localhost/cforum"
db = ARGV[0] if ARGV.length > 0

host, db = db.split(/\//)

$conn = Mongo::Connection.new(host)
$db = $conn[db]

begin
  $db['threads'].ensure_index('messages.0.date')
  $db['threads'].ensure_index('tid')
  $db['threads'].ensure_index('archived')
rescue
  puts "Error creating views: " + $!.to_s
end
