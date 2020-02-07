#include "Color.h"
#include "Functions.h"

namespace Bolt
{

	const Bolt::Color Color::White = Bolt::Color(255, 255, 255, 255);
	const Bolt::Color Color::Black = Bolt::Color(0, 0, 0, 255);

	const Bolt::Color Color::Red = Bolt::Color(255, 0, 0, 255);
	const Bolt::Color Color::Green = Bolt::Color(0, 255, 0, 255);
	const Bolt::Color Color::Blue = Bolt::Color(0, 0, 255, 255);

	const Bolt::Color Color::Yellow = Bolt::Color(255, 255, 0, 255);
	const Bolt::Color Color::Cyan = Bolt::Color(0, 255, 255, 255);
	const Bolt::Color Color::Magenta = Bolt::Color(255, 0, 255, 255);

	const Bolt::Color Color::CornflowerBlue = Bolt::Color(100, 149, 237, 255);
	const Bolt::Color Color::Purple = Bolt::Color(128, 0, 128, 255);
	const Bolt::Color Color::Indigo = Bolt::Color(75, 0, 130, 255);
	const Bolt::Color Color::Lavender = Bolt::Color(230, 230, 250, 255);
	const Bolt::Color Color::Violet = Bolt::Color(238, 130, 238, 255);
	const Bolt::Color Color::Brown = Bolt::Color(165, 42, 42, 255);
	const Bolt::Color Color::SaddleBrown = Bolt::Color(139, 69, 19, 255);
	const Bolt::Color Color::Tan = Bolt::Color(210, 180, 140, 255);
	const Bolt::Color Color::Gold = Bolt::Color(255, 215, 0, 255);
	const Bolt::Color Color::Orange = Bolt::Color(255, 165, 0, 255);
	const Bolt::Color Color::Navy = Bolt::Color(0, 0, 128, 255);
	const Bolt::Color Color::RoyalBlue = Bolt::Color(65, 105, 225, 255);
	const Bolt::Color Color::SkyBlue = Bolt::Color(135, 206, 235, 255);
	const Bolt::Color Color::DeepSkyBlue = Bolt::Color(0, 191, 255, 255);
	const Bolt::Color Color::Olive = Bolt::Color(128, 128, 0, 255);
	const Bolt::Color Color::DarkGreen = Bolt::Color(0, 100, 0, 255);
	const Bolt::Color Color::LawnGreen = Bolt::Color(124, 252, 0, 255);
	const Bolt::Color Color::Pink = Bolt::Color(255, 192, 203, 255);
	const Bolt::Color Color::HotPink = Bolt::Color(255, 105, 180, 255);
	const Bolt::Color Color::DeepPink = Bolt::Color(255, 20, 147, 255);
	const Bolt::Color Color::Teal = Bolt::Color(0, 128, 128, 255);
	const Bolt::Color Color::Turquoise = Bolt::Color(64, 224, 208, 255);

	float Color::GetBrightness() const
	{
		return (r + b + g) / 3.0f;
	}

	Vector4f Color::rgba() const
	{
		return Vector4f(r, g, b, a);
	}

	Vector4f Color::abgr() const
	{
		return Vector4f(a, b, g, r);
	}

	Vector4f Color::bgra() const
	{
		return Vector4f(b, g, r, a);
	}

	Vector3f Color::rgb() const
	{
		return Vector3f(r, g, b);
	}

	Vector3f Color::bgr() const
	{
		return Vector3f(b, g, r);
	}

	Vector2f Color::rg() const
	{
		return Vector2f(r, g);
	}

	Vector2f Color::gb() const
	{
		return Vector2f(g, b);
	}

	Vector2f Color::rb() const
	{
		return Vector2f(r, b);
	}

	Vector4fr Color::rgba()
	{
		return Vector4fr(r, g, b, a);
	}

	Vector4fr Color::abgr()
	{
		return Vector4fr(a, b, g, r);
	}

	Vector4fr Color::bgra()
	{
		return Vector4fr(b, g, r, a);
	}

	Vector3fr Color::rgb()
	{
		return Vector3fr(r, g, b);
	}

	Vector3fr Color::bgr()
	{
		return Vector3fr(b, g, r);
	}

	Vector2fr Color::rg()
	{
		return Vector2fr(r, g);
	}

	Vector2fr Color::gb()
	{
		return Vector2fr(g, b);
	}

	Vector2fr Color::rb()
	{
		return Vector2fr(r, b);
	}

	bool Color::operator==(const Color& other) const
	{
		return ToInt() == other.ToInt();
	}

	bool Color::operator!=(const Color& other) const
	{
		return !(*this == other);
	}

	uint32_t Color::ToInt() const
	{
		byte values[4] = { (byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(a * 255) };
		uint32_t value = *(uint32_t*)values;
		return value;
	}

	Vector4<byte> Color::ToBytes() const
	{
		return Vector4<byte>((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(a * 255));
	}

	Color operator+(const Color& left, const Color& right)
	{
		return Color::FromFloats(left.r + right.r, left.g + right.g, left.b + right.b, left.a + right.a);
	}

	Color operator+(const Color& left, float right)
	{
		return Color::FromFloats(left.r + right, left.g + right, left.b + right, left.a + right);
	}

	Color operator+(float left, const Color& right)
	{
		return right + left;
	}

	Color operator*(const Color& left, const Color& right)
	{
		return Color::FromFloats(left.r * right.r, left.g * right.g, left.b * right.b, left.a * right.a);
	}

	Color operator*(const Color& left, float right)
	{
		return Color::FromFloats(left.r * right, left.g * right, left.b * right, left.a * right);
	}

	Color operator*(float left, const Color& right)
	{
		return right * left;
	}

	std::ostream& operator<<(std::ostream& stream, const Color& color)
	{
		stream << "Color(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ')';
		return stream;
	}

	Color Color::FromFloats(float r, float g, float b, float a)
	{
		Color c((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(a * 255));
		return c;
	}

	Color Color::FromHSV(float hue, float saturation, float value)
	{
		float hh;
		float p;
		float q;
		float t;
		float ff;
		int i;
		float r;
		float g;
		float b;

		if (saturation <= 0.0f) 
		{
			r = value;
			g = value;
			b = value;
			return Color((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), 255);
		}
		hh = hue;
		if (hh >= 360.0f) hh = 0.0f;
		hh /= 60.0f;
		i = (long)hh;
		ff = hh - i;
		p = value * (1.0f - saturation);
		q = value * (1.0f - (saturation * ff));
		t = value * (1.0f - (saturation * (1.0f - ff)));

		switch (i) 
		{
		case 0:
			r = value;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = value;
			b = p;
			break;
		case 2:
			r = p;
			g = value;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = value;
			break;
		case 4:
			r = t;
			g = p;
			b = value;
			break;
		case 5:
		default:
			r = value;
			g = p;
			b = q;
			break;
		}
		return Color((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), 255);
	}

	Color Color::Random()
	{
		Color c = Color::FromFloats(Random::NextFloat(0.0f, 1.0f), Random::NextFloat(0.0f, 1.0f), Random::NextFloat(0.0f, 1.0f), 1.0f);
		return c;
	}

}
