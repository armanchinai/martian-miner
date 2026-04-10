//
// Created by wifir on 2026-04-08.
//

#ifndef MARTIAN_MINER_AUDIOMANAGER_H
#define MARTIAN_MINER_AUDIOMANAGER_H

#include <string>
#include <unordered_map>

#include "SDL3_mixer/SDL_mixer.h"

class AudioManager {
    MIX_Mixer *mixer = nullptr;
    static MIX_Track *musicTrack;
    static MIX_Track *sfxTrack;
    static std::unordered_map<std::string, MIX_Audio*> audio;

public:
    AudioManager();

    void loadAudio(const std::string& name, const char* path) const;
    static void playLooping(const std::string& name);
    static void muteLooping();
    static void unmuteLooping();
    static void stopLooping();
    static void playSfx(const std::string& name);
    static void stopSfx();
};

#endif //MARTIAN_MINER_AUDIOMANAGER_H
