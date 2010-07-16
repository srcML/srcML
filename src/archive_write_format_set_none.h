#ifndef ARCHIVE_WRITE_FORMAT_SET_NONE_H
#define ARCHIVE_WRITE_FORMAT_SET_NONE_H

static ssize_t archive_write_ustar_data(struct archive_write *a, const void *buff,
				       size_t s);
static int archive_write_ustar_free(struct archive_write *);
static int archive_write_ustar_close(struct archive_write *);
static int archive_write_ustar_finish_entry(struct archive_write *);
static int archive_write_ustar_header(struct archive_write *,
				     struct archive_entry *entry);
static int format_256(int64_t, char *, int);
static int format_number(int64_t, char *, int size, int max, int strict);
static int format_octal(int64_t, char *, int);

struct ustar {
  uint64_t entry_bytes_remaining;
  uint64_t entry_padding;
};

#endif

