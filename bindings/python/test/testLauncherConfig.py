##
# @file testLauncherConfg.py
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

"""
This file is a configuration file that generates another file that can be used to run the
test suite for the python bindings.
"""

import os, subprocess, sys

if "LD_LIBRARY_PATH" not in os.environ:
    os.environ["LD_LIBRARY_PATH"] = "${SHARED_LIBSRCML_OUTPUT_PATH}"
else:
    os.environ["LD_LIBRARY_PATH"] = os.environ['LD_LIBRARY_PATH'] + ";${SHARED_LIBSRCML_OUTPUT_PATH}"

proc = subprocess.Popen(["${PYTHON_EXECUTABLE}", "${PY_TEST_SOURCE_LOCATION}"])
proc.wait()
