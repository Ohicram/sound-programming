#include "FmodWrapperLibrary.h"

#include <iostream>
#include <fmod_errors.h>
#include <algorithm>
namespace FmodWrapperLibrary
{
	int FmodWrapper::GetPan()
	{
		return static_cast<int>(m_currCh->m_panAudio * 100);
	}

	int FmodWrapper::GetVolume()
	{
		return static_cast<int>(m_currCh->m_volume * 100);
	}

	bool FmodWrapper::IsRepeatOn()
	{
		return m_currCh->m_repeatMode == FMOD_LOOP_NORMAL;
	}

	bool FmodWrapper::IsPaused()
	{
		return m_currCh->m_isPaused;
	}

	std::string FmodWrapper::GetTrackName()
	{
		return m_currCh->m_TrackName;
	}

	void FmodWrapper::SetPan(int percentage)
	{
		m_currCh->m_panAudio = percentage / 100.f;
		m_currCh->m_panAudio = m_currCh->m_panAudio < -1 ? -1 : (m_currCh->m_panAudio > 1 ? 1 : m_currCh->m_panAudio);
		if (m_currCh->m_channel != nullptr)
		{
			FMOD_RESULT const result = m_currCh->m_channel->setPan(m_currCh->m_panAudio);
			if (result != FMOD_OK)
			{
				printf("[FMOD error %d] : Channel pan cannot be set - %s\n", result, FMOD_ErrorString(result));
			}
			else
			{
				printf("Channel pan set correctly\n");
			}
		}
	}
	void FmodWrapper::SetVolume(int volume)
	{
		m_currCh->m_volume = volume / 100.f;
		m_currCh->m_volume = m_currCh->m_volume < 0 ? 0 : (m_currCh->m_volume > 1 ? 1 : m_currCh->m_volume);
		if(m_currCh->m_channel != nullptr)
		{
			FMOD_RESULT const result = m_currCh->m_channel->setVolume(m_currCh->m_volume);
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
		m_currCh->m_repeatMode = enable ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		if (m_currCh->m_channel != nullptr)
		{
			FMOD_RESULT const result = m_currCh->m_channel->setMode(m_currCh->m_repeatMode);
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
		if(m_currCh->m_sound == nullptr)
		{
			std::cout << "Cannot play/pause the sound. No sound loaded\n";
			return;
		}
		if(m_currCh->m_channel == nullptr)
		{
			Play();
		}
		else
		{
			unsigned currentPosition;
			m_currCh->m_channel->getPosition(&currentPosition, FMOD_TIMEUNIT_MS);
			if(currentPosition >= m_currCh->m_soundLength)
			{
				Play();
			}
			else
			{
				SetPaused(!m_currCh->m_isPaused);
			}
		}
	}

	void FmodWrapper::SetPaused(bool status)
	{
		if (m_currCh->m_channel == nullptr)
		{
			printf("No sound playing\n");
			return;
		}
		FMOD_RESULT const result = m_currCh->m_channel->setPaused(status);
		if (result != FMOD_OK)
		{
			printf("[FMOD error %d] : Sound cannot be paused - %s\n", result, FMOD_ErrorString(result));
		}
		else
		{
			printf("Sound paused correctly\n");
			m_currCh->m_isPaused = status;
		}
	}
	
	void FmodWrapper::Play()
	{
		if (m_currCh->m_sound == nullptr)
		{
			printf("No sound loaded\n");
			return;
		}
		 FMOD_RESULT const result = m_system->playSound(m_currCh->m_sound, 0, false, &(m_currCh->m_channel));
		if (result != FMOD_OK)
		{
			m_currCh->m_isPaused = true;
			m_currCh->m_channel = nullptr;
			printf("[FMOD error %d] : Sound cannot be played - %s\n", result, FMOD_ErrorString(result));
		}
		else
		{
			m_currCh->m_isPaused = false;
			printf("Sound played correctly\n");
		}
	}

	void FmodWrapper::Stop()
	{
		if (m_currCh->m_channel == nullptr)
		{
			printf("No sound played\n");
			return;
		}
		FMOD_RESULT const result = m_currCh->m_channel->stop();
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
		FMOD_RESULT const result = m_system->createSound(filepath, fmod_mode | m_currCh->m_repeatMode, NULL, &(m_currCh->m_sound));
		if (result != FMOD_OK)
		{
			printf("[FMOD error %d] : File (%s) cannot be loaded - %s\n", result, filepath,  FMOD_ErrorString(result));
			m_currCh->m_sound = nullptr;
			m_currCh->m_channel = nullptr;
		}
		else
		{
			printf("Sound: %s loaded correctly\n", filepath);
			m_currCh->m_sound->getLength(&(m_currCh->m_soundLength), FMOD_TIMEUNIT_MS);
		}
		m_currCh->m_TrackName = std::string(filepath);
	}

	void FmodWrapper::LoadSoundStreaming(const char* filepath, bool compressed)
	{
		FMOD_MODE fmod_mode = compressed ? FMOD_CREATECOMPRESSEDSAMPLE : FMOD_CREATESAMPLE;
		FMOD_RESULT const result = m_system->createStream(filepath, fmod_mode, NULL, &(m_currCh->m_sound));
		if (result != FMOD_OK)
		{
			printf("[FMOD error %d] : File (%s) cannot be loaded - %s\n", result, filepath,  FMOD_ErrorString(result));
			m_currCh->m_sound->getLength(&(m_currCh->m_soundLength), FMOD_TIMEUNIT_MS);
		}
		else
		{
			printf("Sound: %s loaded correctly\n", filepath);
		}
	}

	void FmodWrapper::SelectChannel(size_t index)
	{
		index = index > MAX_CHNLS ? (MAX_CHNLS - 1) : index;
		m_currCh = &(m_availableChannels[index]);
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
