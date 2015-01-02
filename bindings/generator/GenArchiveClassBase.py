##
# @file GenArchiveClassBase.py
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


class GenArchiveClassBase(object):

    def __init__(self):
        self.fileName = ""

    def startFile(self):
        assert False, "Implement Me!"

    def endFile(self):
        assert False, "Implement Me!"

    def genCtors(self):
        assert False, "Implement Me!"

    def genDtor(self):
        assert False, "Implement Me!"

    def genProperty(self, propertyInfo):
        assert False, "Implement Me!"

    def genOpenWriteFunc(self, funcInfo):
        assert False, "Implement Me!"

    def genOpenReadFunc(self, funcInfo):
        assert False, "Implement Me!"

    def genCreateUnit(self):
        """ struct srcml_unit* srcml_create_unit(struct srcml_archive* archive); """
        assert False, "Implement Me!"

# __LIBSRCML_DECL const char* srcml_archive_check_extension(struct srcml_archive* archive, const char* filename);

# /* Create a new srcml archive
#    Client is responsible for freeing memory using srcml_free_archive() */
# __LIBSRCML_DECL struct srcml_archive* srcml_create_archive();

# /* Clone the setup of an existing archive
#    Client is responsible for freeing memory using srcml_free_archive() */
# __LIBSRCML_DECL struct srcml_archive* srcml_clone_archive(const struct srcml_archive*);

# /* Open a srcML archive for output */
# __LIBSRCML_DECL int srcml_write_open_filename(struct srcml_archive*, const char* srcml_filename);
# __LIBSRCML_DECL int srcml_write_open_memory  (struct srcml_archive*, char** buffer, int * size);
# __LIBSRCML_DECL int srcml_write_open_FILE    (struct srcml_archive*, FILE* srcml_file);
# __LIBSRCML_DECL int srcml_write_open_fd      (struct srcml_archive*, int srcml_fd);
# __LIBSRCML_DECL int srcml_write_open_io      (struct srcml_archive*, void * context, int (*write_callback)(void * context, const char * buffer, int len), int (*close_callback)(void * context));

# /* Setup options for srcml archive */
# __LIBSRCML_DECL int srcml_archive_set_encoding           (struct srcml_archive*, const char* encoding);
# __LIBSRCML_DECL int srcml_archive_set_src_encoding       (struct srcml_archive*, const char* encoding);
# __LIBSRCML_DECL int srcml_archive_set_language           (struct srcml_archive*, const char* language);
# __LIBSRCML_DECL int srcml_archive_set_filename           (struct srcml_archive*, const char* filename);
# __LIBSRCML_DECL int srcml_archive_set_directory          (struct srcml_archive*, const char* directory);
# __LIBSRCML_DECL int srcml_archive_set_version            (struct srcml_archive*, const char* version);

# __LIBSRCML_DECL int srcml_archive_set_options               (struct srcml_archive*, unsigned long long option);
# __LIBSRCML_DECL int srcml_archive_enable_option             (struct srcml_archive*, unsigned long long option);
# __LIBSRCML_DECL int srcml_archive_disable_option            (struct srcml_archive*, unsigned long long option);
# __LIBSRCML_DECL int srcml_archive_set_tabstop               (struct srcml_archive*, int tabstop);
# __LIBSRCML_DECL int srcml_archive_register_file_extension   (struct srcml_archive*, const char* extension, const char* language);
# __LIBSRCML_DECL int srcml_archive_register_namespace        (struct srcml_archive*, const char* prefix, const char* ns);
# __LIBSRCML_DECL int srcml_archive_set_processing_instruction(struct srcml_archive*, const char* target, const char* data); 
# __LIBSRCML_DECL int srcml_archive_register_macro            (struct srcml_archive*, const char* token, const char* type);  

# /* Query of the options for srcml archive */
# __LIBSRCML_DECL const char*        srcml_archive_get_encoding                     (const struct srcml_archive*);
# __LIBSRCML_DECL const char*        srcml_archive_get_src_encoding                 (const struct srcml_archive*);
# __LIBSRCML_DECL const char*        srcml_archive_get_revision                     (const struct srcml_archive*);
# __LIBSRCML_DECL const char*        srcml_archive_get_language                     (const struct srcml_archive*);
# __LIBSRCML_DECL const char*        srcml_archive_get_filename                     (const struct srcml_archive*);
# __LIBSRCML_DECL const char*        srcml_archive_get_directory                    (const struct srcml_archive*);
# __LIBSRCML_DECL const char*        srcml_archive_get_version                      (const struct srcml_archive*);
# __LIBSRCML_DECL unsigned long long srcml_archive_get_options                      (const struct srcml_archive*);
# __LIBSRCML_DECL int                srcml_archive_get_tabstop                      (const struct srcml_archive*);
# __LIBSRCML_DECL int                srcml_archive_get_namespace_size               (const struct srcml_archive*);
# __LIBSRCML_DECL const char*        srcml_archive_get_namespace_prefix             (const struct srcml_archive*, int pos);
# __LIBSRCML_DECL const char*        srcml_archive_get_prefix_from_uri              (const struct srcml_archive*, const char* namespace_uri);
# __LIBSRCML_DECL const char*        srcml_archive_get_namespace_uri                (const struct srcml_archive*, int pos);
# __LIBSRCML_DECL const char*        srcml_archive_get_uri_from_prefix              (const struct srcml_archive*, const char* prefix);
# __LIBSRCML_DECL const char*        srcml_archive_get_processing_instruction_target(const struct srcml_archive*); 
# __LIBSRCML_DECL const char*        srcml_archive_get_processing_instruction_data  (const struct srcml_archive*); 
# __LIBSRCML_DECL int                srcml_archive_get_macro_list_size              (const struct srcml_archive*);
# __LIBSRCML_DECL const char*        srcml_archive_get_macro_token                  (const struct srcml_archive*, int pos);
# __LIBSRCML_DECL const char*        srcml_archive_get_macro_token_type             (const struct srcml_archive*, const char* namespace_uri);
# __LIBSRCML_DECL const char*        srcml_archive_get_macro_type                   (const struct srcml_archive*, int pos);

