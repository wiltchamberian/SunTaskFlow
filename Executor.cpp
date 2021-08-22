#include "Executor.h"
#include "Task.h"
#include "ThreadPool.h"
#include <cassert>

namespace Sun {

	Executor::Executor() {
		pool_ = new ThreadPool(std::thread::hardware_concurrency());
	}

	Executor::~Executor() {
		delete pool_;
	}

	void Executor::run(Task* taskf) {
		if (taskf == nullptr) return;

		//�ݹ�taskf,������task ��
		totalUnFinishedTaskNum_ = taskf->getTaskNum();
		//totalUnFinishedTaskNum_ -= 1;

		spawn(taskf);

		while (totalUnFinishedTaskNum_>0) {
			queue_.wait();
			if (queue_.empty()) {
				assert(false);
			}

			Msg msg = queue_.front();
			//��pop,֮ǰ���Ȱ�msg push�����У��Ӷ���֤����ʼ�շǿ�
			if (msg.taskState == TaskState::FINISH) {
				totalUnFinishedTaskNum_ -= 1;
				if (totalUnFinishedTaskNum_ == 0) {
					//queue_.stop_wait();
				}
				for (Task* task : msg.task->successors_) {
					task->unfinished_dependents_num_ -= 1;
					if (task->unfinished_dependents_num_ == 0) {
						spawn(task);
					}
				}
				if (msg.task->parent) {
					msg.task->parent->unfinished_children_num_ -= 1;
					if (msg.task->parent->unfinished_children_num_ == 0)
					{
						Msg msg2;
						msg2.task = msg.task;
						notify(msg2);
					}
				}
				else{
					assert(totalUnFinishedTaskNum_ == 0);
				}
			}

			queue_.pop();
			
			
		}
	}

	void Executor::spawn(Task* task) {
		//�������������spawn������
		if (!task->children_.empty()) {
			std::vector<Task*> srcs;
			for (auto& it : task->children_) {
				if (it->dependents_.empty()) {
					srcs.push_back(it);
				}
			}
			for (auto& it : srcs) {
				spawn(it);
			}
		}
		else {
			auto ftr = pool_->enqueue([task, this]() {
				task->run();

				Msg msg;
				msg.task = task;
				notify(msg);
				});
		}

		
	}

	void Executor::notify(const Msg& msg) {
		queue_.push(msg);
	}
}