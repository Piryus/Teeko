#ifndef TEEKO_SOUNDMANAGER_H
#define TEEKO_SOUNDMANAGER_H

#include "SFML/Audio.hpp"

class SoundManager {
public:
    SoundManager();
    void PlayMarkerSound();
    void PlayVictoryMusic();
private:
    sf::SoundBuffer marker_sound_buffer_;
    sf::Sound marker_sound_;
    sf::Sound victory_music_;
    sf::SoundBuffer victory_music_buffer_;
    bool victory_sound_played_;
};


#endif //TEEKO_SOUNDMANAGER_H
