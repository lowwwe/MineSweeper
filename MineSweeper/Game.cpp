/// <summary>
/// @author Peter Lowe
/// @date April 2022
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
	m_window{ sf::VideoMode{ 1200U, 800U, 32U }, "MineSweeper" },
	m_exitGame{false} //when true game will exit
{
	loadTexture(); // load texture
	setupFontAndText(); // load font 

	addDigitVertexs();
	setupCorner();
	resetMap(mapWidth, mapHeight, mapMines);
	resetArray(mapWidth, mapHeight);
	calculateMap(mapWidth, mapHeight);
	setupFrame();
	emojiVertexSetup();
	
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
	int draws = 0;
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
		//	std::cout << "rendering at " << draws * 60 << " frames per second \r"  ;
			draws = 0;
		}
		render(); // as many as possible
		draws++;
	}
}
void Game::showEmoji(int t_emoji)
{
	float emojiWidth = 96.0f;
	unsigned left = m_window.getSize().x / 2 + emojiWidth/ 2;
	int index = INDEX_OFFSET - 6;
	sf::Vector2f topLeft = { 0.0f + t_emoji * 100.0f, 225.0f };

	m_tilesArray[index++].texCoords = topLeft; // a
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ emojiWidth,0.0f }; //b
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, emojiWidth }; //d;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ emojiWidth,0.0f }; //b;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ emojiWidth, emojiWidth }; //c;;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, emojiWidth }; //d;;
}
void Game::emojiVertexSetup()
{
	float emojiWidth = 96.0f;
	unsigned left = m_window.getSize().x / 2 - emojiWidth / 2;
	int index = INDEX_OFFSET - 6;
	sf::Vector2f topLeft = { static_cast<float>(left), 40.0f };


	m_tilesArray[index++].position = topLeft; // a
	m_tilesArray[index++].position = topLeft + sf::Vector2f{ emojiWidth,0.0f }; //b
	m_tilesArray[index++].position = topLeft + sf::Vector2f{ 0.0f, emojiWidth }; //d;
	m_tilesArray[index++].position = topLeft + sf::Vector2f{ emojiWidth,0.0f }; //b;
	m_tilesArray[index++].position = topLeft + sf::Vector2f{ emojiWidth, emojiWidth }; //c;;
	m_tilesArray[index++].position = topLeft + sf::Vector2f{ 0.0f, emojiWidth }; //d;
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
		case::sf::Event::Resized:
			resize(newEvent);
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
	if (t_event.mouseButton.x - m_boardTopLefti.x > 0.0f && t_event.mouseButton.y - m_boardTopLefti.y > 0.0f)
	{
		square.y = (t_event.mouseButton.x - m_boardTopLefti.x) / SPRITE_TILE_WIDTH;
		square.x = (t_event.mouseButton.y - m_boardTopLefti.y) / SPRITE_TILE_HEIGHT;
#ifdef TEST
		std::cout << "clicking on " << square.x << " , " << square.y << std::endl;
#endif // TEST

		if (square.x < mapHeight && square.y < mapWidth)
		{
			showEmoji(Emoji::HappyDown);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
				sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
			{
				highlightAdjacent(sf::Color::Blue, square);
			}
			if (sf::Mouse::Button::Middle == t_event.mouseButton.button)
			{
				highlightAdjacent(sf::Color::Blue, square);
			}
			else
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
	showEmoji(Emoji::HappyUp);
	if (t_event.mouseButton.x - m_boardTopLefti.x > 0.0f && t_event.mouseButton.y - m_boardTopLefti.y > 0.0f)
	{
		square.y = (t_event.mouseButton.x - m_boardTopLefti.x) / SPRITE_TILE_WIDTH;
		square.x = (t_event.mouseButton.y - m_boardTopLefti.y) / SPRITE_TILE_HEIGHT;
#ifdef TEST
		std::cout << "releasing  on " << square.x << " , " << square.y << std::endl;
#endif // TEST

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

void Game::resize(sf::Event t_event)
{
	bool tooSmall = false;
	int width = t_event.size.width;
	int height = t_event.size.height;
	mapWidth = width / 50;
	mapHeight = height / 50;
	mapMines = (mapHeight * mapWidth) / 10;
	sf::FloatRect visibleArea(0.0f, 0.0f, static_cast<float>( t_event.size.width), static_cast<float>(t_event.size.height));
	m_window.setView(sf::View(visibleArea));
	addDigitVertexs();
	setupCorner();
	resetMap(mapWidth, mapHeight, mapMines);
	resetArray(mapWidth, mapHeight);
	calculateMap(mapWidth, mapHeight);
	setupFrame();
	emojiVertexSetup();
	if (width < 500)
	{
		tooSmall = true;
		width = 500u;		
	}
	if (height < 500)
	{
		tooSmall = true;
		height = 500u;
	}
	if (tooSmall)
	{
		m_window.setSize(sf::Vector2u(width, height));
	}

}

void Game::highLight(sf::Color t_colour, sf::Vector2i t_square)
{
	int index = (t_square.y  + t_square.x * mapWidth) * 6 + INDEX_OFFSET;
	for (int i = index; i < index + 6; i++)
	{
		m_tilesArray[i].color = t_colour;
	}
}

void Game::showTile(sf::Vector2i t_square)
{
	int index = (t_square.y + t_square.x * mapWidth) * 6 + INDEX_OFFSET;
	sf::Vector2f topLeft;
	m_gameOn = true;
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
	int index = (t_square.y + t_square.x * mapWidth) * 6 + INDEX_OFFSET;
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
	int remaining = mapMines - countFlags();
	if (m_exitGame)
	{
		m_window.close();
	}
	if (m_gameOn)
	{
		timeElapsed += t_deltaTime;
	}
	secondsElapsed = timeElapsed.asSeconds();
	displayTime(secondsElapsed);
	displayRemaing(remaining);
	if (remaining == 0)
	{
		gameOver(true);
		showEmoji(Emoji::Cool);
	}
	
}

void Game::gameOver(bool t_victory)
{
	m_gameOn = false;
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(GRAY_COLOUR);
	//m_window.draw(m_welcomeMessage);
	m_window.draw(m_tilesArray ,&m_tilesTexture);
	m_window.display();
}

void Game::addDigitVertexs()
{
	sf::Vertex vertex;
	m_tilesArray.clear();
	vertex.color = sf::Color::White;
	for (int i = 0; i < INDEX_OFFSET; i++)
	{
		m_tilesArray.append(vertex);
	}
}

void Game::display(sf::Vector2f t_loction, int t_value, int t_offset)
{
	int first = 0;
	int second = 0;
	int third = 0;
	int value = t_value;


	first = t_value % 10;
	if (first == 0)
	{
		first = 10;
	}
	t_value = t_value / 10;
	second = t_value % 10;
	if (second == 0)
	{
		second = 10;
	}
	t_value = t_value / 10;
	third = t_value % 10;
	if (third == 0)
	{
		third = 10;
	}
	if (value < 100)
	{
		third = 12;
	}
	if (value < 10)
	{
		second = 12;
	}
	drawDigit(t_loction, third, t_offset);
	drawDigit(t_loction + sf::Vector2f{52.0f, 0.0f}, second, t_offset + 6);
	drawDigit(t_loction + sf::Vector2f{104.0f, 0.0f},  first, t_offset + 12);

}
void Game::displayRemaing(int t_value)
{
	display(sf::Vector2f{ 40.0f,40.0f }, t_value,0);
}

void Game::displayTime(int t_time)
{
	float left = m_window.getSize().x - 3 * 52.0f - 40.0f;
	display(sf::Vector2f{ left,40.0f }, t_time, 36);
}

void Game::drawDigit(sf::Vector2f t_loction, int t_value, int t_arrayOffset)
{
	sf::Vector2f offset{ -DIGIT_TEXTURE_WIDTH,129.0f };
	sf::Vector2f width{DIGIT_WIDTH, 0.0f };
	sf::Vector2f height{0.0f, DIGIT_TEXTURE_HEIGHT };
	sf::Vector2f textureWidth = { DIGIT_WIDTH,0.0f };
	sf::Vector2f textureHeight = { 0.0f, DIGIT_TEXTURE_HEIGHT };
	
	offset.x += t_value * DIGIT_TEXTURE_WIDTH;
	m_tilesArray[t_arrayOffset].position = t_loction;
	m_tilesArray[t_arrayOffset++].texCoords = offset;

	m_tilesArray[t_arrayOffset].position = t_loction + width;
	m_tilesArray[t_arrayOffset++].texCoords = offset + textureWidth;

	m_tilesArray[t_arrayOffset].position = t_loction + width + height;
	m_tilesArray[t_arrayOffset++].texCoords = offset + textureWidth + textureHeight;

	m_tilesArray[t_arrayOffset].position = t_loction;
	m_tilesArray[t_arrayOffset++].texCoords = offset;

	m_tilesArray[t_arrayOffset].position = t_loction + height;
	m_tilesArray[t_arrayOffset++].texCoords = offset + textureHeight;

	m_tilesArray[t_arrayOffset].position = t_loction + width + height;;
	m_tilesArray[t_arrayOffset].texCoords = offset + textureWidth + textureHeight;

	
}

int Game::countFlags()
{
	int count = 0;
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (m_playerMap[i][j] == FLAG)
			{
				count++;
			}
		}
	}
	return count;
}

void Game::setupCorner()
{
	sf::Vector2u size = m_window.getSize();
	unsigned widthu = (size.x - BORDER * 8) / mapWidth;
	SPRITE_TILE_WIDTH = static_cast<float>(widthu);
	unsigned heightu = (size.y - BORDER * 8 - 128.0f) / mapHeight;
	SPRITE_TILE_HEIGHT = static_cast<float>(heightu);
	float width = static_cast<float>( mapWidth) * SPRITE_TILE_WIDTH;
	float height = static_cast<float>(mapHeight) * SPRITE_TILE_HEIGHT;
	
	m_boardTopLeft.x = (size.x - width) / 2.0f;
	m_boardTopLeft.y = (size.y - height) / 2.0f + 64.0f;
	m_boardTopLefti = static_cast<sf::Vector2i>(m_boardTopLeft);

}

void Game::setupFrame()
{
	sf::Vertex vertex;
	sf::Vector2u size = m_window.getSize();
	float width = static_cast<float>(size.x);
	float height = static_cast<float>(size.y);
	vertex.color = HIGH_LIGHT_COLOUR;
	
	vertex.texCoords = { 667.0f,0.0f };
	vertex.position = sf::Vector2f{ 0.0f,0.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ width,0.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ width - BORDER,BORDER };
	m_tilesArray.append(vertex);

	vertex.position = sf::Vector2f{ 0.0f,BORDER };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ 0.0f, 0.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ width - BORDER,BORDER };
	m_tilesArray.append(vertex);

	vertex.position = sf::Vector2f{ 0.0f,0.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ 0.0f, height };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ 0.0f + BORDER, height -BORDER};
	m_tilesArray.append(vertex);

	vertex.position = sf::Vector2f{ 0.0f,0.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ BORDER, BORDER };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ 0.0f + BORDER, height - BORDER };
	m_tilesArray.append(vertex);


	vertex.color = SHADOW_COLOUR;
	
	vertex.position = sf::Vector2f{ BORDER*3.0f,BORDER * 3.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ width - BORDER * 3.0f,BORDER * 3.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ width - BORDER*4.0f, BORDER* 4.0f };
	m_tilesArray.append(vertex);

	
	vertex.position = sf::Vector2f{ BORDER * 3.0f,BORDER * 4.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ BORDER * 3.0f,BORDER * 3.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ width - BORDER * 4.0f, BORDER * 4.0f };
	m_tilesArray.append(vertex);
	
	vertex.position = sf::Vector2f{ BORDER * 3.0f,BORDER * 3.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ BORDER * 3.0f, height - BORDER *3.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ BORDER * 4.0f , height - BORDER * 4.0f};
	m_tilesArray.append(vertex);

	vertex.position = sf::Vector2f{ BORDER * 3.0f,BORDER * 3.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{ BORDER *4.0f, BORDER*4.0f };
	m_tilesArray.append(vertex);
	vertex.position = sf::Vector2f{  BORDER*4.0f, height - BORDER *3.0f };
	m_tilesArray.append(vertex);
	
	addQuad(sf::Vector2f{ width ,height - BORDER },
		sf::Vector2f{ BORDER,height-BORDER },
		sf::Vector2f{ 0.0f, height },
		sf::Vector2f{ width, height },
		SHADOW_COLOUR);
	addQuad(sf::Vector2f{ width, height },
		sf::Vector2f{ width - BORDER,height  },
		sf::Vector2f{ width - BORDER ,BORDER },
		sf::Vector2f{ width , 0.0f },
		
		SHADOW_COLOUR);

	addQuad(sf::Vector2f{ width - BORDER*3.0f, height - BORDER * 3.0f },
		sf::Vector2f{ width - BORDER *4.0f ,height - BORDER * 3.0f },
		sf::Vector2f{ width - BORDER * 4.0f,BORDER*4.0f },
		sf::Vector2f{ width - BORDER * 3.0f ,  BORDER * 3.0f },

		HIGH_LIGHT_COLOUR);

	addQuad(sf::Vector2f{ width - BORDER * 3.0f,height - BORDER * 4.0f },
		sf::Vector2f{ BORDER *4.0f ,height - BORDER * 4.0f },
		sf::Vector2f{ BORDER * 3.0f, height - BORDER * 3.0f },
		sf::Vector2f{ width - BORDER * 3.0f , height - BORDER * 3.0f },
		HIGH_LIGHT_COLOUR);
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
	int chance = t_width * t_height / 2;
	int nextTile = 0;
	int bombsPlaced = 0;
	m_currentSquare = sf::Vector2i{ 0,0 };
	m_gameOn = false;
	timeElapsed = sf::Time::Zero;
	secondsElapsed = 0;
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
		std::cout << "another pass" << std::endl;
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
	//m_tilesArray.clear();
	vertex.color = sf::Color::White;
#ifdef TEST
	std::cout << "reset to " << t_height << " * " << t_width << std::endl;
#endif // TEST

	for (int i = 0; i < t_height; i++)
	{
		for (int j = 0; j < t_width; j++)
		{
			vertex.position = m_boardTopLeft + sf::Vector2f{ j * SPRITE_TILE_WIDTH, i * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left , top };
			m_tilesArray.append(vertex); //a

			vertex.position = m_boardTopLeft + sf::Vector2f{ (j +1) * SPRITE_TILE_WIDTH, i * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left +TEXTURE_TILE_WIDTH , top  };
			m_tilesArray.append(vertex); //b

			vertex.position = m_boardTopLeft + sf::Vector2f{ j * SPRITE_TILE_WIDTH, (i +1) * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left  , top + TEXTURE_TILE_HEIGHT };
			m_tilesArray.append(vertex);//d

			vertex.position = m_boardTopLeft + sf::Vector2f{ (j + 1) * SPRITE_TILE_WIDTH, i * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left + TEXTURE_TILE_WIDTH , top  };
			m_tilesArray.append(vertex); //b

			vertex.position = m_boardTopLeft + sf::Vector2f{ (j + 1) * SPRITE_TILE_WIDTH, (i+1) * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left + TEXTURE_TILE_WIDTH , top + TEXTURE_TILE_HEIGHT };
			m_tilesArray.append(vertex);//c

			vertex.position = m_boardTopLeft + sf::Vector2f{ j * SPRITE_TILE_WIDTH, (i + 1) * SPRITE_TILE_HEIGHT };
			vertex.texCoords = sf::Vector2f{ left  , top + TEXTURE_TILE_WIDTH };
			m_tilesArray.append(vertex);//d
		}
	}

#ifdef TEST
	std::cout << " size calculated " << t_width * t_height * 6 + 36<< std::endl;
	std::cout << " size actual " << m_tilesArray.getVertexCount() << std::endl;
#endif // TEST

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
			index = (j * 6) + (i * mapWidth * 6) + INDEX_OFFSET;
			m_tilesArray[index++].texCoords = topLeft; // a
			m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b
			m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d;
			m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b;
			m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_HEIGHT, TEXTURE_TILE_WIDTH }; //c;;
			m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d;;
		}
	}
	topLeft = EXPLODED_MINE_TILE_TOPLEFT;

	index = (t_square.y * 6) + (t_square.x * mapWidth * 6) + INDEX_OFFSET;
	m_tilesArray[index++].texCoords = topLeft; // a
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_WIDTH,0.0f }; //b;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ TEXTURE_TILE_HEIGHT, TEXTURE_TILE_WIDTH }; //c;;
	m_tilesArray[index++].texCoords = topLeft + sf::Vector2f{ 0.0f, TEXTURE_TILE_HEIGHT }; //d
	showEmoji(Emoji::Bad);
}

int Game::countFlags(sf::Vector2i t_square)
{
	int count=0;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (FLAG == m_playerMap[t_square.x + i][t_square.y + j])
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
	showEmoji(Emoji::Guess);
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

void Game::addQuad(sf::Vector2f t_a, sf::Vector2f t_b, sf::Vector2f t_c, sf::Vector2f t_d, sf::Color t_colour)
{
	sf::Vertex vertex;
	vertex.color = t_colour;
	vertex.texCoords = { 667.0f,0.0f };
	
	vertex.position = t_a;
	m_tilesArray.append(vertex);
	vertex.position = t_b;
	m_tilesArray.append(vertex);
	vertex.position = t_c;
	m_tilesArray.append(vertex);

	vertex.position = t_d;
	m_tilesArray.append(vertex);
	vertex.position = t_c;
	m_tilesArray.append(vertex);
	vertex.position = t_a;
	m_tilesArray.append(vertex);
	
}
