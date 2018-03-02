##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

copyDependentFile("runtest.py")

add_subdirectory(testsuite)

add_custom_target(gentest COMMAND cd testsuite && make -f Makefile.gen)
add_custom_target(cleantest COMMAND cd testsuite && make -f Makefile.gen clean)

# Collection of targest to manage parser testing, and parser test reports
add_custom_target(run COMMAND time ./runtest.py)
add_custom_target(c COMMAND time ./runtest.py C)
add_custom_target(java COMMAND time ./runtest.py Java)
add_custom_target(cpp COMMAND time ./runtest.py C++)
add_custom_target(cs COMMAND time ./runtest.py CS)
add_custom_target(cleanreport COMMAND rm srcMLTestReport*.txt)
add_custom_target(purge COMMAND ls -t srcMLTestReport*.txt | sed '1d' | xargs rm)
add_custom_target(report COMMAND ls -t srcMLTestReport*.txt | head -1 | xargs cat)
