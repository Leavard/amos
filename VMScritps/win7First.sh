#!/bin/bash
cd c:
cd cygwin/
cd AMOS/
./bootstrap > /cygdrive/c/cygwin/win7First.log
if [ $? -ne 0 ]
then
cp /cygdrive/c/cygwin/win7First.log /cygdrive/c/cygwin/win7First_Failed.log
usr/bin/expect <<EOD
spawn scp /cygdrive/c/cygwin/win7First_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
shutdown /s
fi

./configure --prefix=/usr/local/AMOS >> /cygdrive/c/cygwin/win7First.log
if [ $? -ne 0 ]
then
cp /cygdrive/c/cygwin/win7First.log /cygdrive/c/cygwin/win7First_Failed.log
usr/bin/expect <<EOD
spawn scp /cygdrive/c/cygwin/win7First_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
shutdown /s
fi

make >> /cygdrive/c/cygwin/win7First.log 
if [ $? -ne 0 ]
then
cp /cygdrive/c/cygwin/win7First.log /cygdrive/c/cygwin/win7First_Failed.log
usr/bin/expect <<EOD
spawn scp /cygdrive/c/cygwin/win7First_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
shutdown /s
fi

make install >> /cygdrive/c/cygwin/win7First.log
if [ $? -ne 0 ]
then
cp /cygdrive/c/cygwin/win7First.log /cygdrive/c/cygwin/win7First_Failed.log
usr/bin/expect <<EOD
spawn scp /cygdrive/c/cygwin/win7First_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
shutdown /s
fi

/usr/bin/expect <<EOD
spawn scp /cygdrive/c/cygwin/win7First.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
rm /cygdrive/c/cygwin/win7First.log

shutdown /s

