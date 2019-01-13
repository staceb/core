#!/bin/bash

#
#	MetaCall Build Bash Script by Parra Studios
#	Build and install bash script utility for MetaCall.
#
#	Copyright (C) 2016 - 2019 Vicente Eduardo Ferrer Garcia <vic798@gmail.com>
#
#	Licensed under the Apache License, Version 2.0 (the "License");
#	you may not use this file except in compliance with the License.
#	You may obtain a copy of the License at
#
#		http://www.apache.org/licenses/LICENSE-2.0
#
#	Unless required by applicable law or agreed to in writing, software
#	distributed under the License is distributed on an "AS IS" BASIS,
#	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#	See the License for the specific language governing permissions and
#	limitations under the License.
#

RUN_AS_ROOT=0
SUDO_CMD=sudo
BUILD_TYPE=Release
BUILD_TESTS=0
BUILD_INSTALL=0

sub_options() {
	for option in "$@"
	do
		if [ "$option" = 'root' ]; then
			echo "Running build script as root"
			RUN_AS_ROOT=1
			SUDO_CMD=""
		fi
		if [ "$option" = 'debug' ]; then
			echo "Build all scripts in debug mode"
			BUILD_TYPE=Debug
		fi
		if [ "$option" = 'release' ]; then
			echo "Build all scripts in release mode"
			BUILD_TYPE=Release
		fi
		if [ "$option" = 'relwithdebinfo' ]; then
			echo "Build all scripts in release mode with debug symbols"
			BUILD_TYPE=RelWithDebInfo
		fi
		if [ "$option" = 'tests' ]; then
			echo "Build and run all tests"
			BUILD_TESTS=1
		fi
		if [ "$option" = 'install' ]; then
			echo "Install all libraries"
			BUILD_INSTALL=1
		fi
	done
}

sub_build() {

	# Make without distributable
	make -j$(getconf _NPROCESSORS_ONLN)

	# Tests
	if [ $BUILD_TESTS = 1 ]; then
		ctest -VV -C $BUILD_TYPE
	fi

	# Install
	if [ $BUILD_INSTALL = 1 ]; then
		$SUDO_CMD make install
	fi
}

sub_help() {
	echo "Usage: `basename "$0"` list of options"
	echo "Options:"
	echo "	root: build being run by root"
	echo "	debug | release | relwithdebinfo: build type"
	echo "	tests: build and run all tests"
	echo "	install: install all libraries"
	echo ""
}

case "$#" in
	0)
		sub_help
		;;
	*)
		sub_options $@
		sub_build
		;;
esac
