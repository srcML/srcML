#include "archive_write_format_set_non.h"
#include <stdio.h>
#include <string.h>
#include "archive_platform.h"
#include <archive.h>
#include <archive_entry.h>

/*
 * Set output format to 'none' format.
 */
int
archive_write_set_format_none(struct archive *_a)
{
  struct archive_write *a = (struct archive_write *)_a;
  struct none *none;

  archive_check_magic(_a, ARCHIVE_WRITE_MAGIC,
		      ARCHIVE_STATE_NEW, "archive_write_set_format_none");

  /* If someone else was already registered, unregister them. */
  if (a->format_free != NULL)
    (a->format_free)(a);

  /* Basic internal sanity test. */
  if (sizeof(template_header) != 512) {
    archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC, "Internal: template_header wrong size: %zu should be 512", sizeof(template_header));
    return (ARCHIVE_FATAL);
  }

  none = (None *)malloc(sizeof(*none));
  if (none == NULL) {
    archive_set_error(&a->archive, ENOMEM, "Can't allocate none data");
    return (ARCHIVE_FATAL);
  }
  memset(none, 0, sizeof(*none));
  a->format_data = none;
  a->format_name = "none";
  a->format_write_header = archive_write_none_header;
  a->format_write_data = archive_write_none_data;
  a->format_close = archive_write_none_close;
  a->format_free = archive_write_none_free;
  a->format_finish_entry = archive_write_none_finish_entry;
  a->archive.archive_format = ARCHIVE_FORMAT_NONE;
  a->archive.archive_format_name = "POSIX none";
  return (ARCHIVE_OK);
}

static int
archive_write_none_header(struct archive_write *a, struct archive_entry *entry)
{
  char buff[512];
  int ret, ret2;
  None *none;

  none = (None *)a->format_data;

  /* Only regular files (not hardlinks) have data. */
  if (archive_entry_hardlink(entry) != NULL ||
      archive_entry_symlink(entry) != NULL ||
      !(archive_entry_filetype(entry) == AE_IFREG))
    archive_entry_set_size(entry, 0);

  if (AE_IFDIR == archive_entry_filetype(entry)) {
    const char *p;
    char *t;
    /*
     * Ensure a trailing '/'.  Modify the entry so
     * the client sees the change.
     */
    p = archive_entry_pathname(entry);
    if (p[strlen(p) - 1] != '/') {
      t = (char *)malloc(strlen(p) + 2);
      if (t == NULL) {
	archive_set_error(&a->archive, ENOMEM,
			  "Can't allocate none data");
	return(ARCHIVE_FATAL);
      }
      strcpy(t, p);
      strcat(t, "/");
      archive_entry_copy_pathname(entry, t);
      free(t);
    }
  }

  none->entry_bytes_remaining = archive_entry_size(entry);
  none->entry_padding = 0x1ff & (-(int64_t)none->entry_bytes_remaining);
  return (ret);
}

/*
 * Format a basic 512-byte "none" header.
 *
 * Returns -1 if format failed (due to field overflow).
 * Note that this always formats as much of the header as possible.
 * If "strict" is set to zero, it will extend numeric fields as
 * necessary (overwriting terminators or using base-256 extensions).
 *
 * This is exported so that other 'tar' formats can use it.
 */
int
__archive_write_format_header_none(struct archive_write *a, char h[512],
				    struct archive_entry *entry, int tartype, int strict)
{
  unsigned int checksum;
  int i, ret;
  size_t copy_length;
  const char *p, *pp;
  int mytartype;

  ret = 0;
  mytartype = -1;
  /*
   * The "template header" already includes the "none"
   * signature, various end-of-field markers and other required
   * elements.
   */
  memcpy(h, &template_header, 512);

  /*
   * Because the block is already null-filled, and strings
   * are allowed to exactly fill their destination (without null),
   * I use memcpy(dest, src, strlen()) here a lot to copy strings.
   */

