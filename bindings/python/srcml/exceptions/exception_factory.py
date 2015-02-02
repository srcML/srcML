##
# @file exception_factory.py
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

from .. import bindings
from srcml_error import srcml_error
from srcml_invalid_argument import srcml_invalid_argument
from srcml_invalid_input import srcml_invalid_input
from srcml_invalid_io_operation import srcml_invalid_io_operation
from srcml_io_error import srcml_io_error
from srcml_uninitialized_unit import srcml_uninitialized_unit
from srcml_language_not_set import srcml_language_not_set
from srcml_no_transformation import srcml_no_transformation

def get_srcml_exception(error_code, native_function_name, message):
    """
    Handles error code results from the srcML functions and throws an exception
    that if the result wasn't acceptable.
    """
    if error_code == bindings.STATUS_OK:
        pass

    elif error_code == bindings.STATUS_ERROR:
        raise srcml_error(native_function_name, message)

    elif error_code == bindings.STATUS_INVALID_ARGUMENT:
        raise srcml_invalid_argument(native_function_name, message)

    elif error_code == bindings.STATUS_INVALID_INPUT:
        raise srcml_invalid_input(native_function_name, message)

    elif error_code == bindings.STATUS_INVALID_IO_OPERATION:
        raise srcml_invalid_io_operation(native_function_name, message)

    elif error_code == bindings.STATUS_IO_ERROR:
        raise srcml_io_error(native_function_name, message)

    elif error_code == bindings.STATUS_UNINITIALIZED_UNIT:
        raise srcml_uninitialized_unit(native_function_name, message)

    elif error_code == bindings.STATUS_UNSET_LANGUAGE:
        raise srcml_language_not_set(native_function_name, message)

    elif error_code == bindings.STATUS_NO_TRANSFORMATION:
        raise srcml_no_transformation(native_function_name, message)

    else:
        raise Exception("Unknown error code received from native function {0}. {1}.".format(native_function_name, message))