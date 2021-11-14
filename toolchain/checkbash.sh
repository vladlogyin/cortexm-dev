#!/bin/sh
bash --version | grep -x ".*bash, version [4-9].*"
if [ $? -eq 0 ]; then
  exit 0
fi
if [ $1 == "Darwin" ]; then
  which brew
  # If brew is not found in $PATH, try installing it
  if [ $? -eq 0 ]; then
    echo "Homebrew is installed"
  else
    echo "Running brew install script"
    sh -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    if [ $? -ne 0 ]; then
      exit $?
    fi
  fi
  
  brew install bash
  exit $?
fi
printf "Your version of bash is not supported!\nPlease update to bash 4 or newer\n"
exit 1