  pp = archive_entry_pathname(entry);
  if (strlen(pp) <= NONE_name_size)
    memcpy(h + NONE_name_offset, pp, strlen(pp));
  else {
    /* Store in two pieces, splitting at a '/'. */
    p = strchr(pp + strlen(pp) - NONE_name_size - 1, '/');
    /*
     * Look for the next '/' if we chose the first character
     * as the separator.  (none format doesn't permit
     * an empty prefix.)
     */
    if (p == pp)
      p = strchr(p + 1, '/');
    /* Fail if the name won't fit. */
    if (!p) {
      /* No separator. */
      archive_set_error(&a->archive, ENAMETOOLONG,
			"Pathname too long");
      ret = ARCHIVE_FAILED;
    } else if (p[1] == '\0') {
      /*
       * The only feasible separator is a final '/';
       * this would result in a non-empty prefix and
       * an empty name, which POSIX doesn't
       * explicity forbid, but it just feels wrong.
       */
      archive_set_error(&a->archive, ENAMETOOLONG,
			"Pathname too long");
      ret = ARCHIVE_FAILED;
    } else if (p  > pp + NONE_prefix_size) {
      /* Prefix is too long. */
      archive_set_error(&a->archive, ENAMETOOLONG,
			"Pathname too long");
      ret = ARCHIVE_FAILED;
    } else {
      /* Copy prefix and remainder to appropriate places */
      memcpy(h + NONE_prefix_offset, pp, p - pp);
      memcpy(h + NONE_name_offset, p + 1, pp + strlen(pp) - p - 1);
    }
  }

  p = archive_entry_hardlink(entry);
  if (p != NULL)
    mytartype = '1';
  else
    p = archive_entry_symlink(entry);
  if (p != NULL && p[0] != '\0') {
    copy_length = strlen(p);
    if (copy_length > NONE_linkname_size) {
      archive_set_error(&a->archive, ENAMETOOLONG,
			"Link contents too long");
      ret = ARCHIVE_FAILED;
      copy_length = NONE_linkname_size;
    }
    memcpy(h + NONE_linkname_offset, p, copy_length);
  }

  p = archive_entry_uname(entry);
  if (p != NULL && p[0] != '\0') {
    copy_length = strlen(p);
    if (copy_length > NONE_uname_size) {
      archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			"Username too long");
      ret = ARCHIVE_FAILED;
      copy_length = USTAR_uname_size;
    }
    memcpy(h + NONE_uname_offset, p, copy_length);
  }

  p = archive_entry_gname(entry);
  if (p != NULL && p[0] != '\0') {
    copy_length = strlen(p);
    if (strlen(p) > NONE_gname_size) {
      archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			"Group name too long");
      ret = ARCHIVE_FAILED;
      copy_length = NONE_gname_size;
    }
    memcpy(h + NONE_gname_offset, p, copy_length);
  }

  if (format_number(archive_entry_mode(entry) & 07777, h + NONE_mode_offset, NONE_mode_size, NONE_mode_max_size, strict)) {
    archive_set_error(&a->archive, ERANGE, "Numeric mode too large");
    ret = ARCHIVE_FAILED;
  }

  if (format_number(archive_entry_uid(entry), h + NONE_uid_offset, NONE_uid_size, NONE_uid_max_size, strict)) {
    archive_set_error(&a->archive, ERANGE, "Numeric user ID too large");
    ret = ARCHIVE_FAILED;
  }

  if (format_number(archive_entry_gid(entry), h + NONE_gid_offset, NONE_gid_size, NONE_gid_max_size, strict)) {
    archive_set_error(&a->archive, ERANGE, "Numeric group ID too large");
    ret = ARCHIVE_FAILED;
  }

  if (format_number(archive_entry_size(entry), h + NONE_size_offset, NONE_size_size, NONE_size_max_size, strict)) {
    archive_set_error(&a->archive, ERANGE, "File size out of range");
    ret = ARCHIVE_FAILED;
  }

  if (format_number(archive_entry_mtime(entry), h + NONE_mtime_offset, NONE_mtime_size, NONE_mtime_max_size, strict)) {
    archive_set_error(&a->archive, ERANGE,
		      "File modification time too large");
    ret = ARCHIVE_FAILED;
  }

  if (archive_entry_filetype(entry) == AE_IFBLK
      || archive_entry_filetype(entry) == AE_IFCHR) {
    if (format_number(archive_entry_rdevmajor(entry), h + NONE_rdevmajor_offset,
		      NONE_rdevmajor_size, NONE_rdevmajor_max_size, strict)) {
      archive_set_error(&a->archive, ERANGE,
			"Major device number too large");
      ret = ARCHIVE_FAILED;
    }

    if (format_number(archive_entry_rdevminor(entry), h + NONE_rdevminor_offset,
		      NONE_rdevminor_size, NONE_rdevminor_max_size, strict)) {
      archive_set_error(&a->archive, ERANGE,
			"Minor device number too large");
      ret = ARCHIVE_FAILED;
    }
  }

  if (tartype >= 0) {
    h[NONE_typeflag_offset] = tartype;
  } else if (mytartype >= 0) {
    h[NONE_typeflag_offset] = mytartype;
  } else {
    switch (archive_entry_filetype(entry)) {
    case AE_IFREG: h[NONE_typeflag_offset] = '0' ; break;
    case AE_IFLNK: h[NONE_typeflag_offset] = '2' ; break;
    case AE_IFCHR: h[NONE_typeflag_offset] = '3' ; break;
    case AE_IFBLK: h[NONE_typeflag_offset] = '4' ; break;
    case AE_IFDIR: h[NONE_typeflag_offset] = '5' ; break;
    case AE_IFIFO: h[NONE_typeflag_offset] = '6' ; break;
    case AE_IFSOCK:
      archive_set_error(&a->archive,
			ARCHIVE_ERRNO_FILE_FORMAT,
			"tar format cannot archive socket");
      return (ARCHIVE_FAILED);
    default:
      archive_set_error(&a->archive,
			ARCHIVE_ERRNO_FILE_FORMAT,
			"tar format cannot archive this (mode=0%lo)",
			(unsigned long)archive_entry_mode(entry));
      ret = ARCHIVE_FAILED;
    }
  }

  checksum = 0;
  for (i = 0; i < 512; i++)
    checksum += 255 & (unsigned int)h[i];
  h[NONE_checksum_offset + 6] = '\0'; /* Can't be pre-set in the template. */
  /* h[NONE_checksum_offset + 7] = ' '; */ /* This is pre-set in the template. */
  format_octal(checksum, h + NONE_checksum_offset, 6);
  return (ret);
}

