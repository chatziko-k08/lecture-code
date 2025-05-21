#!/bin/bash

# Helper script for executing programs in tasks/debugger with flexibility for
# passing arguments, input/output redirection, etc.

if [ -z "$CODE_DEBUG" ]
then
	echo " *  Executing: $CODE_RUN"
	echo

	# If the child process launched by eval $CODE_RUN below	segfaults, bash by
	# default prints the current script's (run.sh) full path and line number,
	# before the "Segmentation fault" message. We can make the error message cleaner
	# by using the "trap" below.
	#
	# NOTE1: the trap handler is not really executed since the segfault happens
	# in a child process. However the presence of the trap somehow causes bash
	# not to print the script name, so the message becomes cleaner. We still
	# echo a relevant message, just in case the trap does run on some system.
	#
	# NOTE2: SIGABRT does the same for asserts
	#
	trap 'echo "Segmentation fault"' SIGSEGV SIGABRT

	# we use "eval" to allow for input/output redirections
	eval $CODE_RUN

else
	echo " *  Executing: $CODE_DEBUG"
	echo

	# When debugging we exec in the same process being debugged
	eval "exec $CODE_DEBUG"
fi

