#!/usr/bin/python

import os
import sys

machines = [
      "fedora20build"
    , "fedora20test"
    , "fedora21build"
    , "fedora21test"
    , "fedora20build64"
    , "fedora20test64"
    , "fedora21build64"
    , "fedora21test64"
    , "suse13build"
    , "suse13test"
    , "suse132build"
    , "suse132test"
    , "suse13build64"
    , "suse13test64"
    , "suse132build64"
    , "suse132test64"
    , "ubuntu12build"
    , "ubuntu12test"
    , "ubuntu14build"
    , "ubuntu14test"
    , "ubuntu12build64"
    , "ubuntu12test64"
    , "ubuntu14build64"
    , "ubuntu14test64"
    , "macmavericksbuild"
    , "macmaverickstest"
    , "mint13build"
    , "mint13test"
    , "mint17build"
    , "mint17test"
    , "mint171build"
    , "mint171test"
    , "mint13build64"
    , "mint13test64"
    , "mint17build64"
    , "mint17test64"
    , "mint171build64"
    , "mint171test64"
]

options = []

argv = sys.argv[:]

background = ""

for arg in sys.argv[1:] :

    if arg.lower().find("--bg") == 0 :
        background = "true"
        argv.remove(arg)

    elif arg.find("--") == 0 :
        options.append(str(arg.lstrip("-")).lower())
        argv.remove(arg)
    else :
        break;

matches = machines[:]

for option in options :

    for machine in machines :

        if machine not in matches :
            continue

        if option[0] == '~' :           
            if machine.lower().find(option[1:]) != -1 :
                matches.remove(machine)
        else :
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
