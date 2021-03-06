//
// Created by Alexander Winter on 2022-02-11.
//

#ifndef GAMETEMPLATE_SCREEN_H
#define GAMETEMPLATE_SCREEN_H

#include "Game.h"
#include "SFML/Graphics/RenderTarget.hpp"

namespace wiz {
	class Screen;
}

class wiz::Screen {
	Game& game;

public:
	explicit Screen(Game& game);

	Screen(const Screen&) = delete;
	Screen& operator=(const Screen&) = delete;

	virtual ~Screen() = default;

	virtual void tick(float delta) = 0;
	virtual void render(sf::RenderTarget& target) = 0;

	virtual void show() = 0;
	virtual void hide() = 0;

	const Game& getGame() const;
	Game& getGame();

	Logger& getLogger();
	const Logger& getLogger() const;

	AssetLoader& getAssets();
	const AssetLoader& getAssets() const;

	sf::RenderWindow& getWindow();
	const sf::RenderWindow& getWindow() const;

	virtual const std::string& getName() const = 0;
};


#endif //GAMETEMPLATE_SCREEN_H
