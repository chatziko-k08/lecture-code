#!/bin/bash

# Helper script for executing programs in tasks/debugger with flexibility for
# passing arguments, input/output redirection, etc.

if [ -z "$CODE_DEBUG" ]
then
	echo " *  Executing: $CODE_RUN"
	echo

	# we use "eval" to allow for input/output redirections
	cd "$CODE_DIR"
	eval $CODE_RUN
else
	echo " *  Executing: $CODE_DEBUG"
	echo

	# When debugging we exec in the same process being debugged
	eval "exec $CODE_DEBUG"
fi

