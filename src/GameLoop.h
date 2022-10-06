#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

namespace Raycaster {

constexpr auto width = 1024;
constexpr auto height = 576;

class GameLoop
{
public:
	GameLoop() = default;
	void Run();

private:
	sf::RenderWindow m_window{sf::VideoMode(1024, 576), "SFML window"};
	float m_posX = 22.0f;
	float m_posY = 12.0f; // x and y start position
	float m_dirX = -1.0f;
	float m_dirY = 0.0f; // initial direction vector
	float m_planeX = 0.0f;
	float m_planeY = 0.66f; // the 2d raycaster version of camera plane
};

}
