#include "Task.h"

namespace Sun {

	Task::Task(std::function<void()>&& func ,size_t id) {
		func_ = func;
		id_ = id;
	}

	size_t Task::getTaskNum() {
		size_t res = 0;
		for (auto& it : children_) {
			res += it->getTaskNum();
		}
		res += 1;
		return res;
	}

	void Task::addChild(Task* task) {
		children_.push_back(task);
		task->parent = this;

		unfinished_children_num_ += 1;
	}

	void Task::precede(Task* t) {
		this->successors_.push_back(t);
		t->dependents_.push_back(this);
		t->unfinished_dependents_num_ += 1;
	}

	void Task::succeed(Task* t) {
		t->successors_.push_back(this);
		this->dependents_.push_back(t);
		this->unfinished_dependents_num_ += 1;
	}

	void Task::run() {
		func_();
	}
}