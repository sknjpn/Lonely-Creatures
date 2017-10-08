#include"Camera.h"

Camera::Camera()
	: restrictedRegion(Vec2(-2048, -2048), 3 * Vec2(2048, 2048))
	, drawingRegion(0, 0, 1024, 1024)
	, smoothDrawingRegion(drawingRegion)
	, gazePoint(none)
{}

void Camera::update()
{
	{
		auto t = createTransformer2D();

		{
			double delta = Mouse::Wheel();
			//if (drawingRegion.size.y >= 180_deg && delta > 0) delta = 0;
			//if (drawingRegion.size.y <= 1.8_deg && delta < 0) delta = 0;
			drawingRegion = drawingRegion.scaledAt(Cursor::PosF(), 1.0 + 0.1*delta);
		}

		if (gazePoint)
		{
			drawingRegion.setCenter(gazePoint.value());
			gazePoint = none;
		}
		/*
		if (drawingRegion.size.y > 180_deg) drawingRegion = drawingRegion.scaledAt(drawingRegion.center(), (180_deg / drawingRegion.size.y));
		if (drawingRegion.pos.y < -90_deg) drawingRegion.pos.y = -90_deg;
		if (drawingRegion.pos.y + drawingRegion.size.y > 90_deg) drawingRegion.pos.y = 90_deg - drawingRegion.size.y;
		if (smoothDrawingRegion.pos.x < -180_deg) { drawingRegion.pos.x += 360_deg; smoothDrawingRegion.pos.x += 360_deg; }
		if (smoothDrawingRegion.pos.x > 180_deg) { drawingRegion.pos.x -= 360_deg; smoothDrawingRegion.pos.x -= 360_deg; }
		*/
		const double followingSpeed = 0.2;
		smoothDrawingRegion.pos = smoothDrawingRegion.pos*(1.0 - followingSpeed) + drawingRegion.pos*followingSpeed;
		smoothDrawingRegion.size = smoothDrawingRegion.size*(1.0 - followingSpeed) + drawingRegion.size*followingSpeed;

	}

	const double slidingSpeed = (drawingRegion.size.y / 180_deg)*0.05;
	const bool useKeyViewControl = true;
	
	if ((useKeyViewControl && KeyA.pressed()) || Cursor::Pos().x <= 0) drawingRegion.pos.x -= slidingSpeed;
	if ((useKeyViewControl && KeyW.pressed()) || Cursor::Pos().y <= 0) drawingRegion.pos.y -= slidingSpeed;
	if ((useKeyViewControl && KeyD.pressed()) || Cursor::Pos().x >= Window::Size().x - 1) drawingRegion.pos.x += slidingSpeed;
	if ((useKeyViewControl && KeyS.pressed()) || Cursor::Pos().y >= Window::Size().y - 1) drawingRegion.pos.y += slidingSpeed;
}
Transformer2D	Camera::createTransformer2D(double _magnification) const
{
	auto mat3x2 = Mat3x2::Scale(_magnification).translated(-smoothDrawingRegion.center()).scaled(Window::Size().y / smoothDrawingRegion.size.y).translated(Window::ClientRect().center());

	return Transformer2D(mat3x2, true);
}