//
// Created by wifir on 2026-04-08.
//

#include "AudioManager.h"

#include <iostream>
#include <ostream>

MIX_Track* AudioManager::sfxTrack;
MIX_Track* AudioManager::musicTrack;
std::unordered_map<std::string, MIX_Audio*> AudioManager::audio;

AudioManager::AudioManager() {
    if (MIX_Init() == 0) {
        std::cout << "Audio Error: Could not initialize SDL Mixer." << std::endl;
        return;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer) {
        std::cout << "Audio Error: Could not initialize SDL Mixer." << std::endl;
        return;
    }

    musicTrack = MIX_CreateTrack(mixer);
    sfxTrack = MIX_CreateTrack(mixer);

    MIX_SetTrackGain(musicTrack, 0.5f);
    MIX_SetTrackGain(sfxTrack, 0.5f);
}

void AudioManager::loadAudio(const std::string &name, const char *path) const {
    if (audio.contains(name)) {
        return;
    }

    auto audioPtr = MIX_LoadAudio(mixer, path, true);
    if (!audioPtr) {
        std::cout << "Audio Error: Could not load audio from \"" << path << "\"." << std::endl;
        return;
    }
    audio.emplace(name, audioPtr);
    std::cout << "Loaded audio from \"" << path << "\"." << std::endl;
}

void AudioManager::playLooping(const std::string &name) {
    if (MIX_SetTrackAudio(musicTrack, audio[name]) == 0) {
        std::cout << "Audio Error: Could not set track audio to \"" << name << "\"." << std::endl;
        return;
    }

    SDL_PropertiesID properties = SDL_CreateProperties();
    SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, -1);

    MIX_PlayTrack(musicTrack, properties);
    std::cout << "Now Playing: \"" << name << "\"." << std::endl;
}

void AudioManager::stopLooping() {
    MIX_StopTrack(musicTrack, 0);
}

void AudioManager::muteLooping() {
    MIX_SetTrackGain(musicTrack, 0.0f);
}

void AudioManager::unmuteLooping() {
    MIX_SetTrackGain(musicTrack, 0.5f);
}

void AudioManager::playSfx(const std::string &name) {
    if (MIX_SetTrackAudio(sfxTrack, audio[name]) == 0) {
        std::cout << "Audio Error: Could not set track audio to \"" << name << "\"." << std::endl;
        return;
    }

    if (!MIX_TrackPlaying(sfxTrack)) {
        MIX_PlayTrack(sfxTrack, 0);
    }
}

void AudioManager::stopSfx() {
    MIX_StopTrack(sfxTrack, 0);
}
