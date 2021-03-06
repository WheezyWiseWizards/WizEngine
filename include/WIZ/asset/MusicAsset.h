//
// Created by Alexander Winter on 2022-02-11.
//

#ifndef GAMETEMPLATE_MUSICASSET_H
#define GAMETEMPLATE_MUSICASSET_H


#include "SFML/Audio/Music.hpp"
#include "SinglePathAsset.h"

namespace wiz {
	class MusicAsset;
}

class wiz::MusicAsset : public wiz::SinglePathAsset<sf::Music> {
public:
    MusicAsset(const std::string& path);

    void* load() const override;
};


#endif //GAMETEMPLATE_MUSICASSET_H
