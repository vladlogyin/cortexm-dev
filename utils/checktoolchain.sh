#!/bin/sh
COMPILER="gcc g++ as ld objcopy objdump"
TOOLS="gdb openocd"
checkbinaries()
{
 error=0
  for compiler in $COMPILER
    do
      COMPILERPATH=$(which $CROSS$compiler 2>/dev/null)
      if [ $? -eq 0 ]; then
        echo "$CROSS$compiler found: $COMPILERPATH"
      else
        echo "$CROSS$compiler not found"
        error=1
      fi
    done
    
    # Check if all other tool binaries can be found
    for tool in $TOOLS
    do
      TOOLPATH=$(which tool 2>/dev/null)
      if [ $? -eq 0 ]; then
        echo "$tool found: $TOOLPATH"
      else
        echo "$tools not found"
        error=1
      fi
    done
    return $error
}


echo Checking toolchain for $1

case $1 in
  Windows)
    echo "???"
  ;;
  Linux)
    # Check if all required compiler binaries can be found
    CROSS="arm-none-eabi-"
    checkbinaries
    if [ $? -eq 0 ]; then
      echo "Toolchain binaries are present"
    else
      # Change this to actually try to install a toolchain
      echo "Something went wrong! Toolchain binaries are not present"
      exit 3
    fi
  ;;
  Darwin)
    if [ $(uname -m) != "x86_64" ]; then
      echo "only AMD64 is supported"
      exit 1
    fi
    which brew
    # If brew is not found in $PATH, try installing it
    if [ $? -eq 0 ]; then
      echo "Homebrew is installed"
    else
      echo "Running brew install script"
      /bin/sh -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    # If installer script return anything other than 0, error out
    if [ $? -eq 0 ]; then
      echo "Homebrew is installed"
    else
      echo "Error installing brew"
      exit 2
    fi
    # Assume brew is successfully installed
    brew tap osx-cross/arm
    brew install arm-gcc-bin open-ocd gdb
    # This should create a functioning install of gcc as arm-none-eabi-*
    # Check this is actually true
    CROSS="arm-none-eabi-"
    checkbinaries
    if [ $? -eq 0 ]; then
      echo "Toolchain binaries are present"
    else
      echo "Something went wrong! Toolchain binaries are not present"
      exit 3
    fi
  ;;
  *)
    echo "$1 is unsupported"
  ;;
esac

