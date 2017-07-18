/**
 * @file global_errors.cpp
 *
 * @copyright Copyright (C) 2017 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <global_errors.hpp>
#include <mutex>
#include <condition_variable>

static std::mutex e;
static bool production_errors = false;

static std::mutex c;
static bool curl_errors = false;

static std::mutex d;

void setProductionErrors() {
    std::unique_lock<std::mutex> l(e);
    production_errors = true;
}

void clearProductionErrors() {
    std::unique_lock<std::mutex> l(e);
    production_errors = false;
}

bool getProductionErrors() {
    std::unique_lock<std::mutex> l(e);
    return production_errors;
}

void setCurlErrors() {
    std::unique_lock<std::mutex> l(c);
    curl_errors = true;
}

void clearCurlErrors() {
    std::unique_lock<std::mutex> l(c);
    curl_errors = false;
}

bool getCurlErrors() {
    std::unique_lock<std::mutex> l(c);
    return curl_errors;
}

static bool go = false;
static std::condition_variable cv;

int waitCurl() {
    std::unique_lock<std::mutex> l(d);
    cv.wait(l);
    return go;
}

void goCurl(bool flag) {
    std::unique_lock<std::mutex> l(d);
    go = flag;
    cv.notify_one();
}
