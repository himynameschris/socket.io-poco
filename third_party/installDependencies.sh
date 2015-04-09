#!/bin/bash
SCRIPT=`pwd`/$0
FILENAME=`basename $SCRIPT`
BUILD_DIR=`dirname $SCRIPT`
LIB_DIR="$BUILD_DIR/local"

cd $BUILD_DIR
CURRENT_DIR=`pwd`

#POCO_VERSION GITHUB: https://github.com/pocoproject/poco.git

usage()
{
cat << EOF
usage: $0 options
OPTIONS:
   -h      Show this message
   -a      all
   -p      poco
   -c      Cleanup
EOF
}

os_type()
{
case `uname` in
  Linux )
     LINUX=1
     which yum >/dev/null && { echo "CentOS"; }
     which zypper >/dev/null && { return OpenSUSE; }
     which apt-get  >/dev/null && { echo "Debian"; }
     ;;
  Darwin )
     DARWIN=1
     echo "MacOS"
     ;;
  * )
     # Handle AmgiaOS, CPM, and modified cable modems here.
     ;;
esac
}

checkOrInstallapt(){
	PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $1|grep "install ok installed")
	echo Checking for $1
	if [ "" == "$PKG_OK" ]; then
	  echo -e "\tNo $1 installed"
	  sudo apt-get --force-yes install $1
	else
	  echo -e "\t$1 Installed"
	fi
}

checkOrInstallaptitude(){
	PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $1|grep "install ok installed")
	echo Checking for $1
	if [ "" == "$PKG_OK" ]; then
	  echo -e "\tNo $1 installed"
	  sudo aptitude --assume-yes install $1
	else
	  echo -e "\t$1 Installed"
	fi
}

checkOrInstallnpm(){
	NPM_OK=$(npm -g ls $1)
	echo Checking for $1
	if [ "" == "$NPM_OK" ]; then
	  echo -e "\tNo $1 installed"
	  sudo npm install -g $1 $2
	else
	  echo -e "\t$1 Installed"
	fi
}

install_apt_deps(){
  echo "$(os_type) installation" 
  case $(os_type) in
    CentOS )
      ;;
    Debian )
      ;;
  esac
}

install_poco(){
  if [ -d $LIB_DIR ]; then
    mkdir -p $LIB_DIR
  fi
  cd $BUILD_DIR
  echo "Install poco in $BUILD_DIR"
  if [ ! -d poco ]; then
    git clone -b develop https://github.com/pocoproject/poco.git
  fi
  cd poco
  git pull
  if [ ! -d bin ]; then
    mkdir bin
  fi
  cd bin
  cmake -DCMAKE_INSTALL_PREFIX:PATH=$LIB_DIR ../
  make
  make install
  cd $CURRENT_DIR  
}

cleanup(){  
  if [ -d $LIB_DIR ]; then
    cd $LIB_DIR
    rm -rf ./*
    cd $CURRENT_DIR
  fi
}

install_all(){
  
  if [ ! -d $LIB_DIR/poco ] || [ ! -f $LIB_DIR/lib/libpoco.a ]
  then
    install_poco
  fi
}

if [ "$#" -eq 0 ]
then
  install_all
else
  while getopts “hapc” OPTION
  do
    case $OPTION in
      h)
        usage
        exit 1
        ;;
      a)
        install_all
        ;;
      p)
        install_poco
        ;;
      c)
        cleanup
        ;;
      ?)
        usage
        exit
        ;;
    esac
  done
fi
