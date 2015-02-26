##
# @file archive_macros.py
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

from bindings import *

class archive_macros(object):
    """
    The archive_macros class contains a collection of all macros registered
    with either a readable or writable archive.
    """

    def __init__(self, srcml_archive_ptr):
        self.srcml_archive = srcml_archive_ptr

    def __len__(self):
        """
        Returns the number of macros registered with the current archive.
        """
        return archive_get_macro_list_size(self.srcml_archive)

    def __iter__(self):
        """
        Iterate through the set of macros associated with the current archive.
        """
        for x in range(len(self)):
            yield self[x]

    def __getitem__(self, index):
        """
        Returns a macro at the provided index.
        The returned value is a tuple of token and type.
        """
        if index is None or not isinstance(index, (int, long)):
            raise TypeError("Incorrect type for index. index must be an int or long")
        if index < 0 or index >= len(self):
            raise IndexError("Invalid index", index)
        return (archive_get_macro_token(self.srcml_archive, index), archive_get_macro_type(self.srcml_archive, index))

        # raise NotImplementedError()

    def token_to_type(self, token):
        """
        Locate the type associated with a token.
        """
        if token is None or not isinstance(token,str):
            raise TypeError("Token must not be None and must be a string")
        ret = archive_get_macro_token_type(self.srcml_archive, token)
        if ret == None:
            raise KeyError("Invalid token. Token isn't registered as a macro.", token)
        return ret

    def __contains__(self, macro_token_and_type):
        """
        Checks to see if a token and type pair exists within
        set of macros.
        """
        return self.contains_macro_pair(macro_token_and_type)


    def contains_token(self, other):
        """
        Checks to see if a token is registered as a macro.
        """
        if other == None:
            return False
        return len([x for x in self if x[0] == other]) > 0

    def contains_type(self, other):
        """
        Check to see if a type is used associated with any macros.
        """
        if other == None:
            return False
        return len([x for x in self if x[1] == other]) > 0

    def contains_macro_pair(self, macro_token_and_type):
        """
        Checks to see if a token and type pair exists within
        set of macros.
        """
        if macro_token_and_type == None:
            return False
        return len([x for x in self if x[0] ==macro_token_and_type[0] and x[1] == macro_token_and_type[1]]) > 0