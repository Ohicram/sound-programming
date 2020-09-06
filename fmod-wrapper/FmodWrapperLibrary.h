#pragma once
#include <fmod.hpp>
namespace FmodWrapperLibrary
{
	const size_t MAX_CHNLS = 4;
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

		void SelectChannel(size_t index);

		
		FmodWrapper(FmodWrapper const&) = delete;
		void operator=(FmodWrapper const&) = delete;
	private:
		FmodWrapper();

		FMOD::System* m_system = nullptr;

		struct channelInterface {
			FMOD::Sound* m_sound;
			FMOD::Channel* m_channel;

			FMOD_MODE m_repeatMode = FMOD_LOOP_OFF;

			bool m_isPaused;
			unsigned m_soundLength;
			float m_volume;
			float m_panAudio;
		};

		channelInterface m_availableChannels[MAX_CHNLS];
		channelInterface* m_currCh = m_availableChannels;
	};
}

