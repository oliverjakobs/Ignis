#pragma once

#include <chrono>

namespace obelisk
{
	struct Timer
	{
		float Seconds;
		int Frames;
		int FPS;

		float DeltaTime;
		float LastFrame;

		Timer() : Seconds(0.0f), Frames(0), FPS(0), DeltaTime(0.0f), LastFrame(0.0f) {}

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

	class Chrono
	{
	private:
		long long m_start;
		
	public:
		Chrono()
		{
			auto startPoint = std::chrono::high_resolution_clock::now();
			m_start = std::chrono::time_point_cast<std::chrono::microseconds>(startPoint).time_since_epoch().count();
		}

		inline void Reset()
		{
			auto startPoint = std::chrono::high_resolution_clock::now();
			m_start = std::chrono::time_point_cast<std::chrono::microseconds>(startPoint).time_since_epoch().count();
		}

		inline float GetElapsedMS() const
		{
			auto endPoint = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count() - m_start;

			return elapsed * 0.001f;
		}
	};
}