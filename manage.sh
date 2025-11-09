#!/bin/bash

CONF_INSTALL_DIR="$HOME/.config"
BIN_INSTALL_DIR="/usr/bin"

if [ $# -lt 1 ]
then
  echo "Usage: $0 command arguments" >&2
  echo "" >&2
  echo "  install           Install the nimped++ binary (system-wide)" >&2
  echo "  install-config    Install the nimped++ configuration (per-user)" >&2
  echo "  uninstall         Uninstall the nimped++ binary" >&2
  echo "  uninstall-config  Uninstall the nimped++ binary" >&2
  exit 1
fi

if [ $1 = "install" ]
then
  if [ $# -ne 1 ]
  then
    echo "Usage: $0 install" >&2
    exit 1
  fi
  
  echo "Info: Pre-install checks" >&2
  
  if [ $EUID -ne 0 ]
  then
    echo "Err: Must be running as root to install!" >&2
    exit 1
  fi
  
  stat bin/Release/nimped++ > /dev/null 2> /dev/null
  if [ $? -ne 0 ]
  then
    echo "Err: Build the program in release mode before installing!" >&2
    exit 1
  fi
  
  echo "Info: Copying executable" >&2
  cp bin/Release/nimped++ $BIN_INSTALL_DIR/nimped++ 2> /dev/null
  if [ $? -ne 0 ]
  then
    echo "Err: Failed to copy executable to system!" >&2
    exit 1
  fi
elif [ $1 = "install-config" ]
then
  if [ $# -ne 2 ]
  then
    echo "Usage: $0 install-config theme" >&2
    exit 1
  fi
  
  stat themes/$2.conf > /dev/null 2> /dev/null
  if [ $? -ne 0 ]
  then
    echo "Err: Unrecognized theme: $2!" >&2
    exit 1
  fi
  
  echo "Info: Creating fresh config directory" >&2
  rm -rf $CONF_INSTALL_DIR/nimped++ 2> /dev/null
  
  mkdir $CONF_INSTALL_DIR/nimped++ 2> /dev/null
  if [ $? -ne 0 ]
  then
    echo "Err: Failed to create config directory!" >&2
    exit 1
  fi
  
  echo "Info: Copying config" >&2
  cp -r config/* $CONF_INSTALL_DIR/nimped++ 2> /dev/null
  if [ $? -ne 0 ]
  then
    echo "Err: Failed to copy config!" >&2
    exit 1
  fi
  
  echo "Info: Copying theme" >&2
  cp themes/$2.conf $CONF_INSTALL_DIR/nimped++/color.conf 2> /dev/null
  if [ $? -ne 0 ]
  then
    echo "Err: Failed to copy theme!" >&2
    exit 1
  fi
elif [ $1 = "uninstall" ]
then
  if [ $# -ne 1 ]
  then
    echo "Usage: $0 uninstall" >&2
    exit 1
  fi
  
  echo "Info: Pre-uninstall checks" >&2
  
  if [ $EUID -ne 0 ]
  then
    echo "Err: Run the script as root!" >&2
    exit 1
  fi
  
  stat $BIN_INSTALL_DIR/nimped++ > /dev/null 2> /dev/null
  if [ $? -ne 0 ]
  then
    echo "Err: The program is not installed!" >&2
    exit 1
  fi
  
  echo "Info: Deleting executable"
  rm $BIN_INSTALL_DIR/nimped++ 2> /dev/null
  if [ $? -ne 0 ]
  then
    echo "Err: Failed to delete executable!" >&2
    exit 1
  fi
elif [ $1 = "uninstall-config" ]
then
  if [ $# -ne 1 ]
  then
    echo "Usage: $0 uninstall-config" >&2
    exit 1
  fi
  
  echo "Info: Deleting config directory" >&2
  rm -rf $CONF_INSTALL_DIR/nimped++ 2> /dev/null
  if [ $? -ne 0 ]
  then
    echo "Err: Failed to delete config directory!" >&2
    exit 1
  fi
else
  echo "Invalid command!" >&2
fi

echo "Info: Finished successfully" >&2
