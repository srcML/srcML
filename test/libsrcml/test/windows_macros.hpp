#ifndef INCLUDED_WINDOWS_MACROS_HPP
#define INCLUDED_WINDOWS_MACROS_HPP

#define open _open
#define read _read
#define write _write
#define close(A) _close(A)
#define unlink _unlink
#define strdup _strdup

#endif