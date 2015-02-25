##
# @file file_cleanup.py
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

import os

def cleanup_files(*files_to_cleanup):
    """
    Deletes a file after a test completes. Failure or no failure.
    """
    def cleanup_func(func):

        def delete_files():
            for f in files_to_cleanup:
                if os.path.exists(f):
                    os.remove(f)

        def make_call(*args):
            delete_files()
            try:
                func(*args)
            except:
                delete_files()
                raise
            delete_files()
        return make_call
    return cleanup_func