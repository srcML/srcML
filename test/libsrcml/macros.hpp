#ifndef INCLUDED_MACROS_HPP
#define INCLUDED_MACROS_HPP

#ifdef _MSC_BUILD
#include <io.h>
#endif

#ifndef _MSC_BUILD

#define OPEN(FILE, ACCESS, PERM) open(FILE, ACCESS, PERM)
#define CLOSE(FILE) close(FILE)
#define READ(FILE, BUF, NUM) read(FILE, BUF, NUM)
#define WRITE(FILE, BUF, NUM) write(FILE, BUF, NUM)

#define UNLINK(FILE) unlink(FILE)

#define UNUSED __attribute__ ((unused))

#else

#define S_IRUSR _S_IREAD
#define S_IWUSR _S_IWRITE
#define OPEN(FILE, ACCESS, PERM) _open(FILE, ACCESS, PERM)
#define CLOSE(FILE) _close(FILE)
#define READ(FILE, BUF, NUM) _read(FILE, BUF, NUM)
#define WRITE(FILE, BUF, NUM) _write(FILE, BUF, NUM)

#define UNLINK(FILE) _unlink(FILE)

#define UNUSED

#endif

#endif
