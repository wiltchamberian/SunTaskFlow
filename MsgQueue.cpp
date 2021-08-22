#include "MsgQueue.h"

namespace Sun {

	void MsgQueue::push(const Msg& msg) {
		std::unique_lock<std::mutex> lock(mute_);
		que_.push(msg);
		condition_.notify_one();
	}

	Msg MsgQueue::front() {
		std::unique_lock<std::mutex> lock(mute_);
		return que_.front();
	}

	Msg MsgQueue::pop() {
		std::unique_lock<std::mutex> lock(mute_);
		Msg msg = que_.front();
		que_.pop();
		return msg;
	}

	bool MsgQueue::empty() {
		std::unique_lock<std::mutex> lock(mute_);
		return que_.empty();
	}

	void MsgQueue::wait() {
		std::unique_lock<std::mutex> lock(mute_);
		condition_.wait(lock, [this]() {
			return (!que_.empty());
			});
	}

	void MsgQueue::stop_wait() {
		std::unique_lock<std::mutex> lock(mute_);
		stop_ = true;
		condition_.notify_one();
	}
}