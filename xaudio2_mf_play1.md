<!-- XAudio2とMediaFoundationを使ったWAV/MP3再生その1 -->

# はじめに
C#のSystem.Media.SoundPlayerやWindowsAPIのPlaySoundでは音声の同時再生ができません。
XAudio2を使えば同時再生ができるので再生するまでの手順をまとめます。
また、Media Foundationを使ってMP3の再生を行います。  
その1ではまずXAudio2を使ってWAVを再生します。  
その2ではMedia Foundationを使ってMP3を読み込み、そのデータをXAudio2で再生します。

XAudio2とMediaFoundationを使ったWAV/MP3再生その2  
https://qiita.com/ryo_shiraishi6352/items/d4a40521bdb09c838e9d

# 参考にしたサンプル
本記事は以下のサンプルを参考に参考にしています。  
https://github.com/walbourn/directx-sdk-samples/tree/main/XAudio2/XAudio2BasicSound  
WAVの読み込み部分は以下のWAVFileReader.cppをそのまま使わせてもらいます。  
https://github.com/walbourn/directx-sdk-samples/blob/main/XAudio2/Common/WAVFileReader.cpp

# 動作環境
Windows10 + Visual Studio Community 2022  
Windows7以前ではフラグの指定方法などが異なり動かない可能性が高いので注意してください。

# XAudio2の初期化
XAudio2を使用するには以下のヘッダファイルとlibファイルを必要とします。
```cpp
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
```

XAudio2の初期化をするにはCOMの初期化、XAudio2の初期化の順に行います。
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
マスターボイスはIXAudio2オブジェクトから生成します。
```cpp
IXAudio2MasteringVoice* pMasteringVoice{ nullptr };
pXAudio2->CreateMasteringVoice(&pMasteringVoice);
```

# WAV読み込み
WAVの読み込みはWAVFileReader.cppにあるDirectX::LoadWAVAudioFromFileExを使用します。  
DirectX::WAVData waveDataはstd::unique_ptr<uint8_t[]> waveFileが実体なので
waveFileを破棄すると使えなくなります。
```cpp
std::unique_ptr<uint8_t[]> waveFile{ nullptr };
DirectX::WAVData waveData{ 0 };
DirectX::LoadWAVAudioFromFileEx(path, waveFile, waveData);
```

# ソースボイスの作成
マスターボイスはIXAudio2オブジェクトから生成します。
```cpp
IXAudio2SourceVoice* pSourceVoice{ nullptr };
pXAudio2->CreateSourceVoice(&pSourceVoice, waveData.wfx);
```

# 音声再生
読み込んだ音声データをSubmitSourceBufferでセットしてStartで再生します。
```cpp
XAUDIO2_BUFFER buffer{ 0 };
buffer.pAudioData = waveData.startAudio;
buffer.Flags = XAUDIO2_END_OF_STREAM;
buffer.AudioBytes = waveData.audioBytes;
pSourceVoice->SubmitSourceBuffer(&buffer);

pSourceVoice->Start(0);
```

# 終了まで待機
再生状況を確認してBuffersQueuedが0になるまでプログラムが終了しないように待機します。  
BuffersQueuedはバッファの残数を返しています。  
今回では1つのバッファしか処理させていないので最初は1で再生が終わると0になるのでそれで判定しています。
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

# XAudio2の終了
作られた順番に破棄していきます。
```cpp
pSourceVoice->DestroyVoice();
pMasteringVoice->DestroyVoice();
pXAudio2->Release();
CoUninitialize();
```

# おわりに
チュートリアルとほぼ同じ手順でXAudio2を使ってWAVファイルの再生を行いました。
その2ではMedia Foundationを使ってMP3の再生を行います。  
今回のコードは以下のところにおいています。  
https://github.com/ryo-shiraishi6352/Articles/blob/main/XAudio2Play.h
