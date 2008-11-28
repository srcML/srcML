/*
  srcMLOutputPR.h

  Copyright (C) 2005, 2006  SDML (www.sdml.info)

  This file is part of the srcML translator.

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

  Declaration of process pointer table for srcMLOutput.
*/

srcMLOutput::PROCESS_PTR srcMLOutput::process_table[] = {

  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, 
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText,
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText,
  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText,

  &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText, &srcMLOutput::processText,

  /* process_table[SUNIT] = */ &srcMLOutput::processUnit,

  &srcMLOutput::processText,

  &srcMLOutput::processText,

  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken,
  &srcMLOutput::processToken, &srcMLOutput::processToken, &srcMLOutput::processToken
};
