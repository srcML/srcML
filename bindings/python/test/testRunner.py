##
# @file testrunner.py
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

import unittest
import sys, pdb
import signal, os

# def handler(signum, frame):
#     # print 'Signal handler called with signal', signum
#     raise Exception("FUCK!")

# Set the signal handler and a 5-second alarm
# signal.signal(signal.SIGTERM, handler)
# signal.signal(signal.SIGABRT, handler)
# signal.signal(signal.SIGKILL, handler)
# signal.alarm(5)

# # This open() may hang indefinitely
# fd = os.open('/dev/ttyS0', os.O_RDWR)

# signal.alarm(0)          # Disable the alarm
sys.path.append("../")
from srcml_test import *

if __name__ == "__main__":
    unittest.main()
