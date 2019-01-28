#pragma once

JEONG_BEGIN

class JEONG_DLL SoundManager
{
	//단발 Sound Effect
	//루프 Sound EffectInstance

public:
	bool Init();
	void Update();
	void AllStop();
	void Restart();

	void CreateSoundEffect(const string& KeyName, const wstring& FileName, const string& PathKey = SOUND_PATH);
	void CreateBGMList(const string& KeyName, unique_ptr<SoundEffectInstance> instance);
	void RemoveBGMList(const string& KeyName);

	shared_ptr<SoundEffect>const& FindSoundEffect(const string& KeyName);
	shared_ptr<SoundEffectInstance>const& FindSoundEffectInstance(const string& KeyName);

private:
	unique_ptr<AudioEngine> m_AudioEngine;
	unordered_map<string, shared_ptr<SoundEffect>> m_SoundEffectMap;
	unordered_map<string, shared_ptr<SoundEffectInstance>> m_SoundEffectInstanceMap;

	static shared_ptr<SoundEffect> m_NULLPTR1;
	static shared_ptr<SoundEffectInstance> m_NULLPTR2;

private:
	CLASS_IN_SINGLE(SoundManager)
};

JEONG_END