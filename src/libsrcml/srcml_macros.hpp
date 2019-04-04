#ifndef INCLUDED_SRCML_MACROS_HPP
#define INCLUDED_SRCML_MACROS_HPP

#ifdef _MSC_BUILD
#include <io.h>
#endif

/**
 * isoption
 * @param options the set options
 * @param flag options to query if set
 *
 * Predicate to query if options flag are set in options.
 *
 * @returns if options flag are set in options.
 */
#define isoption(options, flag) (((options) & (flag)) > 0)

/**
 * optional_string_create
 * @param c_str const char * to convert to optional<std::string>
 *
 * Create a optional<std::string> from const char *.
 *
 * @returns the new optional<std::string>
 */
#define optional_string_create(c_str) (c_str ? boost::optional<std::string>(std::string(c_str)) : boost::optional<std::string>())

 /**
 * optional_get_c_str
 * @param str optional<std;:string> to extract c string.
 *
 * Get c string from optional<std::string>
 *
 * @returns the c string
 */
#define optional_get_c_str(str) (str ? str->c_str() : 0)

#ifndef _MSC_BUILD

#define OPEN(FILE, ACCESS, PERM) open(FILE, ACCESS, PERM)
#define CLOSE(FILE) close(FILE)
#define READ(FILE, BUF, NUM) read(FILE, BUF, NUM)
#define WRITE(FILE, BUF, NUM) write(FILE, BUF, NUM)

#define UNLINK(FILE) unlink(FILE)

#define MKTEMP mktemp

#define UNUSED __attribute__ ((unused))

#else

#define S_IRUSR _S_IREAD
#define S_IWUSR _S_IWRITE
#define OPEN(FILE, ACCESS, PERM) _open(FILE, ACCESS, PERM)
#define CLOSE(FILE) _close(FILE)
#define READ(FILE, BUF, NUM) _read(FILE, BUF, NUM)
#define WRITE(FILE, BUF, NUM) _write(FILE, BUF, NUM)

#define UNLINK(FILE) _unlink(FILE)

#define MKTEMP _mktemp

#define UNUSED

#endif

#endif
