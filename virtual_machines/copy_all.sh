#!/bin/bash

if [ ! $# -eq 2  ]; then
 echo "copy_all.sh source_file destination_file"
 exit
fi

from=$1
to=$2

./prlctlall.py --mint --build --13 resume; sleep 15
scp $from mint13build:$to
scp $from mint13build64:$to

scp $from root@mint13build:$to
scp $from root@mint13build64:$to
./prlctlall.py --mint --build --13 suspend

./prlctlall.py --mint --build --~13 resume; sleep 15
scp $from mint17build:$to
scp $from mint171build:$to
scp $from mint17build64:$to
scp $from mint171build64:$to

scp $from root@mint17build:$to
scp $from root@mint171build:$to
scp $from root@mint17build64:$to
scp $from root@mint171build64:$to
./prlctlall.py --mint --build --~13 suspend

./prlctlall.py --mint --test --13 resume; sleep 15
scp $from mint13test:$to
scp $from mint13test64:$to

scp $from root@mint13test:$to
scp $from root@mint13test64:$to
./prlctlall.py --mint --test --13 suspend

./prlctlall.py --mint --test --~13 resume; sleep 15
scp $from mint17test:$to
scp $from mint171test:$to
scp $from mint17test64:$to
scp $from mint171test64:$to

scp $from root@mint17test:$to
scp $from root@mint171test:$to
scp $from root@mint17test64:$to
scp $from root@mint171test64:$to
./prlctlall.py --mint --test --~13 suspend

./prlctlall.py --fedora --build resume; sleep 15
scp $from fedora20build:$to
scp $from fedora21build:$to
scp $from fedora20build64:$to
scp $from fedora21build64:$to

scp $from root@fedora20build:$to
scp $from root@fedora21build:$to
scp $from root@fedora20build64:$to
scp $from root@fedora21build64:$to
./prlctlall.py --fedora --build suspend

./prlctlall.py --fedora --test resume; sleep 15
scp $from fedora20test:$to
scp $from fedora21test:$to
scp $from fedora20test64:$to
scp $from fedora21test64:$to

scp $from root@fedora20test:$to
scp $from root@fedora21test:$to
scp $from root@fedora20test64:$to
scp $from root@fedora21test64:$to
./prlctlall.py --fedora --test suspend

./prlctlall.py --suse --build resume; sleep 15
scp $from suse13build:$to
scp $from suse132build:$to
scp $from suse13build64:$to
scp $from suse132build64:$to

scp $from root@suse13build:$to
scp $from root@suse132build:$to
scp $from root@suse13build64:$to
scp $from root@suse132build64:$to
./prlctlall.py --suse --build suspend

./prlctlall.py --suse --test resume; sleep 15
scp $from suse13test:$to
scp $from suse132test:$to
scp $from suse13test64:$to
scp $from suse132test64:$to

scp $from root@suse13test:$to
scp $from root@suse132test:$to
scp $from root@suse13test64:$to
scp $from root@suse132test64:$to
./prlctlall.py --suse --test suspend

./prlctlall.py --ubuntu --build resume; sleep 15
scp $from ubuntu12build:$to
scp $from ubuntu14build:$to
scp $from ubuntu12build64:$to
scp $from ubuntu14build64:$to

scp $from root@ubuntu12build:$to
scp $from root@ubuntu14build:$to
scp $from root@ubuntu12build64:$to
scp $from root@ubuntu14build64:$to
./prlctlall.py --ubuntu --build suspend

./prlctlall.py --ubuntu --test resume; sleep 15
scp $from ubuntu14test64:$to
scp $from ubuntu12test64:$to
scp $from ubuntu14test:$to
scp $from ubuntu12test:$to

scp $from root@ubuntu14test64:$to
scp $from root@ubuntu12test64:$to
scp $from root@ubuntu14test:$to
scp $from root@ubuntu12test:$toroot@
./prlctlall.py --ubuntu --test suspend
