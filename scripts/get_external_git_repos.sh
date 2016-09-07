#!/usr/bin/env bash
################################################################################
#
# Title       :    get_external_git_repos.sh
#
# License:
#
# GPL
# (c) 2015-2016, thorsten.johannvorderbrueggen@t-online.de
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
################################################################################
#
# Date/Beginn :    06.09.2016/15.08.2015
#
# Version     :    V3.00
#
# Milestones  :    V3.00 (sep 2016) -> takeover for tt_rt_env.git
#                  V2.00 (jul 2016) -> takeover for can_lin_env.git
#                                      cleanups/removes
#                  V1.01 (jul 2016) -> change exit code to 3
#                                      some minor fixes/improvements
#                  V1.00 (jul 2016) -> bump version
#                  V0.24 (jul 2016) -> some minor improvements
#                  V0.23 (may 2016) -> add time_triggert_env
#                  V0.22 (may 2016) -> add libbaalue and baalued
#                  V0.21 (apr 2016) -> add mydriver because of the examples
#                  V0.20 (apr 2016) -> some more cleanups of unused repos
#                                      create $ARMHF_BIN_HOME/* if it not exist
#                                      add https://github.com/tjohann/lcd160x_driver.git
#                  V0.19 (apr 2016) -> some cleanups of unused repos
#                  V0.18 (mar 2016) -> add a20_sdk
#                  V0.17 (mar 2016) -> add missing check for dir
#                  V0.16 (feb 2016) -> finalize new architecture
#                  V0.15 (feb 2016) -> fix a20_sdk_builder
#                  V0.14 (jan 2016) -> implement new architecture
#                  V0.13 (jan 2016) -> add a20_sdk_builder
#                  V0.12 (jan 2016) -> adapt it for usage within a20_sdk
#                  V0.11 (jan 2016) -> add a driver
#                  V0.10 (dez 2015) -> remove baalued and libbalue
#                  V0.09 (nov 2015) -> add led_dot_matrix_clock (see also
#                                      $ARMEL_HOME/projects/led_dot_clock)
#                  V0.08 (nov 2015) -> rebase for arm926_sdk
#                                      some cleanups
#                  V0.07 (sep 2015) -> add linux-lin driver repos
#                  V0.06 (sep 2015) -> add our baalue repos
#                  V0.05 (aug 2015) -> add can4linux svn repot
#                  V0.04 (aug 2015) -> add erika svn repo
#                                      remove ipipe and xenomai (we wont use it)
#                  V0.03 (aug 2015) -> add jailhouse and allwinner docs
#                  V0.02 (aug 2015) -> add void repo
#                  V0.01 (aug 2015) -> first functional version
#
# Requires    :    ...
#
#
################################################################################
# Description
#
#   A simple tool to get externel git repos ...
#
# Some features
#   - clone repo with all 3 possible network protocols
#
# Notes
#   - ...
#
################################################################################
#

# VERSION-NUMBER
VER='3.00'

REPO='none'

# PROTOCOL
# git -> git
# http -> http
# https -> https
PROTOCOL='none'

# REPO_NAME
# {$PROTOCOL$get_repo_name()}
REPO_NAME='none'

# my usage method
my_usage()
{
    echo " "
    echo "+--------------------------------------------------------+"
    echo "| Usage: ./get_external_git_repos.sh                     |"
    echo "|        [-r REPO] -> name of the sdk                    |"
    echo "|        [-p PROTOCOL] -> git/http/https                 |"
    echo "|        [-v] -> print version info                      |"
    echo "|        [-h] -> this help                               |"
    echo "|                                                        |"
    echo "| Example:                                               |"
    echo "| get_external_git_repos.sh -r can-utils -p http         |"
    echo "|                                                        |"
    echo "| Valid repo names:                                      |"
    echo "| REPO: rt-app    -> rt-app                              |"
    echo "| REPO: rt-tests  -> rt-tests                            |"
    echo "| REPO: schedtool -> schedtool                           |"
    echo "|                                                        |"
    echo "| Valid network protocols:                               |"
    echo "| PROTOCOL: none or empty -> use the simple git          |"
    echo "| PROTOCOL: git                                          |"
    echo "| PROTOCOL: http                                         |"
    echo "| PROTOCOL: https                                        |"
    echo "+--------------------------------------------------------+"
    echo " "
    exit
}

