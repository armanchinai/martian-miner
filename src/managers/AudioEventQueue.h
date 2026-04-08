//
// Created by wifir on 2026-04-08.
//

#ifndef MARTIAN_MINER_AUDIOEVENTQUEUE_H
#define MARTIAN_MINER_AUDIOEVENTQUEUE_H
#include <memory>
#include <queue>
#include <string>
#include <utility>

#include "AudioManager.h"

class AudioEvent {
public:
    explicit AudioEvent(std::string name) : name(std::move(name)) {}

    void execute() const {
        AudioManager::playSfx(name);
    }

private:
    std::string name;
};

class AudioEventQueue {
public:
    void push(std::unique_ptr<AudioEvent> event) {
        events.push(std::move(event));
    }

    void process() {
        while (!events.empty()) {
            events.front()->execute();
            events.pop();
        }
    }

private:
    std::queue<std::unique_ptr<AudioEvent>> events;
};

#endif //MARTIAN_MINER_AUDIOEVENTQUEUE_H
