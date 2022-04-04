#pragma once

#include <string>
#include <thread>

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include "WAVFileReader.h"

namespace XAudio2MFSample
{
	class XAudio2Play
	{
	public:
        void Play(std::wstring path)
        {
            CoInitializeEx(nullptr, COINIT_MULTITHREADED);

            IXAudio2* pXAudio2{ nullptr };
            XAudio2Create(&pXAudio2, 0);

#if defined(_DEBUG)
            XAUDIO2_DEBUG_CONFIGURATION debug{ 0 };
            debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
            debug.BreakMask = XAUDIO2_LOG_ERRORS;
            pXAudio2->SetDebugConfiguration(&debug, 0);
#endif

            IXAudio2MasteringVoice* pMasteringVoice{ nullptr };
            pXAudio2->CreateMasteringVoice(&pMasteringVoice);

            std::unique_ptr<uint8_t[]> waveFile{ nullptr };
            DirectX::WAVData waveData{ 0 };
            DirectX::LoadWAVAudioFromFileEx(path.c_str(), waveFile, waveData);

            IXAudio2SourceVoice* pSourceVoice{ nullptr };
            pXAudio2->CreateSourceVoice(&pSourceVoice, waveData.wfx);

            XAUDIO2_BUFFER buffer{ 0 };
            buffer.pAudioData = waveData.startAudio;
            buffer.Flags = XAUDIO2_END_OF_STREAM;
            buffer.AudioBytes = waveData.audioBytes;
            pSourceVoice->SubmitSourceBuffer(&buffer);

            pSourceVoice->Start(0);

            auto quit{ true };
            std::thread t([&quit]()
                {
                    while (true)
                    {
                        auto c{ getchar() };
                        if (c == 'q')
                        {
                            quit = false;
                        }
                    }
                });
            while (true)
            {
                if (!quit)
                {
                    break;
                }

                XAUDIO2_VOICE_STATE state{ 0 };
                pSourceVoice->GetState(&state);
                if (state.BuffersQueued == 0)
                {
                    break;
                }
                Sleep(10);
            }
            t.detach();

            pSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio2->Release();
            CoUninitialize();
		}
	};
}