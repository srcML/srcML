#!/usr/bin/python

import os
import sys

machines = [
    "fedora17build"
    , "fedora17test"
    , "fedora18build"
    , "fedora18test"
    , "fedora19build"
    , "fedora19test"
    , "fedora17build64"
    , "fedora17test64"
    , "fedora18build64"
    , "fedora18test64"
    , "fedora19build64"
    , "fedora19test64"
    , "suse13build"
    , "suse13test"
    , "suse13build64"
    , "suse13test64"
    , "ubuntu10build"
    , "ubuntu10test"
    , "ubuntu12build"
    , "ubuntu12test"
    , "ubuntu10build64"
    , "ubuntu10test64"
    , "ubuntu12build64"
    , "ubuntu12test64"
]

options = []

argv = sys.argv[:]

background = ""

for arg in sys.argv[1:] :

    if arg.lower().find("bg") != -1 :
        background = "true"
        argv.remove(arg)

    elif arg.find("--") != -1 :
        options.append(str(arg.lstrip("-")).lower())
        argv.remove(arg)
    elif arg.find("--") == -1 :
        break;

matches = machines[:]

for option in options :

    for machine in machines :

        if machine not in matches :
            continue

        if machine.lower().find(option) == -1 :
            matches.remove(machine)

args = "'"

for arg in argv[1:] :

    args += " " + arg
args += "'"

if args == "''" :
    args = ""

if background == "" :
    for match in matches :

        command = "ssh root@" + match + " " + args
        print command

        os.system(command)
else :
    list = "'"
    for match in matches :
        
        list += match + "\\n"
    list += "'"

    command = "gecho -e " + list + " | gxargs -I% -t ssh root@" + "'%' " + args 
    print command

    os.system(command)
