#!/bin/bash

INSTALL_DIR="$HOME/.config"

if [ $# -ne 1 ]
then
  echo "usage: $0 theme" >&2
  exit 1
fi

stat themes/$1.conf > /dev/null 2> /dev/null
if [ $? -ne 0 ]
then
  echo "[$0] install-config: unrecognized theme: $1!" >&2
  exit 1
fi

echo "[$0] install-config: creating fresh config directory" >&2
rm -rf $INSTALL_DIR/nimped++ 2> /dev/null

mkdir $INSTALL_DIR/nimped++ 2> /dev/null
if [ $? -ne 0 ]
then
  echo "[$0] install-config: failed to create config directory!" >&2
  exit 1
fi

echo "[$0] install-config: copying config" >&2
cp -r config/* $INSTALL_DIR/nimped++ 2> /dev/null
if [ $? -ne 0 ]
then
  echo "[$0] install-config: failed to copy config!" >&2
  exit 1
fi

echo "[$0] install-config: copying theme" >&2
cp themes/$1.conf $INSTALL_DIR/nimped++/color.conf 2> /dev/null
if [ $? -ne 0 ]
then
  echo "[$0] install-config: failed to copy theme!" >&2
  exit 1
fi

echo "[$0] install-config: finished successfully" >&2
