#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "GameLoop.h"
#include "Map.h"

using namespace Raycaster;

void GameLoop::Run()
{
	m_window.setFramerateLimit(60);



	while (m_window.isOpen())
	{
		// Process events
		sf::Event event{};
		while (m_window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				m_window.close();

			// Escape key: exit
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				m_window.close();
		}

		m_window.clear();

		for (auto x = 0; x < width; x++)
		{
			// calculate ray position and direction
			const auto cameraX = 2.0f * static_cast<float>(x) / width - 1; // x-coordinate in camera space
			const auto rayPosX = m_posX;
			const auto rayPosY = m_posY;
			const auto rayDirX = m_dirX + m_planeX * cameraX;
			const auto rayDirY = m_dirY + m_planeY * cameraX;

			// which box of the map we're in
			auto mapX = static_cast<int>(rayPosX);
			auto mapY = static_cast<int>(rayPosY);

			// length of ray from one x or y-side to next x or y-side
			const auto deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
			const auto deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

			// what direction to step in x or y-direction (either +1 or -1)
			const auto stepX = rayDirX < 0 ? -1 : 1;
			const auto stepY = rayDirY < 0 ? -1 : 1;

			int hit = 0; // was there a wall hit?
			int side = 0; // was a NS or a EW wall hit?

			// length of ray from current position to next x or y-side
			auto sideDistX = rayDirX < 0
				? (static_cast<double>(rayPosX) - static_cast<double>(mapX)) * static_cast<double>(deltaDistX)
				: (static_cast<double>(mapX) + 1.0 - static_cast<double>(rayPosX)) * static_cast<double>(deltaDistX);
			auto sideDistY = rayDirY < 0
				? (static_cast<double>(rayPosY) - static_cast<double>(mapY)) * static_cast<double>(deltaDistY)
				: (static_cast<double>(mapY) + 1.0 - static_cast<double>(rayPosY)) * static_cast<double>(deltaDistY);

			// perform DDA
			while (hit == 0)
			{
				// jump to next map square, OR in x-direction, OR in y-direction
				if (sideDistX < sideDistY)
				{
					sideDistX += static_cast<double>(deltaDistX);
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += static_cast<double>(deltaDistY);
					mapY += stepY;
					side = 1;
				}
				// Check if ray has hit a wall
				if (Map::g_worldMap[mapX][mapY] > 0)
					hit = 1;
			}

			// Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
			const auto perpWallDist = side == 0
				? fabs((static_cast<double>(mapX) - static_cast<double>(rayPosX) + (1.0 - stepX) / 2) /
					   static_cast<double>(rayDirX))
				: fabs((static_cast<double>(mapY) - static_cast<double>(rayPosY) + (1.0 - stepY) / 2) /
					   static_cast<double>(rayDirY));

			// Calculate height of line to draw on screen
			int lineHeight = abs(static_cast<int>(height / perpWallDist));

			// calculate lowest and highest pixel to fill in current stripe
			int drawStart = -lineHeight / 2 + height / 2;
			if (drawStart < 0)
				drawStart = 0;
			int drawEnd = lineHeight / 2 + height / 2;
			if (drawEnd >= height)
				drawEnd = height - 1;

			// choose wall color
			sf::Color color;
			switch (Map::g_worldMap[mapX][mapY])
			{
			case 1:
				color = sf::Color::Red;
				break; // red
			case 2:
				color = sf::Color::Green;
				break; // green
			case 3:
				color = sf::Color::Blue;
				break; // blue
			case 4:
				color = sf::Color::White;
				break; // white
			default:
				color = sf::Color::Yellow;
				break; // yellow
			}

			// give x and y sides different brightness
			if (side == 1)
			{
				color = sf::Color(color.r / 2, color.g / 2, color.b / 2);
			}

			// draw the pixels of the stripe as a vertical line
			// verLine(x, drawStart, drawEnd, color);

			sf::Vertex line[2] = {sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(drawStart)), color),
								  sf::Vertex(sf::Vector2f(static_cast<float>(x), static_cast<float>(drawEnd)), color)};
			m_window.draw(line, 2, sf::Lines);
		}

		m_fps = m_clock.getElapsedTime();
		// float fpsValue = 1000000 / fps.asMicroseconds();
		m_clock.restart();

		const auto moveSpeed = m_fps.asSeconds() * 5.0f; // the constant value is in squares/second
		const auto rotSpeed = m_fps.asSeconds() * 3.0f; // the constant value is in radians/second

		// HANDLING KEYS!!!!!!!!!!!!
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			// both camera direction and camera plane must be rotated
			const auto oldDirX = m_dirX;
			m_dirX = m_dirX * cos(rotSpeed) - m_dirY * sin(rotSpeed);
			m_dirY = oldDirX * sin(rotSpeed) + m_dirY * cos(rotSpeed);
			const auto oldPlaneX = m_planeX;
			m_planeX = m_planeX * cos(rotSpeed) - m_planeY * sin(rotSpeed);
			m_planeY = oldPlaneX * sin(rotSpeed) + m_planeY * cos(rotSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			// both camera direction and camera plane must be rotated
			const auto oldDirX = m_dirX;
			m_dirX = m_dirX * cos(-rotSpeed) - m_dirY * sin(-rotSpeed);
			m_dirY = oldDirX * sin(-rotSpeed) + m_dirY * cos(-rotSpeed);
			const auto oldPlaneX = m_planeX;
			m_planeX = m_planeX * cos(-rotSpeed) - m_planeY * sin(-rotSpeed);
			m_planeY = oldPlaneX * sin(-rotSpeed) + m_planeY * cos(-rotSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (Map::g_worldMap[static_cast<int>(m_posX + m_dirX * moveSpeed)][static_cast<int>(m_posY)] == false)
				m_posX += m_dirX * moveSpeed;
			if (Map::g_worldMap[static_cast<int>(m_posX)][static_cast<int>(m_posY + m_dirY * moveSpeed)] == false)
				m_posY += m_dirY * moveSpeed;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (Map::g_worldMap[static_cast<int>(m_posX - m_dirX * moveSpeed)][static_cast<int>(m_posY)] == false)
				m_posX -= m_dirX * moveSpeed;
			if (Map::g_worldMap[static_cast<int>(m_posX)][static_cast<int>(m_posY - m_dirY * moveSpeed)] == false)
				m_posY -= m_dirY * moveSpeed;
		}

		m_window.display();
		m_window.clear();
	}
}
