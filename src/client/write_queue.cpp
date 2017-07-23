/**
 * @file write_queue.cpp
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

#include <write_queue.hpp>

WriteQueue::WriteQueue(std::function<void(ParseRequest*)> writearg, bool ordered)
       : write(writearg), ordered(ordered), maxposition(0) {
}

/* writes out the current srcml */
void WriteQueue::schedule(ParseRequest* pvalue) {

    // record max position for eos()
	if (pvalue->position > maxposition)
		maxposition = pvalue->position;

    // push the value on the priority queue
	{
		std::unique_lock<std::mutex> lock(qmutex);

		// put this request into the queue
		q.push(pvalue);
	}

    // let the write processing know there is something
    WriteQueue::cv.notify_one();
}

void WriteQueue::eos(ParseRequest* pvalue) {

	// schedule the last one
	pvalue->position = maxposition + 1;
    schedule(pvalue);
}

void WriteQueue::start() {

    // actual thread created here (and not in constructor) because
    // at this point we kwow all object data members are created
    // and initialized correctly
    write_thread = std::thread(&WriteQueue::process, this);
}

void WriteQueue::stop() {

    write_thread.join();
}

void WriteQueue::process() {

    int position = 0;
    while (1) {

        // get a parse request to handle
        ParseRequest* pvalue = 0;
        {
            std::unique_lock<std::mutex> lock(qmutex);

            while (q.empty() || (ordered && (q.top()->position != position + 1))) {
            	cv.wait(lock);
            }

            pvalue = q.top();
            q.pop();
        }
        ++position;

        // record this here because calling write with a request
        // causes it to be deleted
        bool lastone = pvalue->status == 1000 || pvalue->status == 2000;

        // finally write it out
        write(pvalue);

        // may be all done
        if (lastone)
        	break;
    }
}
