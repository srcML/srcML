#!/usr/bin/python

import os
import sys

VM_COMMAND='vmrun'
VM_DIRECTORY='/Volumes/My Book Thunderbolt Duo/VMware/'

machines = [
   'OpenSUSE Linux 13.2 Build'
 , 'OpenSUSE Linux 13.2 Test'
 , 'OpenSUSE Linux 13.2 Build (64-bit)'
 , 'OpenSUSE Linux 13.2 Test (64-bit)'
 , 'Ubuntu Linux 14.04 Build'
 , 'Ubuntu Linux 14.04 Test'
 , 'Ubuntu Linux 14.04 Build (64-bit)'
 , 'Ubuntu Linux 14.04 Test (64-bit)'
 , 'Ubuntu Linux 16.04 Build (64-bit)'
 , 'Mint Linux 17 Build'
 , 'Mint Linux 17 Test'
 , 'Mint Linux 17.1 Build'
 , 'Mint Linux 17.1 Test'
 , 'Mint Linux 17 Build (64-bit)'
 , 'Mint Linux 17 Test (64-bit)'
 , 'Mint Linux 17.1 Build (64-bit)'
 , 'Mint Linux 17.1 Test (64-bit)'
]

def vm_name_2_path(machine) :
    return '\'' + VM_DIRECTORY + machine + '.vmwarevm' + '\''



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

args = ""

for arg in argv[2:] :

    args += " " + arg
args += ""

if args == "''" :
    args = ""

if background == "" :
    for match in matches :

        command = VM_COMMAND + " " + argv[1] + " " + vm_name_2_path(match)  + args
        print command

        os.system(command)
else :
    list = "'"
    for match in matches :
        
        list += match + "\\n"
    list += "'"

    command = "gecho -e " + list + " | gxargs -I% -t  " + VM_COMMAND + argv[1] + " '%'" + args 
    print command

    os.system(command)
