#include"Game.h"

Game::Game() : field(&assets), display(&assets) {

	Window::SetTitle(L"Lonely Creatures");
	Window::Resize(1280, 720);
	
	assets.audio(L"‚©‚Ý‚³‚Ü‚Ì‚ä‚è‚©‚².mp3").setLoop(true);
	//assets.audio(L"‚©‚Ý‚³‚Ü‚Ì‚ä‚è‚©‚².mp3").play();
}

void	Game::update() {

	field.update();
	camera.update();

	{
		auto t = camera.createTransformer2D();

		field.draw();
	}
}