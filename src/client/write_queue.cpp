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
#include <srcml_write.hpp>

WriteQueue::WriteQueue(TraceLog& log, const srcml_output_dest& destination, bool ordered)
       : log(log), destination(destination), ordered(ordered), maxposition(0), q(
            [](ParseRequest* r1, ParseRequest* r2) {
                return r1->position > r2->position;
            }) {
}

/* writes out the current srcml */
void WriteQueue::schedule(ParseRequest* pvalue) {

    // push the value on the priority queue
	{
		std::lock_guard<std::mutex> lock(qmutex);

        // record max position for eos()
        if (pvalue->position > maxposition)
            maxposition = pvalue->position;

		// put this request into the queue
		q.push(pvalue);
	}

    // let the write processing know there is something
    cv.notify_one();
}

void WriteQueue::eos() {

	// schedule the last one
    ParseRequest* pvalue = new ParseRequest;
	pvalue->position = maxposition + 1;
    pvalue->status = 1000;
    schedule(pvalue);
}

void WriteQueue::start() {

    // actual thread created here (and not in constructor) because
    // at this point we know all object data members are created
    // and initialized correctly
    write_thread = std::thread(&WriteQueue::process, this);
}

void WriteQueue::stop() {

    eos();

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

        if (pvalue->status == 1000)
            break;

        // record real units written
        if (pvalue->status == SRCML_STATUS_OK)
            ++total;

        // finally write it out
        srcml_write_request(pvalue, log, destination);
    }
}