# /* Create a new srcml unit.
#    Client is responsible for freeing memory using srcml_free_unit() */
# __LIBSRCML_DECL struct srcml_unit* srcml_create_unit(struct srcml_archive* archive);



# /* Append unit to an archive */
# __LIBSRCML_DECL int srcml_write_unit(struct srcml_archive*, const struct srcml_unit*);

# /* Iteratively build a unit */
# __LIBSRCML_DECL int srcml_write_start_unit(struct srcml_unit*);
# __LIBSRCML_DECL int srcml_write_end_unit(struct srcml_unit*);
# __LIBSRCML_DECL int srcml_write_start_element(struct srcml_unit*, const char * prefix, const char * name, const char * uri);
# __LIBSRCML_DECL int srcml_write_end_element(struct srcml_unit*);
# __LIBSRCML_DECL int srcml_write_namespace(struct srcml_unit*, const char * prefix, const char * uri);
# __LIBSRCML_DECL int srcml_write_attribute(struct srcml_unit*, const char * prefix, const char * name, const char * uri, const char * content);
# __LIBSRCML_DECL int srcml_write_string(struct srcml_unit*, const char * content);
 
# /* Free allocated unit */
# __LIBSRCML_DECL void srcml_free_unit(struct srcml_unit*);

# /* Close the srcML archive */
# __LIBSRCML_DECL void srcml_close_archive(struct srcml_archive*);

# /* Free the srcML archive data */
# __LIBSRCML_DECL void srcml_free_archive(struct srcml_archive*);

# /* Open a srcML archive for reading */
# __LIBSRCML_DECL int srcml_read_open_filename(struct srcml_archive*, const char* srcml_filename);
# __LIBSRCML_DECL int srcml_read_open_memory  (struct srcml_archive*, const char* buffer, size_t buffer_size);
# __LIBSRCML_DECL int srcml_read_open_FILE    (struct srcml_archive*, FILE* srcml_file);
# __LIBSRCML_DECL int srcml_read_open_fd      (struct srcml_archive*, int srcml_fd);
# __LIBSRCML_DECL int srcml_read_open_io      (struct srcml_archive*, void * context, int (*read_callback)(void * context, char * buffer, int len), int (*close_callback)(void * context));

# /* Read the next unit from the archive
#    Returns 0 if there are no more units */
# __LIBSRCML_DECL struct srcml_unit* srcml_read_unit_header(struct srcml_archive*);
# __LIBSRCML_DECL struct srcml_unit* srcml_read_unit_xml(struct srcml_archive*);
# __LIBSRCML_DECL struct srcml_unit* srcml_read_unit(struct srcml_archive*);


# /* srcML XPath query and XSLT transform functions */
# __LIBSRCML_DECL int srcml_clear_transforms                 (struct srcml_archive*);
# __LIBSRCML_DECL int srcml_append_transform_xpath           (struct srcml_archive*, const char* xpath_string);
# __LIBSRCML_DECL int srcml_append_transform_xpath_attribute (struct srcml_archive*, const char* xpath_string,
#                                                             const char* prefix, const char* namespace_uri,
#                                                             const char* attr_name, const char* attr_value);
# __LIBSRCML_DECL int srcml_append_transform_xpath_element   (struct srcml_archive*, const char* xpath_string,
#                                                             const char* prefix, const char* namespace_uri,
#                                                             const char* element,
#                                                             const char* attr_prefix, const char* attr_namespace_uri,
#                                                             const char* attr_name, const char* attr_value);
# __LIBSRCML_DECL int srcml_append_transform_xslt_filename   (struct srcml_archive*, const char* xslt_filename);
# __LIBSRCML_DECL int srcml_append_transform_xslt_memory     (struct srcml_archive*, const char* xslt_buffer, size_t size);
# __LIBSRCML_DECL int srcml_append_transform_xslt_FILE       (struct srcml_archive*, FILE* xslt_file);
# __LIBSRCML_DECL int srcml_append_transform_xslt_fd         (struct srcml_archive*, int xslt_fd);
# __LIBSRCML_DECL int srcml_append_transform_relaxng_filename(struct srcml_archive*, const char* relaxng_filename);
# __LIBSRCML_DECL int srcml_append_transform_relaxng_memory  (struct srcml_archive*, const char* relaxng_buffer, size_t size);
# __LIBSRCML_DECL int srcml_append_transform_relaxng_FILE    (struct srcml_archive*, FILE* relaxng_file);
# __LIBSRCML_DECL int srcml_append_transform_relaxng_fd      (struct srcml_archive*, int relaxng_fd);
# __LIBSRCML_DECL int srcml_append_transform_param           (struct srcml_archive*, const char* param_name, const char* param_value);
# __LIBSRCML_DECL int srcml_append_transform_stringparam     (struct srcml_archive*, const char* param_name, const char* param_value);
# __LIBSRCML_DECL int srcml_apply_transforms                 (struct srcml_archive* iarchive, struct srcml_archive* oarchive);
