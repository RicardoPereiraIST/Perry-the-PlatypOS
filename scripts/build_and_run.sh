#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd)"

if [[ $1 == "clean" ]]; then
	$DIR/clean.sh
fi
if [[ $1 == "install" ]]; then
	$DIR/clean.sh
	$DIR/headers.sh
fi
if [[ $1 == "iso" ]]; then
	$DIR/clean.sh
	$DIR/headers.sh
	$DIR/iso.sh
fi
if [[ $1 == "run" ]]; then
	$DIR/clean.sh
	$DIR/headers.sh
	$DIR/iso.sh
	$DIR/qemu.sh
fi
if [[ $1 == "debug" ]]; then
	$DIR/clean.sh
	$DIR/headers.sh
	$DIR/iso.sh
	$DIR/qemu.sh debug
fi
