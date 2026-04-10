//
// Created by wifir on 2026-04-08.
//

#include "AudioManager.h"

#include <iostream>
#include <ostream>

MIX_Track* AudioManager::sfxTrack;
MIX_Track* AudioManager::musicTrack;
std::unordered_map<std::string, MIX_Audio*> AudioManager::audio;

/* Initializes the audio system, creates mixer device and sets up music/SFX tracks with default gain. */
AudioManager::AudioManager() {
    if (MIX_Init() == 0) {
        return;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer) {
        return;
    }

    musicTrack = MIX_CreateTrack(mixer);
    sfxTrack = MIX_CreateTrack(mixer);

    MIX_SetTrackGain(musicTrack, 0.5f);
    MIX_SetTrackGain(sfxTrack, 0.5f);
}

/* Loads an audio file into the audio map if it doesn't already exist. */
void AudioManager::loadAudio(const std::string &name, const char *path) const {
    if (audio.contains(name)) {
        return;
    }

    auto audioPtr = MIX_LoadAudio(mixer, path, true);
    if (!audioPtr) {
        return;
    }
    audio.emplace(name, audioPtr);
}

/* Plays the specified audio on the music track in an infinite loop. */
void AudioManager::playLooping(const std::string &name) {
    if (MIX_SetTrackAudio(musicTrack, audio[name]) == 0) {
        return;
    }

    SDL_PropertiesID properties = SDL_CreateProperties();
    SDL_SetNumberProperty(properties, MIX_PROP_PLAY_LOOPS_NUMBER, -1);

    MIX_PlayTrack(musicTrack, properties);
}

/* Stops the currently playing looping music track. */
void AudioManager::stopLooping() {
    MIX_StopTrack(musicTrack, 0);
}

/* Mutes the currently playing looping music track. */
void AudioManager::muteLooping() {
    MIX_SetTrackGain(musicTrack, 0.0f);
}

/* Restores the looping music track volume to its default level. */
void AudioManager::unmuteLooping() {
    MIX_SetTrackGain(musicTrack, 0.5f);
}

/* Plays a sound effect on the SFX track if it is not already playing. */
void AudioManager::playSfx(const std::string &name) {
    if (MIX_SetTrackAudio(sfxTrack, audio[name]) == 0) {
        return;
    }

    if (!MIX_TrackPlaying(sfxTrack)) {
        MIX_PlayTrack(sfxTrack, 0);
    }
}

/* Stops the currently playing sound effect track. */
void AudioManager::stopSfx() {
    MIX_StopTrack(sfxTrack, 0);
}
