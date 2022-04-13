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
	loadTexture(); // load texture
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
	if (t_event.mouseButton.x - TOP_LEFTi.x > 0.0f && t_event.mouseButton.y - TOP_LEFTi.y > 0.0f)
	{
		square.y = (t_event.mouseButton.x - TOP_LEFTi.x) / SPRITE_TILE_WIDTH;
		square.x = (t_event.mouseButton.y - TOP_LEFTi.y) / SPRITE_TILE_HEIGHT;
		if (sf::Mouse::Button::Middle == t_event.mouseButton.button)
		{
			highlightAdjacent(sf::Color::Blue,square);
		}
		else
		{
			if (square.x < mapHeight && square.y < mapWidth)
			{
				if (m_currentSquare != square)
				{
					highLight(sf::Color::White, m_currentSquare);
					highLight(sf::Color::Red, square);
					m_currentSquare = square;
				}
			}
		}
	}
}

void Game::processMouseUp(sf::Event t_event)
{
	sf::Vector2i square;
	if (t_event.mouseButton.x - TOP_LEFTi.x > 0.0f && t_event.mouseButton.y - TOP_LEFTi.y > 0.0f)
	{
		square.y = (t_event.mouseButton.x - TOP_LEFTi.x) / SPRITE_TILE_WIDTH;
		square.x = (t_event.mouseButton.y - TOP_LEFTi.y) / SPRITE_TILE_HEIGHT;
		if ( square.x < mapHeight && square.y < mapWidth)
		{
			if (sf::Mouse::Button::Middle == t_event.mouseButton.button)
			{
				int count = countFlags(square);
				if (count == m_playerMap[square.x][square.y])
				{
					clearAdjacent(square);
				}
				highlightAdjacentWhite(sf::Color::White, square);
			}
			if (sf::Mouse::Button::Left == t_event.mouseButton.button)
			{
				if (m_currentSquare == square)
				{
					highLight(sf::Color::White, m_currentSquare);
					m_playerMap[square.x][square.y] = m_map[square.x][square.y];
					showTile(m_currentSquare);
					if (m_map[square.x][square.y] == EMPTY)
					{
						clearSpace(square);
					}
					if (m_map[square.x][square.y] == MINE)
					{
						clearMap(square);
					}
				}
				else
				{
					highLight(sf::Color::White, m_currentSquare);
				}
			}
			if (sf::Mouse::Button::Right == t_event.mouseButton.button)
			{
				if (m_currentSquare == square)
				{
					if (QUESTION == m_playerMap[square.x][square.y])
					{
						highLight(sf::Color::White, m_currentSquare);
						showSpecialTile(m_currentSquare, BLANK_TILE_TOPLEFT);
						m_playerMap[square.x][square.y] = BLANK;
					}
					else
					{
						if (FLAG == m_playerMap[square.x][square.y])
						{
							highLight(sf::Color::White, m_currentSquare);
							showSpecialTile(m_currentSquare, QUESTION_TILE_TOPLEFT);
							m_playerMap[square.x][square.y] = QUESTION;
						}
						else
						{
							if (BLANK == m_playerMap[square.x][square.y])
							{
								highLight(sf::Color::White, m_currentSquare);
								showSpecialTile(m_currentSquare, FLAG_TILE_TOPLEFT);
								m_playerMap[square.x][square.y] = FLAG;
							}
						}
					}
					
					
				}
				else
				{
					highLight(sf::Color::White, m_currentSquare);
				}
			}
		}
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
	topLeft.x = (m_map[t_square.x][t_square.y] % TILES_PER_ROW) * TEXTURE_TILE_WIDTH;
	topLeft.y = (m_map[t_square.x][t_square.y] / TILES_PER_ROW) * TEXTURE_TILE_HEIGHT;

	m_tilesArray[index++].texCoords = topLeft; // a
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{TEXTURE_TILE_WIDTH,0.0f }; //b
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_HEIGHT, TEXTURE_TILE_WIDTH }; //c;;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d;;

}

void Game::showSpecialTile(sf::Vector2i t_square, sf::Vector2f t_style)
{
	int index = (t_square.y + t_square.x * mapWidth) * 6;
	sf::Vector2f topLeft;
	topLeft = t_style;
	
	m_tilesArray[index++].texCoords = topLeft; // a
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_HEIGHT, TEXTURE_TILE_WIDTH }; //c;;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d;;
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
void Game::loadTexture()
{
	if (!m_tilesTexture.loadFromFile("ASSETS\\IMAGES\\newspritesheet.png"))
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
				m_map[i][j] = MINE;
			}
			else
			{
				m_map[i][j] = EMPTY;
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
					m_map[i][j] = MINE;
				}				
			}
		}
	}
}

