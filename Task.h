#ifndef __SUN_TASK_H
#define __SUN_TASK_H

#include <vector>
#include <functional>

namespace Sun {

	class Executor;
	enum class ETaskType {
		TASK = 0,
		TASK_FLOW =1,
	};
	class Task {
	public:
		Task() {}
		Task(std::function<void()>&& func ,size_t id=0);

		size_t getTaskNum();
		union {
			Task* parent =nullptr;
		};
		size_t id_ = 0;
		ETaskType parentType_ = ETaskType::TASK;
		Executor* executor_;
		std::function<void()> func_;

		std::vector<Task*> children_;
		int unfinished_children_num_ = 0;

		std::vector<Task*> successors_;
		std::vector<Task*> dependents_;

		int unfinished_dependents_num_ = 0;

		void addChild(Task*);

		void precede(Task* task);

		void succeed(Task* task);

		void run();
	};

}

#endif 