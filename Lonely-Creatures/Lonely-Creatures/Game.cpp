#include"Game.h"

Game::Game() : field(&assets), display(&assets) {

	Window::SetTitle(L"Lonely Creatures");
	Window::Resize(1280, 720);

	assets.audio(L"‚©‚Ý‚³‚Ü‚Ì‚ä‚è‚©‚².mp3").setLoop(true);
	assets.audio(L"‚©‚Ý‚³‚Ü‚Ì‚ä‚è‚©‚².mp3").play();
}

void	Game::update() {

	camera.update();

	field.update();

	if (MouseL.down())
	{
		auto t = camera.createTransformer2D();

		display.selectedCreature = nullptr;

		for (auto& c : field.creatures)
		{
			if (Circle(c.pos, c.size() / 2.0).intersects(Cursor::PosF()))
			{
				display.selectedCreature = &c;
				break;
			}
		}
	}

	{
		auto t = camera.createTransformer2D();

		field.draw();
	}

	if (display.selectedCreature != nullptr)
	{
		auto t = camera.createTransformer2D();
		auto* sc = display.selectedCreature;
		Circle(sc->drawPos(), sc->size() / 2.0).draw(Color(Palette::Skyblue, 96)).drawFrame(1, Palette::Black);
		camera.gazePoint = sc->pos;
	}
	else
	{
		auto t = camera.createTransformer2D();
		for (auto& c : field.creatures)
		{
			if (Circle(c.drawPos(), c.size() / 2.0).intersects(Cursor::PosF()))
			{
				Circle(c.drawPos(), c.size() / 2.0).drawFrame(1, Palette::Black);
				break;
			}
		}
	}
}