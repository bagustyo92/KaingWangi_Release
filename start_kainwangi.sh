#!/bin/bash

sudo pcscd
sudo /var/kainwangi/server/ELM/xelm/elm &
#sleep 5 && /var/etollDB/etollDB printApdu &
/var/etollDB/etollDB printApdu &
exec /var/kainwangi/build-test-Desktop-Release/test &
