#!/usr/bin/lua

-- checkTime: specifies the interval in seconds in which
-- the script is called. only integer values are acceptable
checkTime = 3

-- script: specifies the script name which the daemon should call.
-- do NOT give the full path, only the name. the working directory is specified
-- in the main config file. you should ensure that this script is executable for 
-- the dramaqueen user.
-- dramaqueen notifies you only if this script produces output. so it is your turn to ensure,
-- that it only prints to std::out it you want a message.
script    = "zabbix.lua"

-- recipients is a table of user, who should be notified by dramaqueen.
recipients = { "roa@localhost", "admin@localhost" }

-- hosts is a table of hosts, on which dramaqueen tries to execute the script.
-- dramaqueen will not inform you, if a host could not be reached.
hosts = { "localhost:9898" }
