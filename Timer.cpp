#include "Timer.h"

SimpleTimer::SimpleTimer()
{
	start = std::chrono::high_resolution_clock::now();
}

SimpleTimer::~SimpleTimer()
{
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;
	cout << "Duration " << duration.count() << " s";
}
