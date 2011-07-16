#!/bin/sh
cd /
cd home/bryanta/
cd AMOS/
./bootstrap > /home/bryanta/ubuntuLast.log
if [ $? -ne 0 ]
then
cp /home/bryanta/ubuntuLast.log /home/bryanta/ubuntuLast_Failed.log
echo "bootstrap error" >> /home/bryanta/ubuntuLast_Failed.log
/usr/bin/expect <<EOD
spawn scp /home/bryanta/ubuntuLast_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
echo "1234561" | sudo -S shutdown -h now
fi

./configure --prefix=/usr/local/AMOS >> /home/bryanta/ubuntuLast.log
if [ $? -ne 0 ]
then
cp /home/bryanta/ubuntuLast.log /home/bryanta/ubuntuLast_Failed.log
echo "configure error" >> /home/bryanta/ubuntuLast_Failed.log
/usr/bin/expect <<EOD
spawn scp /home/bryanta/ubuntuLast_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
echo "1234561" | sudo -S shutdown -h now
fi

make >> /home/bryanta/ubuntuLast.log
if [ $? -ne 0 ]
then
cp /home/bryanta/ubuntuLast.log /home/bryanta/ubuntuLast_Failed.log
echo "make error" >> /home/bryanta/ubuntuLast_Failed.log
/usr/bin/expect <<EOD
spawn scp /home/bryanta/ubuntuLast_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
echo "1234561" | sudo -S shutdown -h now
fi

make check >> /home/bryanta/ubuntuLast.log
if [ $? -ne 0 ]
then
cp /home/bryanta/ubuntuLast.log /home/bryanta/ubuntuLast_Failed.log
echo "make check error" >> /home/bryanta/ubuntuLast_Failed.log
/usr/bin/expect <<EOD
spawn scp /home/bryanta/ubuntuLast_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
echo "1234561" | sudo -S shutdown -h now
fi

echo "1234561" | sudo -S make install >> /home/bryanta/ubuntuLast.log
if [ $? -ne 0 ]
then
cp /home/bryanta/ubuntuLast.log /home/bryanta/ubuntuLast_Failed.log
echo "make install error" >> /home/bryanta/ubuntuLast_Failed.log
/usr/bin/expect <<EOD
spawn scp /home/bryanta/ubuntuLast_Failed.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD
echo "1234561" | sudo -S shutdown -h now
fi

echo "1234561" | sudo -S ln -s /usr/local/AMOS/bin/* /usr/local/bin/
echo "sending log to walnut..."
now=$(date +"%y%m%d")
echo "SUCCESS: complete log stored on http://sauron.cs.umd.edu/$now" >> /home/bryanta/ubuntuLast.log
/usr/bin/expect <<EOD
spawn scp /home/bryanta/ubuntuLast.log ssh@sauron.cs.umd.edu:VMlogs
expect "ssh@sauron.cs.umd.edu's password:"
send "123\r"
expect eof
EOD

echo "deleting log..."
rm /home/bryanta/ubuntuLast.log


echo "shutting down..."
echo "1234561" | sudo -S shutdown -h now
