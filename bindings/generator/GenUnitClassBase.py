##
# @file GenUnitClassBase.py
#
# @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
#
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# /* Query options of srcml unit */
# __LIBSRCML_DECL const char* srcml_unit_get_encoding (const struct srcml_unit*);
# __LIBSRCML_DECL const char* srcml_unit_get_revision (const struct srcml_unit*);
# __LIBSRCML_DECL const char* srcml_unit_get_language (const struct srcml_unit*);
# __LIBSRCML_DECL const char* srcml_unit_get_filename (const struct srcml_unit*);
# __LIBSRCML_DECL const char* srcml_unit_get_directory(const struct srcml_unit*);
# __LIBSRCML_DECL const char* srcml_unit_get_version  (const struct srcml_unit*);
# __LIBSRCML_DECL const char* srcml_unit_get_timestamp(const struct srcml_unit*);
# __LIBSRCML_DECL const char* srcml_unit_get_hash     (const struct srcml_unit*);
# __LIBSRCML_DECL const char* srcml_unit_get_xml      (struct srcml_unit*);

# /* Convert from srcML to source code */
# __LIBSRCML_DECL int srcml_unparse_unit_filename(struct srcml_unit*, const char* src_filename);
# __LIBSRCML_DECL int srcml_unparse_unit_memory  (struct srcml_unit*, char** src_buffer, int * src_size);
# __LIBSRCML_DECL int srcml_unparse_unit_FILE    (struct srcml_unit*, FILE* srcml_file);
# __LIBSRCML_DECL int srcml_unparse_unit_fd      (struct srcml_unit*, int srcml_fd);
# __LIBSRCML_DECL int srcml_unparse_unit_io      (struct srcml_unit*, void * context, int (*write_callback)(void * context, const char * buffer, int len), int (*close_callback)(void * context));

# /* Setup options for srcml unit */
# __LIBSRCML_DECL int srcml_unit_set_encoding (struct srcml_unit*, const char* language);
# __LIBSRCML_DECL int srcml_unit_set_language (struct srcml_unit*, const char* language);
# __LIBSRCML_DECL int srcml_unit_set_filename (struct srcml_unit*, const char* filename);
# __LIBSRCML_DECL int srcml_unit_set_directory(struct srcml_unit*, const char* directory);
# __LIBSRCML_DECL int srcml_unit_set_version  (struct srcml_unit*, const char* version);
# __LIBSRCML_DECL int srcml_unit_set_timestamp(struct srcml_unit*, const char* timestamp);
# __LIBSRCML_DECL int srcml_unit_set_hash     (struct srcml_unit*, const char* hash);

# /* Convert to srcml.  Files/buffer can be compressed, but not a
#    source archive format (e.g., not .tar) */
# __LIBSRCML_DECL int srcml_parse_unit_filename(struct srcml_unit* unit, const char* src_filename);
# __LIBSRCML_DECL int srcml_parse_unit_memory  (struct srcml_unit*, const char* src_buffer, size_t buffer_size);
# __LIBSRCML_DECL int srcml_parse_unit_FILE    (struct srcml_unit*, FILE* src_file);
# __LIBSRCML_DECL int srcml_parse_unit_fd      (struct srcml_unit*, int src_fd);
# __LIBSRCML_DECL int srcml_parse_unit_io      (struct srcml_unit*, void * context, int (*read_callback)(void * context, char * buffer, int len), int (*close_callback)(void * context));
