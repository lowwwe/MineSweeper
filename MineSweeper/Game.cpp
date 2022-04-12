/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1200U, 600U, 32U }, "MineSweeper" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	resetMap(mapWidth, mapHeight, 10);
	resetArray(mapWidth, mapHeight);
	calculateMap(mapWidth, mapHeight);
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		switch (newEvent.type)
		{
		case sf::Event::Closed:
			m_exitGame = true;
			break;
		case sf::Event::KeyPressed:
			processKeys(newEvent);
			break;
		case sf::Event::MouseButtonPressed:
			processMouseDown(newEvent);
			break;
		case sf::Event::MouseButtonReleased:
			processMouseUp(newEvent);
			break;
		default:
			break;
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

void Game::processMouseDown(sf::Event t_event)
{
	sf::Vector2i square;
	square.y = (t_event.mouseButton.x - TOP_LEFTi.x) / SPRITE_TILE_WIDTH;
	square.x = (t_event.mouseButton.y - TOP_LEFTi.y) / SPRITE_TILE_HEIGHT;
	if (m_currentSquare != square)
	{
		highLight(sf::Color::White, m_currentSquare);
		highLight(sf::Color::Red, square);
		m_currentSquare = square;
	}
}

void Game::processMouseUp(sf::Event t_event)
{
	sf::Vector2i square;
	square.y = (t_event.mouseButton.x - TOP_LEFTi.x) / SPRITE_TILE_WIDTH;
	square.x = (t_event.mouseButton.y - TOP_LEFTi.y) / SPRITE_TILE_HEIGHT;
	if (m_currentSquare == square)
	{
		highLight(sf::Color::White, m_currentSquare);
		showTile(m_currentSquare);
	}
}

void Game::highLight(sf::Color t_colour, sf::Vector2i t_square)
{
	int index = (t_square.y  + t_square.x * mapWidth) * 6;
	for (int i = index; i < index + 6; i++)
	{
		m_tilesArray[i].color = t_colour;
	}
}

void Game::showTile(sf::Vector2i t_square)
{
	int index = (t_square.y + t_square.x * mapWidth) * 6;
	sf::Vector2f topLeft;
	topLeft.x = (m_map[t_square.x][t_square.y] % 4) * TEXTURE_TILE_WIDTH;
	topLeft.y = (m_map[t_square.x][t_square.y] / 4) * TEXTURE_TILE_HEIGHT;
	if (m_map[t_square.x][t_square.y] == 9)
	{
		topLeft = sf::Vector2f{ 64.0f,64.0f };
	}
	m_tilesArray[index++].texCoords = topLeft; // a
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{0.0f, TEXTURE_TILE_WIDTH }; //b
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_HEIGHT,0.0f }; //d;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_WIDTH }; //b;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_HEIGHT, TEXTURE_TILE_WIDTH }; //c;;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_HEIGHT, 0.0f}; //d;;

}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	m_window.draw(m_welcomeMessage);
	m_window.draw(m_tilesArray ,&m_tilesTexture);
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_tilesTexture.loadFromFile("ASSETS\\IMAGES\\SpriteSheet.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}	
}




void Game::resetMap(int t_width, int t_height, int t_bombsCount)
{
	int chance = t_width * t_height / t_bombsCount;
	int nextTile = 0;
	int bombsPlaced = 0;
	for (int i = 0; i < t_height; i++)
	{
		for (int j = 0; j < t_width; j++)
		{
			nextTile = std::rand() % chance;
			if (nextTile == 0 && bombsPlaced < t_bombsCount)
			{
				bombsPlaced++;
				m_map[i][j] = 9;
			}
			else
			{
				m_map[i][j] = 0;
			}			
		}
	}
	while (bombsPlaced != t_bombsCount)
	{
		for (int i = 0; i < t_height; i++)
		{
			for (int j = 0; j < t_width; j++)
			{
				nextTile = std::rand() % chance;
				if (nextTile == 0 && bombsPlaced < t_bombsCount)
				{
					bombsPlaced++;
					m_map[i][j] = 9;
				}				
			}
		}
	}
}

void Game::resetArray(int t_width, int t_height)
{
	sf::Vertex vertex;
	m_tilesArray.clear();
	vertex.color = sf::Color::White;
	for (int i = 0; i < t_height; i++)
	{
		for (int j = 0; j < t_width; j++)
		{
			vertex.position = TOP_LEFT + sf::Vector2f{ j * SPRITE_TILE_WIDTH, i * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ 32.0f,64.0f };
			m_tilesArray.append(vertex); //a

			vertex.position = TOP_LEFT + sf::Vector2f{ (j +1) * SPRITE_TILE_WIDTH, i * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ 64.0f,64.0f };
			m_tilesArray.append(vertex); //b

			vertex.position = TOP_LEFT + sf::Vector2f{ j * SPRITE_TILE_WIDTH, (i +1) * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ 32.0f,96.0f };
			m_tilesArray.append(vertex);//d

			vertex.position = TOP_LEFT + sf::Vector2f{ (j + 1) * SPRITE_TILE_WIDTH, i * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ 64.0f,64.0f };
			m_tilesArray.append(vertex); //b

			vertex.position = TOP_LEFT + sf::Vector2f{ (j + 1) * SPRITE_TILE_WIDTH, (i+1) * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ 64.0f,96.0f };
			m_tilesArray.append(vertex);//c

			vertex.position = TOP_LEFT + sf::Vector2f{ j * SPRITE_TILE_WIDTH, (i + 1) * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ 32.0f,96.0f };
			m_tilesArray.append(vertex);//d
		}
	}

}

void Game::calculateMap(int t_width, int t_height)
{
	int x=0;
	for (int i = 0; i < t_height; i++)
	{
		for (int j = 0; j < t_width; j++)
		{
			if (m_map[i][j] == 9)
			{
				if (j == 3) 
					x = 9;
				// top row
				x++;
				if (i > 0 && j > 0 && m_map[i - 1][j-1] != 9)
				{
					m_map[i - 1][j-1]++; //a
				}
				if (j > 0 && m_map[i][j - 1] != 9)
				{
					m_map[i][j - 1]++; //b
				}
				if (i > 0 && j > 0 && m_map[i- 1][j -  1] != 9)
				{
					m_map[i + 1][j - 1]++; //c
				}
				// middle row
				if (i > 0 && m_map[i - 1][j] != 9)
				{
					m_map[i - 1][j]++; //d
				}
				if ( i < t_width - 1 && m_map[i+1][j] != 9)
				{
					m_map[i + 1][j]++; //f
				}
				// bottom row
				if (i > 0 && j < t_width - 1  && m_map[i - 1][j + 1] != 9)
				{
					m_map[i - 1][j + 1]++; //g
				}
				if (j < t_width - 1 && m_map[i][j + 1] != 9)
				{
					m_map[i][j + 1]++; //h
				}
				if (i < t_height -1 && j < t_width - 1 && m_map[i + 1][j + 1] != 9)
				{
					m_map[i + 1][j + 1]++; //i
				}
			}
		}
	}
}
