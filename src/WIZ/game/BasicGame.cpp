//
// Created by Alexander Winter on 2022-02-11.
//

#include "WIZ/game/BasicGame.h"
#include "WIZ/game/Screen.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "WIZ/logging/Logger.h"

#include <utility>


wiz::BasicGame::BasicGame(sf::RenderWindow* window,
						  wiz::Logger* logger,
						  wiz::AssetLoader* assetLoader)
	: BasicGame(std::shared_ptr<sf::RenderWindow>(window),
	        	std::shared_ptr<wiz::Logger>(logger),
	        	std::shared_ptr<wiz::AssetLoader>(assetLoader)) {}

wiz::BasicGame::BasicGame(std::shared_ptr<sf::RenderWindow> window,
						  std::shared_ptr<wiz::Logger> logger,
						  std::shared_ptr<wiz::AssetLoader> assetLoader)
		: window(std::move(window)),
			logger(std::move(logger)),
			assetLoader(std::move(assetLoader)) {}

void wiz::BasicGame::update() {
	sf::Event event;
	while(window->pollEvent(event)) {

		tmpInputListeners.clear();
		for (InputListener* listener : inputListeners)
			tmpInputListeners.insert(listener);

		tmpWindowListeners.clear();
		for (WindowListener* listener : windowListeners)
			tmpWindowListeners.insert(listener);

		switch (event.type) {
			case sf::Event::Closed:
				for (WindowListener* listener : tmpWindowListeners)
					listener->windowClosed();
				break;

			case sf::Event::Resized:
				for (WindowListener* listener : tmpWindowListeners)
					listener->windowResized(event.size);
				break;

			case sf::Event::LostFocus:
				for (WindowListener* listener : tmpWindowListeners)
					listener->windowLostFocus();
				break;

			case sf::Event::MouseEntered:
				for (WindowListener* listener : tmpWindowListeners)
					listener->mouseEntered();
				break;

			case sf::Event::MouseLeft:
				for (WindowListener* listener : tmpWindowListeners)
					listener->mouseExited();
				break;

			case sf::Event::GainedFocus:
				for (WindowListener* listener : tmpWindowListeners)
					listener->windowGainedFocus();
				break;

			case sf::Event::TextEntered:
				for (InputListener* listener : tmpInputListeners)
					listener->textEntered(event.text);
				break;

			case sf::Event::KeyPressed:
				for (InputListener* listener : tmpInputListeners)
					listener->keyPressed(event.key);
				break;

			case sf::Event::KeyReleased:
				for (InputListener* listener : tmpInputListeners)
					listener->keyReleased(event.key);
				break;

			case sf::Event::MouseWheelScrolled:
				for (InputListener* listener : tmpInputListeners)
					listener->mouseWheelScrolled(event.mouseWheelScroll);
				break;

			case sf::Event::MouseButtonPressed:
				for (InputListener* listener : tmpInputListeners)
					listener->mouseButtonPressed(event.mouseButton);
				break;

			case sf::Event::MouseButtonReleased:
				for (InputListener* listener : tmpInputListeners)
					listener->mouseButtonReleased(event.mouseButton);
				break;

			case sf::Event::MouseMoved:
				for (InputListener* listener : tmpInputListeners)
					listener->mouseMoved(event.mouseMove);
				break;

			case sf::Event::JoystickButtonPressed:
				for (InputListener* listener : tmpInputListeners)
					listener->joystickButtonPressed(event.joystickButton);
				break;

			case sf::Event::JoystickButtonReleased:
				for (InputListener* listener : tmpInputListeners)
					listener->joystickButtonReleased(event.joystickButton);
				break;

			case sf::Event::JoystickMoved:
				for (InputListener* listener : tmpInputListeners)
					listener->joystickMoved(event.joystickMove);
				break;

			case sf::Event::JoystickConnected:
				for (InputListener* listener : tmpInputListeners)
					listener->joystickConnected(event.joystickConnect);
				break;

			case sf::Event::JoystickDisconnected:
				for (InputListener* listener : tmpInputListeners)
					listener->joystickDisconnected(event.joystickConnect);
				break;

			case sf::Event::TouchBegan:
				for (InputListener* listener : tmpInputListeners)
					listener->touchBegan(event.touch);
				break;

			case sf::Event::TouchMoved:
				for (InputListener* listener : tmpInputListeners)
					listener->touchMoved(event.touch);
				break;

			case sf::Event::TouchEnded:
				for (InputListener* listener : tmpInputListeners)
					listener->touchEnded(event.touch);
				break;

			case sf::Event::SensorChanged:
				for (InputListener* listener : tmpInputListeners)
					listener->sensorChanged(event.sensor);
				break;

			case sf::Event::Count:
				logger->error("Received COUNT event from window");
				break;
		}
	}

	if(currentScreen != nullptr) {
		float delta;

		std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();

		if(lastUpdate.time_since_epoch().count() == 0) {
			delta = 0.0f;
		} else {
			std::chrono::duration<double> diff = now - lastUpdate;
			delta = static_cast<float>(diff.count() * 1000.0);
		}

		currentScreen->tick(delta);
		lastUpdate = now;

		for(const std::function<void()>& run : runnables)
			run();
		runnables.clear();

		currentScreen->render(*window);
		window->display();
	} else {
		window->clear();
		window->display();
	}
}

wiz::Screen& wiz::BasicGame::getScreen() {
	return *currentScreen;
}

const wiz::Screen& wiz::BasicGame::getScreen() const {
	return *currentScreen;
}

void wiz::BasicGame::setScreen(std::shared_ptr<wiz::Screen> screen) {
	if(this->currentScreen != nullptr) {
		this->logger->info("Hiding screen " + this->currentScreen->getName());
		this->currentScreen->hide();
	}

	this->currentScreen = std::move(screen);

	if(this->currentScreen != nullptr) {
		this->logger->info("Showing screen " + this->currentScreen->getName());
		this->currentScreen->show();
	}
}

void wiz::BasicGame::setScreen(wiz::Screen* screen) {
	setScreen(std::shared_ptr<wiz::Screen>(screen));
}

wiz::Logger& wiz::BasicGame::getLogger() {
	return *logger;
}

const wiz::Logger& wiz::BasicGame::getLogger() const {
	return *logger;
}

sf::RenderWindow& wiz::BasicGame::getWindow() {
	return *window;
}

const sf::RenderWindow& wiz::BasicGame::getWindow() const {
	return *window;
}

wiz::AssetLoader& wiz::BasicGame::getAssets() {
	return *assetLoader;
}

const wiz::AssetLoader& wiz::BasicGame::getAssets() const {
	return *assetLoader;
}

void wiz::BasicGame::addInputListener(wiz::InputListener* listener) {
	inputListeners.emplace(listener);
}

void wiz::BasicGame::removeInputListener(wiz::InputListener* listener) {
	inputListeners.erase(listener);
}

void wiz::BasicGame::addWindowListener(wiz::WindowListener* listener) {
	windowListeners.emplace(listener);
}

void wiz::BasicGame::removeWindowListener(wiz::WindowListener* listener) {
	windowListeners.erase(listener);
}

void wiz::BasicGame::postRunnable(const std::function<void()>& runnable) {
	runnables.push_back(runnable);
}
