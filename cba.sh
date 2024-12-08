#!/bin/bash
fname="$1"
codepath="storage/shared/codes"
cd ~/"$codepath"
if [ -z "$1" ]; then
printf "\e[44mcodes:\n\e[0m";ls
printf "\e[30;106mEnter source file name:\e[0;34m\t"
read fname
fi
if [ -e "$fname.cpp" ]; then
oname="$2"
  if [ -z "$2" ]; then
  printf "\e[30;106mEnter object file name:\e[0;34m\t"
  read oname; printf "\e[0m"
  fi
g++ -o ~/$oname -w "$fname".cpp
  if [ $? -eq 0 ]; then
  printf "\e[30;102mrunning\e[34;1m $oname\e[0;102;30m now…\e[0m\n"
  ~/$oname
  else
  printf "\e[41mCode error, try:\t\e[mg++ ~/$codepath/$fname.cpp \n"
  g++ "$fname".cpp 2> ~/err.txt
  less ~/err.txt
  rm ~/err.txt
  fi
else echo "$fname.cpp doesn't exist"
fi