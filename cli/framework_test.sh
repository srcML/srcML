# 

PATH=$PATH:../bin/

# output files for standard output and error
STDERR=.stderr_$(BASENAME $0)
STDOUT=.stdout_$(BASENAME $0)

# log stdout and stderr to files
exec 6>&1 7>&2 1>$STDOUT 2>$STDERR

check() {

   # log stdout and stderr to standard streams
   exec 1>&6 #2>&7

   # verify stdout of command (stdin to this check) agrees with STDOUT
   if [ $# -eq 1 ]; then
       diff $1 <(cat <&3)
       [ "$?" != "0" ] && exit 1
   else
       diff $STDOUT <(cat <&3)
       [ "$?" != "0" ] && exit 1
   fi

   # verify stderr of command agrees with caught stderr
   diff $STDERR <(cat <&4)
   [ "$?" != "0" ] && exit 1

   # log stdout and stderr back to files
   exec 6>&- 7>&- 6>&1 7>&2 1>$STDOUT 2>$STDERR
}
