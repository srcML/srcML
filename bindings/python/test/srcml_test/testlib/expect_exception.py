##
# @file expect_exception.py
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

def expect_exception(exception_type):
    """
    This is used as a decorator of a test that check if a particular type of
    exception was thrown. If no exception is throw the test is failed in by the
    decorator but if the test raises an exception of the incorrect type 
    that exception is raised again so that the error is visible.
    """
    def exception_test(func):
        def instance_extraction(self):
            try:
                func(self)
                self.assertTrue(False, "Didn't catch expected exception from function: {0}".format(func.__name__))
            except Exception as e:
                if not isinstance(e, exception_type):
                    raise
                self.assertTrue(isinstance(e, exception_type), "Incorrect exception returned. Expected type: {0}. Actual Type: {1}".format(exception_type.__name__, e.__class__.__name__))
        return instance_extraction
    return exception_test