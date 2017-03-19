#!/usr/bin/ruby

require 'cgi'

puts "Content-Type: text/html"
puts

puts "<html>"
puts "<head>"
puts "<link rel='stylesheet' href='style.css' type='text/css'>"
puts "</head>"
puts "<body>"
File.open("/var/sshpot/sshpot.log", "a+") {|f|
	f.flock(File::LOCK_EX)
	f.each_line {|line|
		if line =~ /^(\d\d\d\d-\d\d-\d\d) (\d\d\:\d\d\:\d\d) (\d+\.\d+\.\d+\.\d+) (\S+) (\S+)$/
			puts "<div class='attack'><div class='time'>#{$1} #{$2}</div><div class='from'>from: #{$3}</div><div class='auth'><span class='uid'>#{CGI.escapeHTML($4)}</span>/<span class='pwd'>#{CGI.escapeHTML($5)}</span></div></div>"
		end
	}
	f.flock(File::LOCK_UN)
}
puts "</body>"
puts "</html>"