void Game::resetArray(int t_width, int t_height)
{
	sf::Vertex vertex;
	float left = BLANK_TILE_TOPLEFT.x;
	float top = BLANK_TILE_TOPLEFT.y;
	m_tilesArray.clear();
	vertex.color = sf::Color::White;
	for (int i = 0; i < t_height; i++)
	{
		for (int j = 0; j < t_width; j++)
		{
			vertex.position = BOARD_TOP_LEFT + sf::Vector2f{ j * SPRITE_TILE_WIDTH, i * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left , top };
			m_tilesArray.append(vertex); //a

			vertex.position = BOARD_TOP_LEFT + sf::Vector2f{ (j +1) * SPRITE_TILE_WIDTH, i * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left +TEXTURE_TILE_WIDTH , top  };
			m_tilesArray.append(vertex); //b

			vertex.position = BOARD_TOP_LEFT + sf::Vector2f{ j * SPRITE_TILE_WIDTH, (i +1) * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left  , top + TEXTURE_TILE_HEIGHT };
			m_tilesArray.append(vertex);//d

			vertex.position = BOARD_TOP_LEFT + sf::Vector2f{ (j + 1) * SPRITE_TILE_WIDTH, i * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left + TEXTURE_TILE_WIDTH , top  };
			m_tilesArray.append(vertex); //b

			vertex.position = BOARD_TOP_LEFT + sf::Vector2f{ (j + 1) * SPRITE_TILE_WIDTH, (i+1) * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left + TEXTURE_TILE_WIDTH , top + TEXTURE_TILE_HEIGHT };
			m_tilesArray.append(vertex);//c

			vertex.position = BOARD_TOP_LEFT + sf::Vector2f{ j * SPRITE_TILE_WIDTH, (i + 1) * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left  , top + TEXTURE_TILE_WIDTH };
			m_tilesArray.append(vertex);//d
		}
	}

}

void Game::calculateMap(int t_width, int t_height)
{
	for (int i = 0; i < t_height; i++)
	{
		for (int j = 0; j < t_width; j++)
		{
			m_playerMap[i][j] = -1;
			if (m_map[i][j] == MINE)
			{
				// top row
				if (i > 0 && j > 0 && m_map[i - 1][j-1] != MINE)
				{
					m_map[i - 1][j-1]++; //a
				}
				if (i > 0 && m_map[i - 1][j] != MINE)
				{
					m_map[i - 1][j]++; //b
				}
				if (i > 0 && j < t_width - 1 && m_map[i - 1][j + 1] != MINE)
				{
					m_map[i - 1][j + 1]++; //c
				}
				// middle row
				if (j > 0 && m_map[i][j - 1] != MINE)
				{
					m_map[i][j - 1]++; //d
				}
				if (j < t_width - 1 && m_map[i][j + 1] != MINE)
				{
					m_map[i][j + 1]++; //h
				}
				// bottom row

				if (i < t_height - 1 && j > 0 && m_map[i+ 1][j -  1] != MINE)
				{
					m_map[i + 1][j - 1]++; //g
				}				
				if ( i < t_height - 1 && m_map[i+1][j] != MINE)
				{
					m_map[i + 1][j]++; //f
				}				
				if (i < t_height -1 && j < t_width - 1 && m_map[i + 1][j + 1] != MINE)
				{
					m_map[i + 1][j + 1]++; //i
				}
			}
		}
	}
}

void Game::clearSpace(sf::Vector2i t_square)
{
	m_playerMap[t_square.x][t_square.y] = 0;
	if (t_square.x > 0 && m_playerMap[t_square.x - 1][t_square.y] == -1)
	{
		clearTile(sf::Vector2i{ t_square.x - 1, t_square.y });
	}
	if (t_square.y > 0 && m_playerMap[t_square.x][t_square.y-1] == -1)
	{
		clearTile(sf::Vector2i{ t_square.x, t_square.y-1 });
	}
	if (t_square.x > 0 && t_square.y > 0 && m_playerMap[t_square.x-1][t_square.y - 1] == -1)
	{
		clearTile(sf::Vector2i{ t_square.x -1, t_square.y - 1 });
	}

	if (t_square.x < mapHeight -1 && m_playerMap[t_square.x + 1][t_square.y] == -1)
	{
		clearTile(sf::Vector2i{ t_square.x + 1, t_square.y });
	}
	if (t_square.y < mapWidth -1 && m_playerMap[t_square.x][t_square.y + 1] == -1)
	{
		clearTile(sf::Vector2i{ t_square.x, t_square.y + 1 });
	}
	if (t_square.x < mapHeight - 1 && t_square.y < mapWidth-1 && m_playerMap[t_square.x+1][t_square.y + 1] == -1)
	{
		clearTile(sf::Vector2i{ t_square.x + 1, t_square.y + 1 });
	}

	if (t_square.x > 0 && t_square.y < mapWidth-1 && m_playerMap[t_square.x-1][t_square.y + 1] == -1)
	{
		clearTile(sf::Vector2i{ t_square.x - 1, t_square.y + 1 });
	}
	
	if (t_square.x < mapHeight - 1 && t_square.y >0 && m_playerMap[t_square.x+1][t_square.y - 1] == -1)
	{
		clearTile(sf::Vector2i{ t_square.x + 1, t_square.y - 1 });
	}

}

