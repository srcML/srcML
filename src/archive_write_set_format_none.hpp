#ifndef ARCHIVE_WRITE_SET_FORMAT_NONE_H
#define ARCHIVE_WRITE_SET_FORMAT_NONE_H

#include <sys/stat.h>
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static ssize_t archive_write_none_data(struct archive_write *a, const void *buff,
				       size_t s);
static int archive_write_none_free(struct archive_write *);
static int archive_write_none_close(struct archive_write *);
static int archive_write_none_finish_entry(struct archive_write *);
static int archive_write_none_header(struct archive_write *,

#endif

