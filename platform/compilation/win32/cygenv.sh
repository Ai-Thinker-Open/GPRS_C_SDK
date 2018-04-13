#!/bin/bash -f

# the following environment variable must be defined before sourcing this file
# PROJ_ROOT

# Project name
export PROJ=$1

export PROJ_WORKDIR=$PROJ_ROOT/$PROJ

# Soft directory location
export SOFT_WORKDIR=$PROJ_WORKDIR/soft

# SVN server database location
export SVN_SOFT_REPOS=http://subversion-server/svn/developing

# Soft environment setup
if [ -f ${SOFT_WORKDIR}/env/set_env.sh ]; then
	source ${SOFT_WORKDIR}/env/set_env.sh
fi

cd $SOFT_WORKDIR
echo Project Switched to $PROJ


