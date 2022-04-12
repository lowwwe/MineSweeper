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

const int MAX_MAP_WIDTH = 35;
const int MAX_MAP_HEIGHT = 25;
const sf::Vector2f TOP_LEFT = { 100.0f, 50.0f };
const sf::Vector2i TOP_LEFTi = static_cast<sf::Vector2i>( TOP_LEFT);

const float SPRITE_TILE_WIDTH = 35.0f;
const float SPRITE_TILE_HEIGHT = 35.0f;

const float TEXTURE_TILE_WIDTH = 32.0f;
const float TEXTURE_TILE_HEIGHT = 32.0f;

const sf::Vector2f BLANK = { 0.0f, 96.0f };
const sf::Vector2f FLAG = { 96.0f, 64.0f };
const sf::Vector2f QUESTION = { 32.0f, 96.0f };


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
	void processMouseDown(sf::Event t_event);
	void processMouseUp(sf::Event t_event);
	void highLight(sf::Color t_colour, sf::Vector2i t_square);
	void showTile(sf::Vector2i t_square);
	void showQuestion(sf::Vector2i t_square, sf::Vector2f t_style);
	void clearTile(sf::Vector2i t_square);
	void clearTileAuto(sf::Vector2i t_square);

	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFontAndText();
	void setupSprite();
	void resetMap(int t_width, int t_height, int t_bombsCount);
	void resetArray(int t_width, int t_height);
	void calculateMap(int t_width, int t_height);
	void clearSpace(sf::Vector2i t_square);
	void clearMap(sf::Vector2i t_square);
	int countFlags(sf::Vector2i t_square);
	void clearAdjacent(sf::Vector2i t_square);
	void highlightAdjacent(sf::Color t_colour, sf::Vector2i t_square);
	void highLightBlank(sf::Color t_colour, sf::Vector2i t_square);
	void highLightWhite(sf::Color t_colour, sf::Vector2i t_square);
	void highlightAdjacentWhite(sf::Color t_colur, sf::Vector2i t_square);
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Texture m_tilesTexture; // texture used for sfml logo
	
	sf::VertexArray m_tilesArray{ sf::Triangles }; // vertex array for tiles
	bool m_exitGame; // control exiting game

	int mapWidth = 15;
	int mapHeight = 8;
	int m_map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
	int m_playerMap[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
	sf::Vector2i m_currentSquare;
};

#endif // !GAME_HPP

