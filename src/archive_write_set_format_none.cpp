#include "archive_write_format_set_non.h"
#include <stdio.h>
#include <string.h>
#include "archive_platform.h"
#include <archive.h>
#include <archive_entry.h>

int ARCHIVE_FORMAT_NONE = 0x10000000

/*
 * Set output format to 'none' format.
 */
int archive_write_set_format_none(struct archive *_a)
{
  struct archive_write *a = (struct archive_write *)_a;

  archive_check_magic(_a, ARCHIVE_WRITE_MAGIC,
		      ARCHIVE_STATE_NEW, "archive_write_set_format_none");

  /* If someone else was already registered, unregister them. */
  if (a->format_free != NULL)
    (a->format_free)(a);

  a->format_data = 0;
  a->format_name = "none";
  a->format_write_header = archive_write_none_header;
  a->format_write_data = archive_write_none_data;
  a->format_close = archive_write_none_close;
  a->format_free = archive_write_none_free;
  a->format_finish_entry = archive_write_none_finish_entry;
  a->archive.archive_format = ARCHIVE_FORMAT_NONE;
  a->archive.archive_format_name = "No format";
  return (ARCHIVE_OK);
}

static int archive_write_none_header(struct archive_write *a, struct archive_entry *entry)
{
  return ARCHIVE_OK;
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
int archive_write_format_header_none(struct archive_write *a, char h[512],
				    struct archive_entry *entry, int tartype, int strict)
{
  return ARCHIVE_OK;
}

static int
archive_write_none_close(struct archive_write *a)
{
  return ARCHIVE_OK;
}

static int archive_write_none_free(struct archive_write *a)
{
  a->format_data = NULL;
  return (ARCHIVE_OK);
}

static int archive_write_none_finish_entry(struct archive_write *a)
{
  return ARCHIVE_OK;
}

static ssize_t archive_write_none_data(struct archive_write *a, const void *buff, size_t s)
{
  int ret;

  ret = __archive_write_output(a, buff, s);
  if (ret != ARCHIVE_OK)
    return (ret);
  return (s);
}
