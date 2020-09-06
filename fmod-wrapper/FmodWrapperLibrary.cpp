#include "FmodWrapperLibrary.h"

#include <iostream>
#include <fmod_errors.h>

namespace FmodWrapperLibrary
{
	void FmodWrapper::SetVolume(int volume)
	{
		m_volume = volume / 100.f;
		if(m_channel != nullptr)
		{
			FMOD_RESULT const result = m_channel->setVolume(m_volume);
			if (result != FMOD_OK)
			{
				printf("[FMOD error %d] : Channel volume cannot be set - %s\n", result, FMOD_ErrorString(result));
			}
			else
			{
				printf("Channel Volume set correctly\n");
			}
		}
	}
	void FmodWrapper::Repeat(bool enable)
	{
		m_repeatMode = enable ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		if (m_channel != nullptr)
		{
			FMOD_RESULT const result = m_channel->setMode(m_repeatMode);
			if (result != FMOD_OK)
			{
				printf("[FMOD error %d] : Sound mode cannot be set - %s\n", result, FMOD_ErrorString(result));
			}
			else
			{
				printf("Sound loop mode set correctly\n");
			}
		}
	}

	void FmodWrapper::PlayPause()
	{
		if(m_sound == nullptr)
		{
			std::cout << "Cannot play/pause the sound. No sound loaded\n";
			return;
		}
		if(m_channel == nullptr)
		{
			Play();
		}
		else
		{
			unsigned currentPosition;
			m_channel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);
			if(currentPosition >= m_soundLength)
			{
				Play();
			}
			else
			{
				Pause(!m_isPaused);
			}
		}
	}

	void FmodWrapper::Pause(bool status)
	{
		if (m_channel == nullptr)
		{
			printf("No sound playing\n");
			return;
		}
		FMOD_RESULT const result = m_channel->setPaused(status);
		if (result != FMOD_OK)
		{
			printf("[FMOD error %d] : Sound cannot be paused - %s\n", result, FMOD_ErrorString(result));
		}
		else
		{
			printf("Sound paused correctly\n");
			m_isPaused = status;
		}
	}
	
	void FmodWrapper::Play()
	{
		if (m_sound == nullptr)
		{
			printf("No sound loaded\n");
			return;
		}
		 FMOD_RESULT const result = m_system->playSound(m_sound, 0, false, &m_channel);
		if (result != FMOD_OK)
		{
			m_isPaused = false;
			m_channel = nullptr;
			printf("[FMOD error %d] : Sound cannot be played - %s\n", result, FMOD_ErrorString(result));
		}
		else
		{
			printf("Sound played correctly\n");
		}
	}

	void FmodWrapper::Stop()
	{
		if (m_channel == nullptr)
		{
			printf("No sound played\n");
			return;
		}
		FMOD_RESULT const result = m_channel->stop();
		if (result != FMOD_OK)
		{
			printf("[FMOD error %d] : Sound cannot be stopped - %s\n", result, FMOD_ErrorString(result));
		}
		else
		{
			printf("Sound stopped correctly\n");
		}
	}

	
	void FmodWrapper::LoadSound(const char* filepath, bool compressed)
	{
		FMOD_MODE const fmod_mode = compressed ? FMOD_CREATECOMPRESSEDSAMPLE : FMOD_CREATESAMPLE;
		FMOD_RESULT const result = m_system->createSound(filepath, fmod_mode | m_repeatMode, NULL, &m_sound);
		if (result != FMOD_OK)
		{
			printf("[FMOD error %d] : File (%s) cannot be loaded - %s\n", result, filepath,  FMOD_ErrorString(result));
			m_sound = nullptr;
			m_channel = nullptr;
		}
		else
		{
			printf("Sound: %s loaded correctly\n", filepath);
			m_sound->getLength(&m_soundLength, FMOD_TIMEUNIT_MS);
		}

	}

	void FmodWrapper::LoadSoundStreaming(const char* filepath, bool compressed)
	{
		FMOD_MODE fmod_mode = compressed ? FMOD_CREATECOMPRESSEDSAMPLE : FMOD_CREATESAMPLE;
		FMOD_RESULT const result = m_system->createStream(filepath, fmod_mode, NULL, &m_sound);
		if (result != FMOD_OK)
		{
			printf("[FMOD error %d] : File (%s) cannot be loaded - %s\n", result, filepath,  FMOD_ErrorString(result));
			m_sound->getLength(&m_soundLength, FMOD_TIMEUNIT_MS);
		}
		else
		{
			printf("Sound: %s loaded correctly\n", filepath);
		}
	}


	void FmodWrapper::SayHello()
	{
		std::cout << "Hello!\n";
	}
	FmodWrapper::FmodWrapper()
	{
		FMOD_RESULT result = FMOD::System_Create(&m_system);      // Create the main system object.
		if (result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			exit(-1);
		}

		result = m_system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
		if (result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			exit(-1);
		}
	}

}
