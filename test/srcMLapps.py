from ctypes import *

# language constants
LANGUAGE_NONE = ("", 64)
LANGUAGE_C = ("C", 1)
LANGUAGE_CS = ("C#", 32)
LANGUAGE_CXX = ("C++", 2)
LANGUAGE_CXX_11 = ("C++11", 4)
LANGUAGE_JAVA = ("Java", 8)
LANGUAGE_ASPECTJ = ("AspectJ", 16)
LANGUAGE_CSHARP = ("C#", 32)

# parsing options
OPTION_ARCHIVE         = 1 << 0
OPTION_LITERAL         = 1 << 1
OPTION_MODIFIER        = 1 << 2
OPTION_OPERATOR        = 1 << 3
OPTION_DEBUG           = 1 << 4
OPTION_CPP             = 1 << 7
OPTION_CPP_TEXT_ELSE   = 1 << 10
OPTION_XPATH_TOTAL     = OPTION_CPP_TEXT_ELSE
OPTION_CPP_MARKUP_IF0  = 1 << 11
OPTION_EXPRESSION      = 1 << 29

# default uri
URI_TYPE = c_char_p * 8
URI_PREFIX = URI_TYPE("", "cpp", "err", "lit", "op", "type", "pos", None )
