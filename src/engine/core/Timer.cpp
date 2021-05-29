#include "Timer.h"

namespace MattEngine {
Timer::Timer(unsigned int milliseconds, std::function<void()> callback)
	: m_milliseconds(milliseconds), m_callback(callback) {
	Timers.push_back(this);
}

void Timer::onUpdate(float deltaTime) {
	if (m_elapsed)
		return;

	unsigned int milliseconds = deltaTime * 1000;

	if (m_milliseconds < milliseconds) {
		m_callback();
		m_elapsed = true;
	}

	m_milliseconds -= milliseconds;
}

bool Timer::isElapsed() const { return m_elapsed; }
} // namespace MattEngine