##
# @file srcmlXMLHeaderGeneratorTemplate
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

srcmlHPath = "${SRCML_H_HEADER_PATH}"
generatorPath = "${GENERATOR_LIB_DIR}"
srcmlLibLocation = "${DYNAMIC_LIBSRCML_LOCATION}"
srcmlHeaderOutputPath = "${SRCML_H_XML_OUTPUT_PATH}"
windowsDepBinaryPath = "${WINDOWS_DEP_PATH}/bin"

import sys, re, os

sys.path.append(generatorPath)
from generator import *
tempFileLocation = "${CMAKE_CURRENT_BINARY_DIR}/srcml.h.temp"
inputsrcmlH = open(srcmlHPath, "r")

srcmlhFileContent = "".join(inputsrcmlH.readlines())
inputsrcmlH.close()

removeStructKw = re.compile(r"\bstruct\b", re.M)


outputStrm = open(tempFileLocation, "w")
outputStrm.write(removeStructKw.sub("", srcmlhFileContent))
outputStrm.close()

if os.name == "nt": # windows
    srcml(srcmlLibLocation, tempFileLocation, srcmlHeaderOutputPath, "temp", "C++", windowsDepBinaryPath)
else:
    srcml(srcmlLibLocation, tempFileLocation, srcmlHeaderOutputPath, "temp", "C++")

os.remove(tempFileLocation)
