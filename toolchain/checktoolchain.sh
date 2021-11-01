#!/bin/sh
# There is a version of GDB that does support multitarget, but not all of them do, currently assume the OS shipped one doesn't
COMPILER="gcc g++ as ld objcopy objdump gdb"
TOOLS="openocd"
check_binaries()
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
      TOOLPATH=$(which $tool 2>/dev/null)
      if [ $? -eq 0 ]; then
        echo "$tool found: $TOOLPATH"
      else
        echo "$tool not found"
        error=1
      fi
    done
    return $error
}
do_pacman_install()
{
  # Check if sudo is installed
  which sudo
  if [ $? -eq 0 ]; then
    # Trigger database update
    sudo pacman -Syy
    sudo pacman -S arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-gdb arm-none-eabi-newlib openocd
    return $?
  elif [ $(id -u) -eq 0 ]; then
    # We're root, so we can still proceed
    pacman -Syy
    pacman -S arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-gdb arm-none-eabi-newlib openocd
    return $?
  else
    # We can't really do anything except complain
    echo "You need to be root or have sudo set up for this script to automatically install the required packages"
    return 1
  fi
}

echo Checking toolchain for $1

case $1 in
  Windows)
    echo "???"
  ;;
  Linux)
    # Check if all required compiler binaries can be found
    CROSS="arm-none-eabi-"
    check_binaries
    if [ $? -eq 0 ]; then
      echo "Toolchain binaries are present"
    else
      # Try to figure out which distro we're on in order to attempt to install the right packages
      which lsb_release
      if [ $? -eq 0 ]; then
        DISTRO=$(lsb_release -is)
        case $DISTRO in
          "LinuxMint"|"Mint")
          ;&
          "Ubuntu")
          ;&
          "Debian")
            # Almost all debian derivatives use aptitude as their package manager
            do_apt_install
          ;;
          "Gentoo")
            do_portage_install
          ;;
          "Artix"|"artix"|"artixlinux"|"Artix release"|"Artix Linux")
          ;&
          "ManjaroLinux")
          ;&
          "Arch"|"archlinux"|"arch"|"Arch Linux")
           # Almost all arch derivatives use pacman as their package manager
            do_pacman_install
          ;;
        esac
      else
        echo "poop"
      fi
      
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
    check_binaries
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

