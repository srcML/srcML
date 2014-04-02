/**
 * @file Mode.cpp
 *
 * @copyright Copyright (C) 2004-2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ModeStack.hpp"

/**
 * endDownToMode
 * @param mode mode to end down to
 *
 * End elements down to but not including the mode mode.
 */
void ModeStack::endDownToMode(const srcMLState::MODE_TYPE& mode) {

    if (!inTransparentMode(mode))
        return;

    while (size() > 1 && !inMode(mode))
        endMode();
}

/**
 * endDownToModeSet
 * @param mode mode to end down to
 *
 * End elements down to but not including the mode mode.
 */
void ModeStack::endDownToModeSet(const srcMLState::MODE_TYPE& mode) {

    //  if (getTransparentMode() & (mode == 0))
    //      return;

    while (size() > 1 && (getMode() & mode) == 0)
        endMode();
}

/**
 * endDownOverMode
 * @param mode mode to end down to
 *
 * End elements down to and including the mode mode.
 */
void ModeStack::endDownOverMode(const srcMLState::MODE_TYPE& mode) {

    while (size() > 1 && inMode(mode))
        endMode();
}
