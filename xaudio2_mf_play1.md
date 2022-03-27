<!-- XAudio2とMediaFoundationを使ったWAV/MP3再生その1 -->

# はじめに
C#のSystem.Media.SoundPlayerやWindowsAPIのPlaySoundでは音声の同時再生ができません。
XAudio2を使えば同時再生ができるので再生するまでの手順をまとめます。
また、Media Foundationを使ってMP3の再生を行います。  
その1ではまずXAudio2を使ってWAVを再生します。  
その2ではMedia Foundationを使ってMP3を読み込み、そのデータをXAudio2で再生します。
