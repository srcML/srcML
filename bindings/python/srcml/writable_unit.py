##
# @file writable_unit.py
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


class writable_unit(object):
    """
    This class represents a unit within an archive containing source code that
    has been processed using srcml.
    """

    def __init__(self, unit_ptr, language=None, src_encoding=None, filename=None, directory=None, version=None, timestamp=None, hash=None):
        """
        Parameter descriptions:

        language - The programming language associated with the source code that's
            going to be writing into the unit. The language can be inferred when using
            reading from a filename or if the language is set on the archive that was
            used to create the unit. The language, is written into the unit even if
            it's set to None. Not setting the language prior to calling parse
            on a unit, where the language cannot be inferred, results in a
            language_not_set exception.

        src_encoding - The encoding to expected when reading source code. If not
            set the default encoding used is Latin-1.

        filename - A filename attribute associated with then XML of the unit. If None 
            it's not output onto the current unit.

        directory - A directory attribute associated with the XML of the unit. If None 
            it's not output onto the current unit.

        version - A version attribute associated with the XML of the unit. If None 
            it's not output onto the current unit.

        timestamp - A timestamp attribute to associated with the unit. If None
            it's not output into the archive.

        hash_of_unit - The hash of a unit is set by default so typically there is no
            need to set it manually. To disable the setting of the hash of a unit
            the OPTION_HASH must be disabled within the archive.

        """
        raise NotImplementedError()
