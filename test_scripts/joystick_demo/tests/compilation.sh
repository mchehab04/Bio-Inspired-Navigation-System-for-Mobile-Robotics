##
# \brief: General script use to test if programm continu to compile. Warning this function is local to execution path
# \params: 
#   - project: path of target script without extension
# \author: Sylvain Colomer
##

#!/bin/bash

project=joystick
script_folder=$(dirname $0)

# Test of compilation
echo 1 |cc_leto $script_folder/../$project.script $script_folder/../$project.res> /dev/null 2>&1
if [ $? != 0 ]; then
	exit 1
fi
exit 0

