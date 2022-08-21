#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <cstdlib>

namespace {
constexpr auto MAP_WIDTH = 24;
constexpr auto MAP_HEIGHT = 24;

int g_worldMap[MAP_WIDTH][MAP_HEIGHT] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
										 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
										 {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

} // namespace

int main()
{

	static constexpr auto w = 1024;
	static constexpr auto h = 576;

	auto posX = 22.0f;
	auto posY = 12.0f; // x and y start position
	auto dirX = -1.0f;
	auto dirY = 0.0f; // initial direction vector
	auto planeX = 0.0f;
	auto planeY = 0.66f; // the 2d raycaster version of camera plane

	// double time = 0; // time of current frame
	// double oldTime = 0; // time of previous frame

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(w, h), "SFML window");

	window.setFramerateLimit(60);
	auto clock = sf::Clock();
	sf::Time fps;

	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		sf::Event event{};
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			// Escape key: exit
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		window.clear();

		for (auto x = 0; x < w; x++)
		{
			// calculate ray position and direction
			const auto cameraX = 2.0f * static_cast<float>(x) / w - 1; // x-coordinate in camera space
			const auto rayPosX = posX;
			const auto rayPosY = posY;
			const auto rayDirX = dirX + planeX * cameraX;
			const auto rayDirY = dirY + planeY * cameraX;

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
				if (g_worldMap[mapX][mapY] > 0)
					hit = 1;
			}

			// Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
			const auto perpWallDist = side == 0
				? fabs((static_cast<double>(mapX) - static_cast<double>(rayPosX) + (1.0 - stepX) / 2) /
					   static_cast<double>(rayDirX))
				: fabs((static_cast<double>(mapY) - static_cast<double>(rayPosY) + (1.0 - stepY) / 2) /
					   static_cast<double>(rayDirY));

			// Calculate height of line to draw on screen
			int lineHeight = abs(static_cast<int>(h / perpWallDist));

			// calculate lowest and highest pixel to fill in current stripe
			int drawStart = -lineHeight / 2 + h / 2;
			if (drawStart < 0)
				drawStart = 0;
			int drawEnd = lineHeight / 2 + h / 2;
			if (drawEnd >= h)
				drawEnd = h - 1;

			// choose wall color
			sf::Color color;
			switch (g_worldMap[mapX][mapY])
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
			window.draw(line, 2, sf::Lines);
		}

		fps = clock.getElapsedTime();
		// float fpsValue = 1000000 / fps.asMicroseconds();
		clock.restart();

		const auto moveSpeed = fps.asSeconds() * 5.0f; // the constant value is in squares/second
		const auto rotSpeed = fps.asSeconds() * 3.0f; // the constant value is in radians/second

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			// both camera direction and camera plane must be rotated
			const auto oldDirX = dirX;
			dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
			dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
			const auto oldPlaneX = planeX;
			planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
			planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			// both camera direction and camera plane must be rotated
			const auto oldDirX = dirX;
			dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
			dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
			const auto oldPlaneX = planeX;
			planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
			planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			if (g_worldMap[static_cast<int>(posX + dirX * moveSpeed)][static_cast<int>(posY)] == false)
				posX += dirX * moveSpeed;
			if (g_worldMap[static_cast<int>(posX)][static_cast<int>(posY + dirY * moveSpeed)] == false)
				posY += dirY * moveSpeed;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			if (g_worldMap[static_cast<int>(posX - dirX * moveSpeed)][static_cast<int>(posY)] == false)
				posX -= dirX * moveSpeed;
			if (g_worldMap[static_cast<int>(posX)][static_cast<int>(posY - dirY * moveSpeed)] == false)
				posY -= dirY * moveSpeed;
		}

		window.display();
		window.clear();
	}

	return EXIT_SUCCESS;
}