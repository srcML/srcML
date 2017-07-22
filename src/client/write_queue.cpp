#include <write_queue.hpp>

static WriteQueue* wq;

struct cmp
{
    bool operator()(ParseRequest* r1, ParseRequest* r2) {
        return r1->position > r2->position;
    }
};

static std::priority_queue<ParseRequest*, std::deque<ParseRequest*>, cmp > q;
static std::mutex gmutex;
static std::condition_variable cv;
static std::condition_variable cv2;

WriteQueue::WriteQueue(std::function<void(ParseRequest*)> writearg, bool ordered)
       : ordered(ordered) {

    write = writearg;
    wq = this;

    pthread = new std::thread(process);
}

/* writes out the current srcml */
void WriteQueue::schedule(ParseRequest* pvalue) {
	{
		std::unique_lock<std::mutex> lock(gmutex);

		// put this request into the queue
		q.push(pvalue);
	}

    cv.notify_one();
}

void WriteQueue::eos(ParseRequest* pvalue) {

	pvalue->position = 1000;
    schedule(pvalue);

	std::unique_lock<std::mutex> lock(gmutex);
	cv2.wait(lock);
}

void WriteQueue::wait() {

	cv.notify_all();
	{
		std::unique_lock<std::mutex> lock(gmutex);

		cv2.wait(lock);
	}
}

void process() {
    int counter = 0;

    while (1) {

        ParseRequest* pvalue = 0;
        {
            std::unique_lock<std::mutex> lock(gmutex);

            while (q.empty() /* || ((q.top()->position != counter + 1) || (q.top()->status != 1000))*/)
            	cv.wait(lock);

            pvalue = q.top();
            q.pop();
        }
        ++counter;

        bool lastone = pvalue->status == 1000 || pvalue->status == 2000;

        wq->write(pvalue);

        if (lastone)
        	break;
    }

    cv2.notify_all();
}
