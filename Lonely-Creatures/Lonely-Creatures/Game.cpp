#include"Game.h"

Game::Game()
	: field(&assets)
	, display(&assets)
	, playSpeed(1)
{

	Window::SetTitle(L"Lonely Creatures");
	Window::Resize(1280, 720);

	assets.audio(L"かみさまのゆりかご.mp3").setLoop(true);
	//assets.audio(L"かみさまのゆりかご.mp3").play();
}

void	Game::update() {

	ClearPrint();

	//PlaySpeedの調整
	if (KeyF1.down()) {
		if (playSpeed == 1) playSpeed = 0;
		else if (playSpeed > 1) playSpeed /= 2;
	}
	if (KeyF2.down()) {
		if (playSpeed == 0) playSpeed = 1;
		else if (playSpeed > 0 && playSpeed <= 64) playSpeed *= 2;
	}
	Print << L"フルスクリーンにするにはF11キー、ヘルスバーを表示するにはHキーをおしてください。";
	Print << L"実行速度を変更するには、F1キー、もしくはF2キーを使ってください。" << L"実行速度 = " << playSpeed;
	Print << L"生物数 = " << Creature::numEnabled;
	Print << L"マテリアル数 = " << Material::numEnabled;

	//FullScreen
	if (KeyF11.down())
	{
		if (Window::GetState().fullScreen) Graphics::SetFullScreen(false, Size(1280, 720));
		else Graphics::SetFullScreen(true, Graphics::EnumOutputs().front().displayModes.back().size);
		//System::Update();
	}

	camera.update();

	for (int i = 0; i < playSpeed; i++) field.update();

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