/*
 * Format a number into a field, with some intelligence.
 */
static int
format_number(int64_t v, char *p, int s, int maxsize, int strict)
{
  int64_t limit;

  limit = ((int64_t)1 << (s*3));

  /* "Strict" only permits octal values with proper termination. */
  if (strict)
    return (format_octal(v, p, s));

  /*
   * In non-strict mode, we allow the number to overwrite one or
   * more bytes of the field termination.  Even old tar
   * implementations should be able to handle this with no
   * problem.
   */
  if (v >= 0) {
    while (s <= maxsize) {
      if (v < limit)
	return (format_octal(v, p, s));
      s++;
      limit <<= 3;
    }
  }

  /* Base-256 can handle any number, positive or negative. */
  return (format_256(v, p, maxsize));
}

/*
 * Format a number into the specified field using base-256.
 */
static int
format_256(int64_t v, char *p, int s)
{
  p += s;
  while (s-- > 0) {
    *--p = (char)(v & 0xff);
    v >>= 8;
  }
  *p |= 0x80; /* Set the base-256 marker bit. */
  return (0);
}

/*
 * Format a number into the specified field.
 */
static int
format_octal(int64_t v, char *p, int s)
{
  int len;

  len = s;

  /* Octal values can't be negative, so use 0. */
  if (v < 0) {
    while (len-- > 0)
      *p++ = '0';
    return (-1);
  }

  p += s;/* Start at the end and work backwards. */
  while (s-- > 0) {
    *--p = (char)('0' + (v & 7));
    v >>= 3;
  }

  if (v == 0)
    return (0);

  /* If it overflowed, fill field with max value. */
  while (len-- > 0)
    *p++ = '7';

  return (-1);
}

static int
archive_write_none_close(struct archive_write *a)
{
  return (__archive_write_nulls(a, 512*2));
}

static int
archive_write_none_free(struct archive_write *a)
{
  struct none *none;

  none = (struct none *)a->format_data;
  free(none);
  a->format_data = NULL;
  return (ARCHIVE_OK);
}

static int
archive_write_none_finish_entry(struct archive_write *a)
{
  struct none *none;
  int ret;

  none = (struct none *)a->format_data;
  ret = __archive_write_nulls(a,
			      none->entry_bytes_remaining + none->entry_padding);
  none->entry_bytes_remaining = none->entry_padding = 0;
  return (ret);
}

static ssize_t
archive_write_none_data(struct archive_write *a, const void *buff, size_t s)
{
  struct none *none;
  int ret;

  none = (struct none *)a->format_data;
  if (s > none->entry_bytes_remaining)
    s = none->entry_bytes_remaining;
  ret = __archive_write_output(a, buff, s);
  none->entry_bytes_remaining -= s;
  if (ret != ARCHIVE_OK)
    return (ret);
  return (s);
}
