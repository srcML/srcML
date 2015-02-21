##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
#
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#    The CMake project file for srcML.
#

file(STRINGS large_systems_list.csv FILE)

# sets the max threads option. can be modified to match desired thread number,
# or commented out to disable
set(MAX_THREADS "--max-threads=8")

foreach(LINE ${FILE})

	# use the project name from "*,<project-name>,*" in the CSV file
	# as the name for the test
	string(REGEX MATCH ",.*," NAME "${LINE}")
	string(REPLACE "," "" NAME "${NAME}")

	addLargeSystemTest(${NAME} large_systems.sh ${MAX_THREADS} ${LINE})
endforeach()
