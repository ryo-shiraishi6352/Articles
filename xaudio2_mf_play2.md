<!-- XAudio2とMediaFoundationを使ったWAV/MP3再生その2 -->

# はじめに
その1ではXAudio2を使ってWAV再生を行いました。
しかし、このままではWAV以外を再生できません。  
Media FoundationはMP3をサポートしており読み込むことができるので
XAudio2とMedia Foundationを使ってMP3の再生を行います。  

Media Foundationでサポートされるメディア形式  
https://docs.microsoft.com/ja-jp/windows/win32/medfound/supported-media-formats-in-media-foundation

# Media Foundationの初期化
Media Foundationを使用するために以下のヘッダファイルとlibファイルを使用します。
```cpp
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
```

Media Foundationの初期化をするにはCOMの初期化、Media Foundationの初期化の順に行います。
```cpp
CoInitializeEx(nullptr, COINIT_MULTITHREADED);

MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
```

# ソースリーダーの作成
ソースリーダーを作成します。
```cpp
IMFSourceReader* pMFSourceReader{ nullptr };
MFCreateSourceReaderFromURL(path.c_str(), NULL, &pMFSourceReader);
```

# メディアタイプの取得
メディアタイプを取得します。  
まず、ソースリーダーにPCMで読み込むために
MF_MT_MAJOR_TYPEにMFMediaType_Audio、MF_MT_SUBTYPEにMFAudioFormat_PCM
を指定してからソースリーダからメディアタイプを取得します。
```cpp
IMFMediaType* pMFMediaType{ nullptr };
MFCreateMediaType(&pMFMediaType);
pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
pMFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

pMFMediaType->Release();
pMFMediaType = nullptr;
pMFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);
```

# オーディオデータ形式の作成
メディアタイプからWAVEFORMATEXを生成します。
```cpp
WAVEFORMATEX* waveFormat{ nullptr };
MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);
```

# データの読み込み
ソースリーダーからサンプルを読み込んでvectorに格納していきます。
```cpp
std::vector<BYTE> mediaData;
while (true)
{
    IMFSample* pMFSample{ nullptr };
    DWORD dwStreamFlags{ 0 };
    pMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

    if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
    {
        break;
    }

    IMFMediaBuffer* pMFMediaBuffer{ nullptr };
    pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

    BYTE* pBuffer{ nullptr };
    DWORD cbCurrentLength{ 0 };
    pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

    mediaData.resize(mediaData.size() + cbCurrentLength);
    memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

    pMFMediaBuffer->Unlock();

    pMFMediaBuffer->Release();
    pMFSample->Release();
}
```

# XAudio2で再生
XAudio2の初期化などはその1とほとんど同じなので省略します。  
ソースボイスを生成して、データを与えます。
```cpp
IXAudio2SourceVoice* pSourceVoice{ nullptr };
pXAudio2->CreateSourceVoice(&pSourceVoice, waveFormat);

XAUDIO2_BUFFER buffer{ 0 };
buffer.pAudioData = mediaData.data();
buffer.Flags = XAUDIO2_END_OF_STREAM;
buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(mediaData.size());
pSourceVoice->SubmitSourceBuffer(&buffer);
```

# Media Foundationの終了
作られた順番に破棄していきます。
```cpp
CoTaskMemFree(waveFormat);
pMFMediaType->Release();
pMFSourceReader->Release();
MFShutdown();

CoUninitialize();
```