void Game::clearMap(sf::Vector2i t_square)
{
	int index = 0;
	sf::Vector2f topLeft;
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			topLeft.x = (m_map[i][j] % TILES_PER_ROW) * TEXTURE_TILE_WIDTH;
			topLeft.y = (m_map[i][j] / TILES_PER_ROW) * TEXTURE_TILE_HEIGHT;
			if (m_map[i][j] == MINE)
			{
				topLeft = MINE_TILE_TOPLEFT;
			}
			index = (j * 6) + (i * mapWidth * 6);
			m_tilesArray[index++].texCoords = topLeft; // a
			m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b
			m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d;
			m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b;
			m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_HEIGHT, TEXTURE_TILE_WIDTH }; //c;;
			m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d;;
		}
	}
	topLeft = EXPLODED_MINE_TILE_TOPLEFT;

	index = (t_square.y * 6) + (t_square.x * mapWidth * 6);
	m_tilesArray[index++].texCoords = topLeft; // a
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_HEIGHT, TEXTURE_TILE_WIDTH }; //c;;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d

}

int Game::countFlags(sf::Vector2i t_square)
{
	int count=0;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (10 == m_playerMap[t_square.x + i][t_square.y + j])
			{
				count++;
			}
		}
	}
	std::cout << count << " flags" << std::endl;
	return count;
}

void Game::clearAdjacent(sf::Vector2i t_square)
{
	
		clearTileAuto(t_square + sf::Vector2i{ -1, -1 });
		clearTileAuto(t_square + sf::Vector2i{ -1, 0 });
		clearTileAuto(t_square + sf::Vector2i{ -1, +1 });
		clearTileAuto(t_square + sf::Vector2i{ 0, -1 });
		clearTileAuto(t_square + sf::Vector2i{ 0, +1 });
		clearTileAuto(t_square + sf::Vector2i{ 1, -1 });
		clearTileAuto(t_square + sf::Vector2i{ 1, 0 });
		clearTileAuto(t_square + sf::Vector2i{ 1, +1 });
	

}

void Game::highlightAdjacentWhite(sf::Color t_colur, sf::Vector2i t_square)
{
	highLightWhite(t_colur, t_square + sf::Vector2i{ -1, -1 });
	highLightWhite(t_colur, t_square + sf::Vector2i{ -1, 0 });
	highLightWhite(t_colur, t_square + sf::Vector2i{ -1, +1 });
	highLightWhite(t_colur, t_square + sf::Vector2i{ 0, -1 });
	highLightWhite(t_colur, t_square + sf::Vector2i{ 0, +1 });
	highLightWhite(t_colur, t_square + sf::Vector2i{ 1, -1 });
	highLightWhite(t_colur, t_square + sf::Vector2i{ 1, 0 });
	highLightWhite(t_colur, t_square + sf::Vector2i{ 1, +1 });
}

void Game::highlightAdjacent(sf::Color t_colur, sf::Vector2i t_square)
{
	highLightBlank(t_colur, t_square + sf::Vector2i{ -1, -1 });
	highLightBlank(t_colur, t_square + sf::Vector2i{ -1, 0 });
	highLightBlank(t_colur, t_square + sf::Vector2i{ -1, +1 });
	highLightBlank(t_colur, t_square + sf::Vector2i{ 0, -1 });
	highLightBlank(t_colur, t_square + sf::Vector2i{ 0, +1 });
	highLightBlank(t_colur, t_square + sf::Vector2i{ 1, -1 });
	highLightBlank(t_colur, t_square + sf::Vector2i{ 1, 0 });
	highLightBlank(t_colur, t_square + sf::Vector2i{ 1, +1 });
}

void Game::highLightBlank(sf::Color t_colour, sf::Vector2i t_square)
{
	if (t_square.x > -1 && t_square.x < mapHeight &&
		t_square.y > -1 && t_square.y < mapWidth &&
		-1 == m_playerMap[t_square.x][t_square.y] )
	{
		highLight(t_colour, t_square);
	}
}

void Game::highLightWhite(sf::Color t_colour, sf::Vector2i t_square)
{
	if (t_square.x > -1 && t_square.x < mapHeight &&
		t_square.y > -1 && t_square.y < mapWidth)
	{
		highLight(t_colour, t_square);
	}
}

void Game::clearTile(sf::Vector2i t_square)
{
	m_playerMap[t_square.x][t_square.y] = m_map[t_square.x][t_square.y];
	showTile(t_square);
	if (m_playerMap[t_square.x][t_square.y] == 0)
	{
		clearSpace(sf::Vector2i{ t_square.x , t_square.y });
	}
}

void Game::clearTileAuto(sf::Vector2i t_square)
{
	if (t_square.x > -1 && t_square.x < mapHeight &&
		t_square.y > -1 && t_square.y < mapWidth &&
		10 != m_playerMap[t_square.x][t_square.y] )
	{
		m_playerMap[t_square.x][t_square.y] = m_map[t_square.x][t_square.y];
		showTile(t_square);
		if (m_playerMap[t_square.x][t_square.y] == 0)
		{
			clearSpace(sf::Vector2i{ t_square.x , t_square.y });
		}

	}
}
