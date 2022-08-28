#include "flappy.h"

using namespace flappy_bird_game;

static auto rand_gen  = std::mt19937(std::random_device{}());


template <typename IntType>
auto uniform_random_int(IntType a, IntType b) -> IntType
{
	std::uniform_int_distribution<IntType> dist(a, b);
	return dist(rand_gen);
}

template <typename RealType>
auto uniform_random_real(RealType a, RealType b) -> RealType
{
	std::uniform_real_distribution<RealType> dist(a, b);
	return dist(rand_gen);
}

flappy_bird::flappy_bird()
{
	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1000, 600),
	                                            "Flappy Bird",
	                                            sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(60);
	window->setPosition(sf::Vector2i(0, 0));

	bg.loadFromFile("./resources/background.png");
	background = std::make_unique<sf::Sprite>();
	background->setTexture(bg);

	bird.loadFromFile("./resources/flappy.png");
	flappy = std::make_unique<sf::Sprite>();
	flappy->setTexture(bird);
	flappy->setPosition(500.f - bird.getSize().x / 2.f, 300.f - bird.getSize().y / 2.f);
	flappy->setScale(2.f, 2.f);
	flappy->setTextureRect(sf::IntRect(0, 0, 34, 24));

	pipe.loadFromFile("./resources/pipe.png");
	pipe_bottom = std::make_unique<sf::Sprite>();
	pipe_bottom->setTexture(pipe);
	pipe_bottom->setScale(1.5f, 1.5f);
	
	pipe_top = std::make_unique<sf::Sprite>();
	pipe_top->setTexture(pipe);
	pipe_top->setScale(1.5f, -1.5f);

	font.loadFromFile("./resources/flappybird.ttf");
	ko_txt.setFont(font);
	ko_txt.setString("Press SPACE to restart");
	ko_txt.setPosition(200, 300);
	ko_txt.setCharacterSize(50);
	ko_txt.setOutlineThickness(3);

	score_txt.setFont(font);
	score_txt.setString(std::to_string(score));
	score_txt.setPosition(10, 10);
	score_txt.setCharacterSize(50);
	score_txt.setOutlineThickness(3);
}

flappy_bird::~flappy_bird() = default;

void flappy_bird::run()
{
	while (window->isOpen())
	{
		events();
		game();
		draw();
	}
}

void flappy_bird::events()
{
	auto evt = std::make_unique<sf::Event>();
	while (window->pollEvent(*evt))
	{
		if (evt->type == sf::Event::Closed or 
			(evt->KeyPressed and evt->key.code == sf::Keyboard::Escape))
		{
			window->close();
		}

		if (game_over and sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			score = 0;
			pipes.clear();

			flappy->setPosition(500.f - bird.getSize().x / 2.f, 300.f - bird.getSize().y / 2.f);

			game_over = false;
		}
	}
}

void flappy_bird::draw()
{
	window->clear(sf::Color::Black);

	window->draw(*background);
	for (auto &&p : pipes)
	{
		window->draw(p);
	}
	window->draw(*flappy);

	if (game_over)
	{
		window->draw(ko_txt);
	}

	window->draw(score_txt);

	window->display();

	count++;
}

void flappy_bird::game()
{
	if (not game_over)
	{
		animate();
		move_flappy();
		move_pipes();
	}
}

void flappy_bird::move_pipes()
{
	if (count % 150 == 0)
	{
		auto pos = uniform_random_int(0, 275) + 175.f;

		pipe_bottom->setPosition(1000, pos + space);
		pipe_top->setPosition(1000, pos);

		pipes.push_back(*pipe_bottom);
		pipes.push_back(*pipe_top);
	}

	for (size_t i = 0; i < pipes.size(); i++)
	{
		if (pipes[i].getGlobalBounds().intersects(flappy->getGlobalBounds()))
		{
			game_over = true;
		}

		if(pipes[i].getPosition().x < -100)
		{
			pipes.erase(pipes.begin() + i);
		}

		pipes[i].move(-3, 0);

		static auto add{true};
		if(pipes[i].getPosition().x == 298 and add)
		{
			score++;
			score_txt.setString(std::to_string(score));
			add = false;
		}
		else
		{
			add = true;
		}
	}
}

void flappy_bird::animate()
{
	frame += 0.15f;

	if (frame > 3.f)
	{
		frame -= 3.f;
	}

	flappy->setTextureRect(sf::IntRect(34 * static_cast<int>(frame), 0, 34, 24));
}

void flappy_bird::move_flappy()
{
	flappy->move(0, gravity);
	gravity += 0.5f;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		gravity = -8.f;
		flappy->setRotation(-frame - 10.f);
	} 
	else
	{
		flappy->setRotation(frame - 10.f);
	}
}