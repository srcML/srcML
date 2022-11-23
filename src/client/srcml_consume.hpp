// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_consume.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRCML_CONSUME_HPP
#define SRCML_CONSUME_HPP

#include <memory>
struct ParseRequest;
class WriteQueue;

void srcml_consume(int thread_pool_id, std::shared_ptr<ParseRequest> prequest, WriteQueue*);

#endif
