#include "myAudio.h"

bool AudioManager::Init()
{
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_EnvironmentalReverb | DirectX::AudioEngine_ReverbUseFilters;

	m_audioEng = std::make_unique<DirectX::AudioEngine>(eflags);
	m_audioEng->SetReverb(DirectX::Reverb_Default);

	if (m_audioEng) { return true; }
	return false;
}

void AudioManager::Relese()
{
	m_audioEng = nullptr;
}

void AudioManager::StopBGM()
{
	for (auto itr = m_playList.begin(); itr != m_playList.end();)
	{
		if (!itr->second->IsPlaying())
		{
			itr = m_playList.erase(itr);
			continue;
		}
		++itr;
	}
}

void AudioManager::Update()
{
	//サウンドエンジンの更新
	if (m_audioEng != nullptr)
	{
		m_audioEng->Update();
	}
	//再生中ではないインスタンスは終了元としてリストから削除
	for (auto itr = m_playList.begin(); itr != m_playList.end();)
	{
		if (itr->second->IsPlaying())
		{
			itr = m_playList.erase(itr);
			continue;
		}
		++itr;
	}
}

bool SoundEffect::Load(const std::string& filename)
{
	if (AUDIO.GetAudioEngine().get() != nullptr)
	{
		try
		{
			//wstringに変換
			std::wstring wFilename = sjis_to_wide(filename);

			m_soundEffects = (std::make_unique<DirectX::SoundEffect>(AUDIO_ENGINE.get(), wFilename.c_str()));

		}
		catch (...)
		{
			assert(0 && "Sound File Load Error");
		}
	}
	else
	{
		return false;
	}
	return true;
}

void SoundInstance::Init(const std::shared_ptr<SoundEffect>& soundEffect)
{
	if (!soundEffect) { return; }
	DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags = DirectX::SoundEffectInstance_Default;
	m_instance = (soundEffect->CreateInstance(flags));
	m_soundData = soundEffect;
}

void SoundInstance::Play(bool loop)
{
	if (!m_instance) { return; }
	m_instance->Play(loop);
	AUDIO.AddPlayList(shared_from_this());
}

bool AudioManager::Play(const std::string& rName,bool loop)
{
	if (!m_audioEng) { return false; }
	std::shared_ptr<SoundEffect> soundData = ResFac.GetSound(rName);
	if (!soundData) { return false; }
	std::shared_ptr<SoundInstance> instance = std::make_shared<SoundInstance>();
	if (!instance) { return false; }
	instance->Init(soundData);
	instance->Play(loop);
	return true;
}