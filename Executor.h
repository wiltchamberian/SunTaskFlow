#ifndef __SUN_TASK_EXECTOR_H
#define __SUN_TASK_EXECTOR_H

#include "MsgQueue.h"
#include <thread>

class ThreadPool;
namespace Sun {

	class Task;
	
	class Executor {
	public:
		Executor();
		~Executor();
		void run(Task* taskf);
		void spawn(Task* task);
		void notify(const Msg& msg);
	protected:
		size_t totalUnFinishedTaskNum_ = 0;
		std::condition_variable condition_;
		ThreadPool* pool_ = nullptr;
		MsgQueue queue_;
	};

}

#endif