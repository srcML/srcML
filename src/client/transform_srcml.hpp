// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file transform_srcml.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef TRANSFORM_SRCML_HPP
#define TRANSFORM_SRCML_HPP

#include <srcml_cli.hpp>
#include <srcml_input_src.hpp>

int apply_xslt(srcml_archive* in_arch, std::string_view transform_input);

int apply_xpath(srcml_archive* in_arch, srcml_archive* out_arch, std::string_view transform_input, const std::pair< std::optional<element>, std::optional<attribute> >& xpath_support, const std::map<std::string,std::string>& xmlns_namespaces);

int apply_srcql(srcml_archive* in_arch, srcml_archive* out_arch, std::string_view transform_input, const std::pair< std::optional<element>, std::optional<attribute> >& srcql_support, const std::map<std::string,std::string>& xmlns_namespaces);

int apply_relaxng(srcml_archive* in_arch, std::string_view transform_input);

#endif
