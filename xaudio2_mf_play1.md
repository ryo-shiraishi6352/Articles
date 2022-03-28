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
