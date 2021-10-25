#!/bin/sh
echo Checking toolchain for $1
case $1 in
  Windows)
    echo "???"
  ;;
  Linux)
    
  ;;
  Darwin)
    if [ $(uname -m) != "x86_64" ]; then
      echo "only AMD64 is supported"
      exit 1
    fi
    echo "Checking for homebrew"
    which brew
    if [ $? -ne 0 ]; then
      echo "Running brew install script"
      /bin/sh -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    if [ $? -ne 0 ]; then
      echo "Error installing brew"
      exit 2
    fi
    # Assume brew is successfully installed
    brew tap osx-cross/arm
    brew install arm-gcc-bin open-ocd gdb
    # This should create a functioning install of gcc as arm-none-eabi-*
  ;;
  *)
esac
