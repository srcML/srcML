/*
  srcml.h

  Copyright (C) 2013  SDML (www.sdml.info)

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef INCLUDED_SRCML_H
#define INCLUDED_SRCML_H

#ifdef __cplusplus
// extern "C" {
#endif

/* srcml error messages */
#define SRCML_OK 0
#define SRCML_ERROR 1

/* srcml options */
#define SRCML_OPTION_LITERAL  1<<0
#define SRCML_OPTION_MODIFIER 1<<1
#define SRCML_OPTION_ARCHIVE  1<<2
#define SRCML_OPTION_COMPRESS  1<<1
#define SRCML_OPTION_CPP
#define SRCML_OPTION_CPP_TEXT_ELSE
#define SRCML_OPTION_CPP_MARKUP_IF0
#define SRCML_OPTION_APPLY_ROOT

/*
  src2srcml - translate from source code to the srcML format

  NOTE:  Overloaded names until parameters are determined
  NOTE:  "converted"? "transformed"? Other term?
*/

typedef struct srcMLData srcMLData;

/* source code file converted to an individual srcML file */
int src2srcml_filename_filename(char* src_filename, char* srcml_filename, int options, char* language, char* src_encoding, char* attributes[][2]);

/* list of source code files converted to a srcML archive file */
int src2srcml_filelist_filename(char* src_filenames[], int size, char* srcmlfilename, char* language, char* src_encoding, int options);

/* source code file converted to an individual srcML buffer */
int src2srcml_filename_buffer(char* src_filename, char** srcml_buffer, char* language, char* src_encoding, int options);

/* source code buffer converted to a srcML file */
int src2srcml_buffer_filename(char* src_buffer, char* srcml_filename, char* language, char* src_encoding, int options);

/* source code buffer to the individual srcML format */
int src2srcml_buffer_buffer(char* src_buffer, char** srcml_buffer, char* language, char* src_encoding, int options);

/*
  Write a srcML archive file by file
*/

/* open a srcML archive for output */
srcMLData* src2srcml_open_file(char* srcml_filename, int options, char* src_encoding, char* attributes[][2]);

/* write a src file to the srcML archive after translation */
int src2srcml_write_unit_file(srcMLData* arcdata, char* src_filename, char* language, char* attributes[][2]);

/* write a buffer to the srcML archive after translation */
int src2srcml_write_unit_buffer(srcMLData* arcdata, char* buffer, int buffer_size, char* language, char* attributes[][2]);

/* close the srcML archive */
void src2srcml_close(srcMLData* arcdata);

/* free the srcML archive data */
void src2srcml_free(srcMLData*);

/*
  srcml2src - translate from the srcML format back to source code
  NOTE: mirror of src2srcml
*/

/* srcML file converted back to a source code file, or directory of source-code files for an archive */
int srcml2src(char* srcml_filename, char* src_filename, int options, char* src_encoding, int unit);


/* srcml utility functions NOTE:  need memory forms of these */

/* srcML attributes with namespaces */
char* [2][2] srcml_info(char* srcml_filename);

/* srcML attributes with namespaces and number of units */
char* [][2] srcml_longinfo(char* srcml_filename);

/* srcML attributes with namespaces of a particular unit in an archive */
char* [][2] srcml_info_unit(char* srcml_filename, int unit);

/* list of filenames */
char* [] srcml_list(char* srcml_filename);


/* srcml XPath query and XSLT transform functions */

/* XPath (as a string) is applied to the input srcml filename with the query result stored in the
   output srcml filename */
int srcml_xpath(char* input_srcml_filename, char* xpath_string, char* output_srcml_filename, int options);

/* XSLT file is applied to the input srcml filename with the transformed result stored in the
   output srcml filename */
int srcml_xslt(char* input_srcml_filename, char* xslt_filename, char* output_srcml_filename, int options, char* xsltparams[][2]);

/* RelaxNG file is applied to the input srcml filename with the transformed result stored in the
   output srcml filename */
int srcml_relaxng(char* input_srcml_filename, char* relaxng_filename, char* output_srcml_filename, int options, char* xsltparams[][2]);

/* Pipeline of XPath, XSLT, and RelaxNG transformations on the input srcml filename with the transformed result stored in the
   output srcml filename. XPath is a string, while XSLT and RelaxNG are filenames.  Transforms are applied in the order given */
int srcml_transform(char* input_srcml_filename, char* transformation[], int transformation_size, char* output_srcml_filename, int options);

#ifdef __cplusplus
//}
#endif

#endif
