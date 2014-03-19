#ifndef INCLUDED_SRCML_MACROS_HPP
#define INCLUDED_SRCML_MACROS_HPP

#ifndef _MSC_BUILD

#define OPEN(FILE, ACCESS, PERM) open(FILE, ACCESS, PERM)
#define CLOSE(FILE) close(FILE)
#define READ(FILE, BUF, NUM) read(FILE, BUF, NUM)
#define WRITE(FILE, BUF, NUM) write(FILE, BUF, NUM)

#define UNLINK unlink

#else

#define _CRT_SECURE_NO_WARNINGS

#define OPEN(FILE, ACCESS, PERM) _open(FILE, ACCESS)
#define CLOSE(FILE) _close(FILE)
#define READ(FILE, BUF, NUM) _read(FILE, BUF, NUM)
#define WRITE(FILE, BUF, NUM) _write(FILE, BUF, NUM)

#define UNLINK(FILE) _unlink(FILE)

#endif

#endif
