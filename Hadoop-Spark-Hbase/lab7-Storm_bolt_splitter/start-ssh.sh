#!/bin/bash

#ssh-keygen -t rsa -P '' -f ~/.ssh/id_rsa
#cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys

sudo /usr/sbin/rcsshd start
#sudo rcsshd restart
ssh-add
ssh localhost
