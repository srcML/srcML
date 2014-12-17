#!/usr/bin/python
#
# @file srcml_xpath.c
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


# XPath usage.


import srcml

if __name__ == "__main__":

    iarchive = srcml.create_archive()
    srcml.read_open_filename(iarchive, "project.xml")
    oarchive = srcml.clone_archive(iarchive)
    oarchive = None
    srcml.write_open_filename(oarchive, "xpath.xml")

    srcml.append_transform_xpath(iarchive, "//src:unit")

    srcml.apply_transforms(iarchive, oarchive)

    srcml.close_archive(iarchive)
    srcml.close_archive(oarchive)

    srcml.free_archive(iarchive)
    srcml.free_archive(oarchive)
