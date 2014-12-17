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

add_executable(timing_driver timing_driver.cpp ${Boost_LIBRARIES})
set_target_properties(timing_driver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

addTimingTest(C timing_long.h timing_log.txt)
addTimingTest(Cpp timing_long.hpp timing_log.txt)
addTimingTest(Csharp timing_csharp.cs timing_log.txt)
addTimingTest(Java timing_java.java timing_log.txt)
