/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>


/// Constants

const int MAX_MAP_WIDTH = 20;
const int MAX_MAP_HEIGHT = 10;
const sf::Vector2f TOP_LEFT = { 100.0f, 100.0f };

const float SPRITE_TILE_WIDTH = 50.0f;
const float SPRITE_TILE_HEIGHT = 50.0f;

const float TEXTURE_TILE_WIDTH = 32.0f;
const float TEXTURE_TILE_HEIGHT = 32.0f;


class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFontAndText();
	void setupSprite();
	void resetMap(int t_width, int t_height, int t_bombsCount);
	void resetArray(int t_width, int t_height);

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Texture m_tilesTexture; // texture used for sfml logo
	
	sf::VertexArray m_tilesArray{ sf::Triangles }; // vertex array for tiles
	bool m_exitGame; // control exiting game

	int mapWidth = 15;
	int mapHeight = 8;
	int m_map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
};

#endif // !GAME_HPP

