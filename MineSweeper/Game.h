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

#define TEST

enum Emoji
{
	HappyUp,
	HappyDown,
	Guess,
	Cool,
	Bad
};

/// Constants
const sf::Color HIGH_LIGHT_COLOUR = sf::Color::White;
const sf::Color SHADOW_COLOUR = sf::Color{ 96,96,96,255 };
const sf::Color GRAY_COLOUR = sf::Color{ 160,160,160,255 };;



const int MAX_MAP_WIDTH = 50;
const int MAX_MAP_HEIGHT = 50;
const float BORDER = 4.0f;



const float TEXTURE_TILE_WIDTH = 64.0f;
const float TEXTURE_TILE_HEIGHT = 64.0f;

const float DIGIT_WIDTH = 52.0f;
const float DIGIT_TEXTURE_WIDTH = 56.0f;
const float DIGIT_TEXTURE_HEIGHT = 92.0f;

const int TILES_PER_ROW = 10;
const sf::Vector2f BLANK_TILE_TOPLEFT = {128.0f, 64.0f};
const sf::Vector2f MINE_TILE_TOPLEFT = { 256.0f, 64.0f };
const sf::Vector2f EXPLODED_MINE_TILE_TOPLEFT = { 0.0f, 64.0f };
const sf::Vector2f QUESTION_TILE_TOPLEFT = { 192.0f, 64.0f };

const int QUESTION = 11;
const int FLAG = 10;
const int BLANK = -1;
const int MINE = 9;
const int EMPTY = 0;
const int INDEX_OFFSET = 78;
const sf::Vector2f FLAG_TILE_TOPLEFT = {64.0f, 64.0f };



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

	sf::Vector2f m_boardTopLeft = { 100.0f, 50.0f };
	sf::Vector2i m_boardTopLefti = static_cast<sf::Vector2i>(m_boardTopLeft);
	void showEmoji(int t_emoji);
	void emojiVertexSetup();
	void processEvents();
	void processKeys(sf::Event t_event);
	void processMouseDown(sf::Event t_event);
	void processMouseUp(sf::Event t_event);
	void resize(sf::Event t_event);
	void highLight(sf::Color t_colour, sf::Vector2i t_square);
	void showTile(sf::Vector2i t_square);
	void showSpecialTile(sf::Vector2i t_square, sf::Vector2f t_style);
	void clearTile(sf::Vector2i t_square);
	void clearTileAuto(sf::Vector2i t_square);
	void addQuad(sf::Vector2f t_a, sf::Vector2f t_b, sf::Vector2f t_c, sf::Vector2f t_d, sf::Color t_colour);
	void update(sf::Time t_deltaTime);
	void gameOver(bool t_victory);
	void render();
	void addDigitVertexs();
	void display(sf::Vector2f t_loction, int t_value, int t_offset);
	void displayRemaing(int t_value);
	void displayTime(int time);
	void drawDigit(sf::Vector2f t_loction, int t_value, int t_arrayOffset);
	int countFlags();
	void setupCorner();
	void setupFrame();
	void setupFontAndText();
	void loadTexture();
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

	float SPRITE_TILE_WIDTH = 35.0f;
	float SPRITE_TILE_HEIGHT = 35.0f;

	int mapWidth = 24;
	int mapHeight = 16;
	int mapMines = (mapHeight * mapWidth) / 8;
	int m_map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
	int m_playerMap[MAX_MAP_HEIGHT][MAX_MAP_WIDTH];
	sf::Vector2i m_currentSquare;
	int secondsElapsed = 0;
	sf::Time timeElapsed = sf::Time::Zero;
	bool m_gameOn = false;
};

#endif // !GAME_HPP

