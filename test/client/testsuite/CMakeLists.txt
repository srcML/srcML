##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2014-2014 srcML, LLC. (www.srcML.org)
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

# add cli tests
file(GLOB CLI_TESTS *.sh)
file(GLOB IGNORE framework_test.sh)
list(REMOVE_ITEM CLI_TESTS ${IGNORE})

foreach(CLI_TEST ${CLI_TESTS})
  addBashTest(${CLI_TEST} framework_test.sh)
endforeach()
