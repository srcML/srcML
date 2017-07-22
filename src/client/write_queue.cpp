#include <write_queue.hpp>

WriteQueue::WriteQueue(std::function<void(ParseRequest*)> writearg, bool ordered)
       : maxposition(0) {

    WriteQueue::write = writearg;
    WriteQueue::ordered = ordered;
    pwrite_thread = new std::thread(process);
}

/* writes out the current srcml */
void WriteQueue::schedule(ParseRequest* pvalue) {

	if (pvalue->position > maxposition)
		maxposition = pvalue->position;

	{
		std::unique_lock<std::mutex> lock(WriteQueue::gmutex);

		// put this request into the queue
		WriteQueue::q.push(pvalue);
	}

    WriteQueue::cv.notify_one();
}

void WriteQueue::eos(ParseRequest* pvalue) {

	// schedule the last one
	pvalue->position = maxposition + 1;
    schedule(pvalue);
}

void WriteQueue::wait() {

	// make sure the process thread is not asleep
	//WriteQueue::cv.notify_one();

	pwrite_thread->join();
}

void WriteQueue::process() {
    int counter = 0;

    while (1) {

        ParseRequest* pvalue = 0;
        {
            std::unique_lock<std::mutex> lock(WriteQueue::gmutex);

            while (WriteQueue::q.empty() || (WriteQueue::ordered && (WriteQueue::q.top()->position != counter + 1))) {
            	WriteQueue::cv.wait(lock);
            }

            pvalue = WriteQueue::q.top();
            WriteQueue::q.pop();
        }
        ++counter;

        // record this here because calling write with a request
        // causes it to be deleted
        bool lastone = pvalue->status == 1000 || pvalue->status == 2000;

        // finally write it out
        WriteQueue::write(pvalue);

        // may be all done
        if (lastone)
        	break;
    }
}

bool WriteQueue::ordered;
std::function<void(ParseRequest*)> WriteQueue::write;
std::priority_queue<ParseRequest*, std::deque<ParseRequest*>, WriteOrder> WriteQueue::q;
std::mutex WriteQueue::gmutex;
std::condition_variable WriteQueue::cv;
