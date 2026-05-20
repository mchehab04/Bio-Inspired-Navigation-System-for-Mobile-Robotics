##
# \brief: General script use to test if programm continu to compile. Warning this function is local to execution path
# \params: 
#   - project: path of target script without extension
# \author: Sylvain Colomer
##

#!/bin/bash

project=joystick
script_folder=$(dirname $0)

# Test of .symb to .script conversion
if [ -f "$project.symb" ]
then
	lpreprocess.sh $script_folder/../$project.symb $script_folder/../$project.var $script_folder/../$project.script> /dev/null 2>&1
	if [ $? != 0 ]; then
		exit 1
	fi
fi
exit 0

