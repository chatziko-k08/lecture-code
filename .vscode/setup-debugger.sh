#!/bin/bash

# We start debugging via bash which then execs the debugged process (see launch.json).
# This is convenient for setting arguments and redirections, but has 2 issues:
#
# 1. macOS/arm does not allow us to debug /bin/bash
# 2. We get a debugger warning that /bin/bash is older than the source files.
#
# To workaround these issues we run this script before debugging which creates
# a .vscode/bash symlink. This way:
# 1. On macOS we can use homebrew's bash instead of the system bash
#    (under /opt/homebrew/bin for arm, /usr/local/bin for intel).
# 2. The symlink has newer timestamp so we avoid the warning.

for path in /opt/homebrew/bin /usr/local/bin /bin
do
	if [ -x $path/bash ]
	then
		ln -sf $path/bash ./bash
		exit 0
	fi
done

echo Bash not found
exit 1

