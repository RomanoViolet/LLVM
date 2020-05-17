#!/usr/bin/env bash

HomeDirectory=`pwd`

# Initialize gtags
gtags 

# Enable Git Credentials Helper
git config --global credential.helper "cache --timeout=36000"
