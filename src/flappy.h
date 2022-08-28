#pragma once

#include <sfml/Graphics.hpp>

namespace flappy_bird_game
{
	class flappy_bird
	{
	public:
		flappy_bird();
		~flappy_bird();

		void run();

	protected:
		void events();
		void draw();

		void game();
		void move_pipes();
		void animate();
		void move_flappy();

	private:
		std::unique_ptr<sf::RenderWindow> window;
		float gravity{0}, frame{0}, space{160.f};
		int count{0}, score{0};
		bool game_over{false};
		sf::Texture bg, bird, pipe;
		std::unique_ptr<sf::Sprite> background, flappy, pipe_bottom, pipe_top;
		std::vector<sf::Sprite> pipes;
		sf::Font font;
		sf::Text score_txt, ko_txt;
	};
} // namespace flappy_bird
