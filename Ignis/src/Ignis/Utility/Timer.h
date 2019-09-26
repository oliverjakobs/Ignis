#pragma once

#include <chrono>

namespace ignis
{
	struct FrameCounter
	{
		float Seconds;
		int Frames;
		int FPS;

		float DeltaTime;
		float LastFrame;

		FrameCounter() : Seconds(0.0f), Frames(0), FPS(0), DeltaTime(0.0f), LastFrame(0.0f) {}

		inline void Start(float seconds)
		{
			DeltaTime = seconds - LastFrame;
			LastFrame = seconds;
		}

		inline void End(float seconds)
		{
			Frames++;
			if ((seconds - Seconds) > 1.0f)
			{
				Seconds += 1.0f;
				FPS = Frames;
				Frames = 0;
			}
		}
	};

	class Timer
	{
	private:
		long long m_start;
		long long m_end;
		
	public:
		Timer() : m_start(0), m_end(0) { }

		inline void Start()
		{
			auto startPoint = std::chrono::high_resolution_clock::now();
			m_start = std::chrono::time_point_cast<std::chrono::microseconds>(startPoint).time_since_epoch().count();
		}

		inline void End()
		{
			auto endPoint = std::chrono::high_resolution_clock::now();
			m_end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();
		}

		inline float GetDurationMS() const
		{
			auto duration = m_end - m_start;
			return duration * 0.001f;
		}
	};
}