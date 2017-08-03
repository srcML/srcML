#
# CMakeLists.txt
# @copyright
# 
# Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

# For final installation into /usr/local/lib, libsrcml.dylib requires a non-relative pathname inside the library
# If not, then the python bindings will not work. This is due to security features.
if(APPLE)
    install(CODE "execute_process(COMMAND /usr/bin/install_name_tool -id /usr/local/lib/libsrcml.dylib /usr/local/lib/libsrcml.dylib)")
#    install(CODE "execute_process(COMMAND otool -L /usr/local/lib/libsrcml.dylib)")
endif()
