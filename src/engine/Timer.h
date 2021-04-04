#ifndef TIMER_H
#define TIMER_H

#include <vector>
#include <functional>

namespace MattEngine {

	class Timer {
    public:
        inline static std::vector<Timer*> Timers;
	public:
		Timer(unsigned int milliseconds, std::function<void()> callback);
		bool isElapsed() const;
        void onUpdate(float deltaTime);
	private:
		unsigned int m_milliseconds = 0;
        std::function<void()> m_callback;
        bool m_elapsed = false;
	};

}

#endif
