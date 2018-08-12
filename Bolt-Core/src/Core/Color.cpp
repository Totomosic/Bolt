#include "Color.h"
#include "Functions.h"

namespace Bolt
{

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
		return (r == other.r && g == other.g && b == other.b && a == other.a);
	}

	bool Color::operator!=(const Color& other) const
	{
		return !(*this == other);
	}

	size_t Color::ToInt() const
	{
		byte values[4] = { (byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(a * 255) };
		size_t value = *(size_t*)values;
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
		stream << "<<" << color.r << color.g << color.b << color.a << ">>";
		return stream;
	}

	Color Color::FromFloats(float r, float g, float b, float a)
	{
		Color c((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(a * 255));
		return c;
	}

	Color Color::FromHSV(float hue, float saturation, float value)
	{
		double hh;
		double p;
		double q;
		double t;
		double ff;
		int i;
		float r;
		float g;
		float b;

		if (saturation <= 0.0) 
		{
			r = value;
			g = value;
			b = value;
			return Color(r * 255, g * 255, b * 255, 255);
		}
		hh = hue;
		if (hh >= 360.0) hh = 0.0;
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;
		p = value * (1.0 - saturation);
		q = value * (1.0 - (saturation * ff));
		t = value * (1.0 - (saturation * (1.0 - ff)));

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
		return Color(r * 255, g * 255, b * 255, 255);
	}

	Color Color::Random()
	{
		Color c = Color::FromFloats(Random::NextFloat(0.0f, 1.0f), Random::NextFloat(0.0f, 1.0f), Random::NextFloat(0.0f, 1.0f), 1.0f);
		return c;
	}

}