# my cleanup
cleanup() {
   rm $_temp 2>/dev/null
   rm $_log 2>/dev/null
}

# my exit method
my_exit()
{
    echo "+-----------------------------------+"
    echo "|          Cheers $USER            |"
    echo "+-----------------------------------+"
    cleanup
    # http://tldp.org/LDP/abs/html/exitcodes.html
    exit 3
}

# print version info
print_version()
{
    echo "+-----------------------------------+"
    echo "| You are using version: ${VER}       |"
    echo "+-----------------------------------+"
    cleanup
    exit
}

# ---- Some values for internal use ----
_temp="/tmp/get_external_git_repos.$$"
_log="/tmp/get_external_git_repos.log"


# check the args
while getopts 'hvr:p:' opts 2>$_log
do
    case $opts in
        h) my_usage ;;
	v) print_version ;;
	r) REPO=$OPTARG ;;
	p) PROTOCOL=$OPTARG ;;
        ?) my_usage ;;
    esac
done


# ******************************************************************************
# ***                     the functions for main_menu                        ***
# ******************************************************************************

# --- set repo names
set_repo_names()
{
    schedtool="://github.com/scheduler-tools/schedtool-dl.git"
    rt_app="://github.com/scheduler-tools/rt-app.git"
    rt_tests="://git.kernel.org/pub/scm/linux/kernel/git/clrkwllms/rt-tests.git"

    # array with all available repos
    repo_names_array[0]=${schedtool}
    repo_names_array[1]=${rt_app}
    repo_names_array[1]=${rt_tests}
}

# --- get repo name
get_repo_name()
{
    case "$REPO" in
	'schedtool')
	    REPO_NAME="${PROTOCOL}${schedtool}"
	    ;;
	'rt-app')
	    REPO_NAME="${PROTOCOL}${rt_app}"
	    ;;
	'rt-tests')
	    REPO_NAME="${PROTOCOL}${rt_tests}"
	    ;;
	*)
	    echo "ERROR -> ${REPO} is no valid repo ... pls check"
	    my_usage
    esac
}

# --- get repo name
check_protocol()
{
    PROTOCOL_VALID='false'

    if [ $PROTOCOL = 'git' -o $PROTOCOL = 'GIT' ]; then
	PROTOCOL='git'
	PROTOCOL_VALID='true'
    fi

    if [ $PROTOCOL = 'http' -o $PROTOCOL = 'HTTP' ]; then
	PROTOCOL='http'
	PROTOCOL_VALID='true'
    fi

    if [ $PROTOCOL = 'https' -o $PROTOCOL = 'HTTPS' ]; then
	PROTOCOL='https'
	PROTOCOL_VALID='true'
    fi

    if [ $PROTOCOL_VALID = 'false' ]; then
	echo "ERROR -> ${PROTOCOL} is no valid network protocol ... pls check"
	my_usage
    fi
}

# --- clone the repo
clone_repo()
{
    echo "start to clone repo $REPO_NAME"
    git clone $REPO_NAME
    if [ $? -ne 0 ] ; then
	echo "ERROR: could not clone ${REPO_NAME}"
	# we do not want to exit
    fi
}

# --- clone all repos
clone_all_repos()
{
    for item in ${repo_names_array[*]}
    do
	REPO_NAME="${PROTOCOL}${item}"
	clone_repo
    done
}


# ******************************************************************************
# ***                         Main Loop                                      ***
# ******************************************************************************

echo " "
echo "+--------------------------------------------------------+"
echo "|                 get external git repos                 |"
echo "+--------------------------------------------------------+"
echo " "

if [ -d ./external ]; then
    cd ./external
else
    echo "./external not available "
    my_exit
fi
set_repo_names

if [ $PROTOCOL = 'none' ]; then
    echo "PROTOCOL == none -> using git"
    PROTOCOL='git'
else
    check_protocol
fi

if [ $REPO = 'none' ]; then
    echo "REPO == none -> clone all repos"
    REPO_NAME='all'
    clone_all_repos
else
    echo "will clone ${REPO}"
    get_repo_name
    clone_repo
fi

cleanup
echo " "
echo "+------------------------------------------+"
echo "|             Cheers $USER                  "
echo "+------------------------------------------+"
echo " "
