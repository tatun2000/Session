#pragma once
#include <iostream>
#include <chrono>
using namespace std;

class SimpleTimer
{
public:
	SimpleTimer();
	~SimpleTimer();
private:
	std::chrono::time_point<std::chrono::steady_clock>start, end;
};