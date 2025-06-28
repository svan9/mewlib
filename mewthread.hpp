#ifndef MEW_THREAD_LIB_SO2U
#define MEW_THREAD_LIB_SO2U

#include "mewlib.h"
#include "mewtypes.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <mutex>

namespace mew {
	typedef bool(*bool_watcher)(u8* data);

	struct Watcher {
		// todo std::variant
		bool_watcher watcher;
		callable callback;

		Watcher() {}

		Watcher(bool_watcher watcher, callable callback): watcher(watcher), callback(callback) {}
	};

	std::mutex watcher_mutex;

	std::vector<Watcher>& GetWatcherList() {
		static std::vector<Watcher> wchrs;
		return wchrs;
	}

	void watcher_thread_fn() {
		const auto& wchrs = GetWatcherList();
		for (auto& wchr: wchrs) {
			watcher_mutex.lock();
			if (wchr.watcher(nullptr)) { wchr.callback(); }
			watcher_mutex.unlock();
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	
	std::thread watcher_thread;

	void startWatcherThread() {
		if (!watcher_thread.joinable()) {
			watcher_thread = std::thread([](){
				while (true) {
					watcher_thread_fn();
				}
			});
		}
	}

	void watchBool(bool_watcher watcher, callable callback) {
		watcher_mutex.lock();
			auto& wchrs = GetWatcherList();	
			wchrs.emplace_back(watcher, callback);
		watcher_mutex.unlock();
	}

	
};

#endif