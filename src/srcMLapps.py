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
OPTION_DEBUG           = c_ulonglong(1 << 0)
OPTION_NESTED          = c_ulonglong(1 << 1)
OPTION_LITERAL         = c_ulonglong(1 << 2)
OPTION_CPP_MARKUP_ELSE = c_ulonglong(1 << 22)
OPTION_XPATH_TOTAL     = OPTION_CPP_MARKUP_ELSE
OPTION_CPP_MARKUP_IF0  = c_ulonglong(1 << 23)
OPTION_EXPRESSION      = c_ulonglong(1 << 25)
OPTION_CPP             = c_ulonglong(1 << 28)
OPTION_OPERATOR        = c_ulonglong(1 << 16)
OPTION_MODIFIER        = c_ulonglong(1 << 17)

# default uri
URI_TYPE = c_char_p * 8
URI_PREFIX = URI_TYPE("", "cpp", "err", "lit", "op", "type", "pos", None )
