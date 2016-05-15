// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2015 Qianyi Zhou <Qianyi.Zhou@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "Timer.h"

#include <chrono>

#include <Core/Utility/Console.h>

namespace three{

Timer::Timer() :
		start_time_in_milliseconds_(0.0), end_time_in_milliseconds_(0.0)
{
}

Timer::~Timer()
{
}

double Timer::GetSystemTimeInMilliseconds()
{
	std::chrono::duration<double, std::milli> current_time =
			std::chrono::high_resolution_clock::now().time_since_epoch();
	return current_time.count();
}

void Timer::Start()
{
	start_time_in_milliseconds_ = GetSystemTimeInMilliseconds();
}

void Timer::Stop()
{
	end_time_in_milliseconds_ = GetSystemTimeInMilliseconds();
}

double Timer::GetDuration()
{
	return end_time_in_milliseconds_ - start_time_in_milliseconds_;
}

void Timer::Print(const std::string &timer_info)
{
	PrintInfo("%s %.2f ms.\n", timer_info.c_str(),
			end_time_in_milliseconds_ - start_time_in_milliseconds_);
}

ScopeTimer::ScopeTimer(const std::string &scope_timer_info/* = ""*/) :
		scope_timer_info_(scope_timer_info)
{
	Timer::Start();
}

ScopeTimer::~ScopeTimer()
{
	Timer::Stop();
	Timer::Print(scope_timer_info_ + " took");
}

FPSTimer::FPSTimer(const std::string &fps_timer_info/* = ""*/,
			double time_to_print/* = 3000.0*/, int events_to_print/* = 100*/) :
			fps_timer_info_(fps_timer_info), time_to_print_(time_to_print),
			events_to_print_(events_to_print), event_count_(0)
{
}

void FPSTimer::Signal()
{
	if (event_count_ == 0) {
		Start(); Stop();
		event_count_ = 1;
	} else if (GetDuration() >= time_to_print_ ||
			event_count_ >= events_to_print_) {
		// print and reset
		PrintInfo("%s at %.2f fps.\n", fps_timer_info_.c_str(),
				double(event_count_ + 1) / GetDuration() * 1000.0);
		Start(); Stop();
		event_count_ = 1;
	} else {
		Stop();
		event_count_++;
	}
}

}	// namespace three