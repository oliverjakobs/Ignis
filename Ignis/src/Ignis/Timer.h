#pragma once

namespace ignis
{
	struct Timer
	{
		float Seconds;
		int Frames;
		int FPS;

		float DeltaTime;
		float LastFrame;

		Timer() : Seconds(0.0f), Frames(0), FPS(0), DeltaTime(0.0f), LastFrame(0.0f) {}

		inline void Start(float time)
		{
			DeltaTime = time - LastFrame;
			LastFrame = time;
		}

		inline void End(float time)
		{
			Frames++;
			if ((time - Seconds) > 1.0f)
			{
				Seconds += 1.0f;
				FPS = Frames;
				Frames = 0;
			}
		}
	};
}