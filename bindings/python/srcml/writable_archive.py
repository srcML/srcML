##
# @file writable_archive.py
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

class writable_archive:
    def __init__(self, settings):
        pass

# interface WritableArchive
#     String encoding()
#     String srcEncoding()
#     String language()
#     String fileName()
#     String directory()
#     String version()
#     String revision()
#     int tabStop()
#     long options()
#     void setOptions(long newOptionSet)
#     void enableOptions(long optionsToEnable)
#     void disableOptions(long optionsToDisable)
#     ProcessingInstruction processingInstruction()
#     XmlNamespaceProxy xmlNamespaces()
#     MacroProxy macros()
#     void registerFileExtension(String fileExtension, Language lang)
#     void registerFileExtension(String fileExtension, String lang)
#     String getLanguageForExtension(String fileName)
#     WritableUnit createUnit()
#     void write(ReadableUnit unit)
#     void write(WritableUnit unit)
