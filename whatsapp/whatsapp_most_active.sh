#!/bin/sh

if test $# -lt 1
then
  echo "usage: $0 <whatsapp_archive.txt>"
  exit 1
fi

# Developer : Surinder Kumar
# Find most active members in a whatsapp group
# It was used to identify most activ members for DPS East group

WHATSAPP_ARCHIVE=$1

# Find out most active 15 members
cat $WHATSAPP_ARCHIVE | awk -F '-' '{print $2}' | awk -F ':' '{print $1}' | sort | uniq -c | sort -r | head -16
