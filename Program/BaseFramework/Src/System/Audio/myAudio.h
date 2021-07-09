#pragma once

class SoundEffect
{
public:
	SoundEffect() {}
	~SoundEffect() { m_soundEffects = nullptr; };

	bool Load(const std::string& filename);

	std::unique_ptr<DirectX::SoundEffectInstance>CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS flags)
	{
		if (!m_soundEffects) { return nullptr; }
		return m_soundEffects->CreateInstance(flags);
	}
private:

	std::unique_ptr<DirectX::SoundEffect>	m_soundEffects;
	SoundEffect(const SoundEffect& src) = delete;
	void operator=(const SoundEffect& src) = delete;
};

class SoundInstance :public std::enable_shared_from_this<SoundInstance>
{
public:
	SoundInstance() {}
	void Init(const std::shared_ptr<SoundEffect>& soundEffect);

	void Play(bool loop=false);
	bool SoundInstance::IsPlaying()
	{
		if (!m_instance) { return false; }
		return(m_instance->GetState() != DirectX::SoundState::PLAYING);
	}
private:
	std::unique_ptr<DirectX::SoundEffectInstance>	m_instance;
	std::shared_ptr<SoundEffect>                    m_soundData;
	SoundInstance(const SoundInstance& src) = delete;
	void operator=(const SoundInstance& src) = delete;
};

class AudioManager
{
public:
	bool Play(const std::string& rName, bool loop=false);
	bool Init();
	void Relese();
	void StopBGM();

	void Update();
	void AddPlayList(const std::shared_ptr<SoundInstance>& rSound)
	{
		if (!rSound.get()) { return; }
		m_playList[(UINT)(rSound.get())]=rSound;
	}
	std::unique_ptr<DirectX::AudioEngine>& GetAudioEngine() {return m_audioEng;}

	static AudioManager& GetInstance()
	{
		static AudioManager inctance;
		return inctance;
	}

private:
	std::map<UINT,std::shared_ptr<SoundInstance>>m_playList;
	std::unique_ptr<DirectX::AudioEngine>	m_audioEng;
	AudioManager(){}
};

#define AUDIO AudioManager::GetInstance()
#define AUDIO_ENGINE AUDIO.GetAudioEngine()