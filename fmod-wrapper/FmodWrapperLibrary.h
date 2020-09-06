#pragma once
#include <fmod.hpp>
namespace FmodWrapperLibrary
{
	class FmodWrapper
	{
	public:
		static FmodWrapper& getInstance()
		{
			static FmodWrapper instance;
			return  instance;
		}
		static void SayHello();
		void LoadSound(const char* filepath, bool compressed = false);
		void LoadSoundStreaming(const char* filepath, bool compressed = false);
		void Pause(bool status);
		void Play();
		void PlayPause();
		void Repeat(bool enable);
		void Stop();
		void SetVolume(int volume);
		void SetPan(int percentage);

		
		FmodWrapper(FmodWrapper const&) = delete;
		void operator=(FmodWrapper const&) = delete;
	private:
		FmodWrapper();

		FMOD::System* m_system = nullptr;
		FMOD::Sound* m_sound = nullptr;
		FMOD::Channel* m_channel = nullptr;

		FMOD_MODE m_repeatMode = FMOD_LOOP_OFF;
		
		bool m_isPaused;
		unsigned m_soundLength;
		float m_volume;
		float m_panAudio;
	};
}

