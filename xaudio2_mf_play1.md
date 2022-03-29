<!-- XAudio2とMediaFoundationを使ったWAV/MP3再生その1 -->

# はじめに
C#のSystem.Media.SoundPlayerやWindowsAPIのPlaySoundでは音声の同時再生ができません。
XAudio2を使えば同時再生ができるので再生するまでの手順をまとめます。
また、Media Foundationを使ってMP3の再生を行います。  
その1ではまずXAudio2を使ってWAVを再生します。  
その2ではMedia Foundationを使ってMP3を読み込み、そのデータをXAudio2で再生します。

# 参考にしたサンプル
本記事は以下のサンプルを参考に参考にしています。  
https://github.com/walbourn/directx-sdk-samples/tree/main/XAudio2/XAudio2BasicSound  
WAVの読み込み部分は以下のWAVFileReader.cppをそのまま使わせてもらいます。  
https://github.com/walbourn/directx-sdk-samples/blob/main/XAudio2/Common/WAVFileReader.cpp

# XAudio2の初期化
```cpp
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
```

```cpp
CoInitializeEx(nullptr, COINIT_MULTITHREADED);

IXAudio2* pXAudio2{ nullptr };
XAudio2Create(&pXAudio2, 0);
#if defined(_DEBUG)
XAUDIO2_DEBUG_CONFIGURATION debug{ 0 };
debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
debug.BreakMask = XAUDIO2_LOG_ERRORS;
pXAudio2->SetDebugConfiguration(&debug, 0);
#endif
```

# マスターボイスの作成
```cpp
IXAudio2MasteringVoice* pMasteringVoice{ nullptr };
pXAudio2->CreateMasteringVoice(&pMasteringVoice);
```

# WAV読み込み
```cpp
std::unique_ptr<uint8_t[]> waveFile{ nullptr };
DirectX::WAVData waveData{ 0 };
DirectX::LoadWAVAudioFromFileEx(path, waveFile, waveData);
```

# ソースボイスの作成
```cpp
IXAudio2SourceVoice* pSourceVoice{ nullptr };
pXAudio2->CreateSourceVoice(&pSourceVoice, waveData.wfx);
```

# 音声再生
```cpp
XAUDIO2_BUFFER buffer{ 0 };
buffer.pAudioData = waveData.startAudio;
buffer.Flags = XAUDIO2_END_OF_STREAM;
buffer.AudioBytes = waveData.audioBytes;
pSourceVoice->SubmitSourceBuffer(&buffer);

pSourceVoice->Start(0);
```

# 終了まで待機
```cpp
while (true)
{
    XAUDIO2_VOICE_STATE state{ 0 };
    pSourceVoice->GetState(&state);
    if (state.BuffersQueued == 0)
    {
        break;
    }
    Sleep(10);
}
```