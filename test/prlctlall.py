#!/usr/bin/python

import os
import sys

machines = [
   '"Fedora Linux 15 Build"'
 , '"Fedora Linux 15 Test"'
 , '"Fedora Linux 16 Build"'
 , '"Fedora Linux 16 Test"'
 , '"Fedora Linux 17 Build"'
 , '"Fedora Linux 17 Test"'
 , '"Fedora Linux 15 Build (64-bit)"'
 , '"Fedora Linux 15 Test (64-bit)"'
 , '"Fedora Linux 16 Build (64-bit)"'
 , '"Fedora Linux 16 Test (64-bit)"'
 , '"Fedora Linux 17 Build (64-bit)"'
 , '"Fedora Linux 17 Test (64-bit)"'
 , '"Ubuntu Linux 10.04 Build"'
 , '"Ubuntu Linux 10.04 Test"'
 , '"Ubuntu Linux 11.04 Build"'
 , '"Ubuntu Linux 11.04 Test"'
 , '"Ubuntu Linux 12.04 Build"'
 , '"Ubuntu Linux 12.04 Test"'
 , '"Ubuntu Linux 10.04 Build (64-bit)"'
 , '"Ubuntu Linux 10.04 Test (64-bit)"'
 , '"Ubuntu Linux 11.04 Build (64-bit)"'
 , '"Ubuntu Linux 11.04 Test (64-bit)"'
 , '"Ubuntu Linux 12.04 Build (64-bit)"'
 , '"Ubuntu Linux 12.04 Test (64-bit)"'
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

for arg in argv[2:] :

    args += " " + arg
args += "'"

if args == "''" :
    args = ""

if background == "" :
    for match in matches :

        command = "prlctl " + argv[1] + " " + match + " " + args
        print command

        os.system(command)
else :
    list = "'"
    for match in matches :
        
        list += match + "\\n"
    list += "'"

    command = "gecho -e " + list + " | gxargs -I% -t prlctl " + argv[1] + " '%' " + args 
    print command

    os.system(command)
