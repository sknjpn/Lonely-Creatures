#include"TinyCamera.h"

TinyCamera::TinyCamera()
	: restrictedRegion(-1024, -1024, 3072, 3072)
	, drawingRegion(restrictedRegion)
	, smoothDrawingRegion(drawingRegion)
	, gazePoint(none)
	, outputRegion(1280, 720)
{}

void TinyCamera::update()
{
	{
		//視点移動処理
		const auto t1 = createTransformer();

		//ズーム
		{
			double delta = Mouse::Wheel();
			drawingRegion = drawingRegion.scaledAt(Cursor::PosF(), 1.0 + 0.1*delta);
		}

		//注視点
		if (gazePoint)
		{
			auto p = gazePoint.value();
			p = gazePoint.value();
			drawingRegion.setCenter(p);

			//クリア
			gazePoint = none;
		}

		//drawingRegionを範囲内に収める
		/*
		if (drawingRegion.size.y > restrictedRegion.size.y) drawingRegion = drawingRegion.scaledAt(drawingRegion.center(), (drawingRegion.size.y / drawingRegion.size.y));
		if (drawingRegion.size.x > restrictedRegion.size.x) drawingRegion = drawingRegion.scaledAt(drawingRegion.center(), (drawingRegion.size.x / drawingRegion.size.x));
		if (drawingRegion.pos.y < restrictedRegion.tl().y) drawingRegion.pos.y = restrictedRegion.tl().y;
		if (drawingRegion.br().y > 90restrictedRegion.tl().y) drawingRegion.pos.y = 90_deg - drawingRegion.size.y;
		if (smoothDrawingRegion.pos.x < -180_deg) { drawingRegion.pos.x += 360_deg; smoothDrawingRegion.pos.x += 360_deg; }
		if (smoothDrawingRegion.pos.x > 180_deg) { drawingRegion.pos.x -= 360_deg; smoothDrawingRegion.pos.x -= 360_deg; }
		*/

		//smoothDrawingRegionの追従
		const double followingSpeed = 0.2;
		smoothDrawingRegion.pos = smoothDrawingRegion.pos*(1.0 - followingSpeed) + drawingRegion.pos*followingSpeed;
		smoothDrawingRegion.size = smoothDrawingRegion.size*(1.0 - followingSpeed) + drawingRegion.size*followingSpeed;
	}
	//スライダー
	const double slidingSpeed = (drawingRegion.size.y / 180_deg)*0.05;
	const bool useKeyViewControl = true;
	if ((useKeyViewControl && KeyA.pressed()) || Cursor::Pos().x <= 0) drawingRegion.pos.x -= slidingSpeed;
	if ((useKeyViewControl && KeyW.pressed()) || Cursor::Pos().y <= 0) drawingRegion.pos.y -= slidingSpeed;
	if ((useKeyViewControl && KeyD.pressed()) || Cursor::Pos().x >= Window::Size().x - 1) drawingRegion.pos.x += slidingSpeed;
	if ((useKeyViewControl && KeyS.pressed()) || Cursor::Pos().y >= Window::Size().y - 1) drawingRegion.pos.y += slidingSpeed;
}
Vec2 TinyCamera::getCursorPos() const
{
	return getMat3x2().inversed().transform(Cursor::PosF());
}
Mat3x2 TinyCamera::getMat3x2(int _delta) const
{
	return Mat3x2::Translate(-smoothDrawingRegion.center().movedBy(-_delta * 360_deg, 0.0)).scaled(outputRegion.size.y / smoothDrawingRegion.size.y).translated(outputRegion.center());
}
Transformer2D TinyCamera::createTransformer(int _delta) const
{
	return Transformer2D(getMat3x2(_delta), true);
}
