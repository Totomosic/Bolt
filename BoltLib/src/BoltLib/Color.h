#pragma once
#include "Math/Maths.h"

namespace Bolt
{

	struct BLT_API Color
	{
	public:
		static const Bolt::Color White;
		static const Bolt::Color Black;

		static const Bolt::Color Red;
		static const Bolt::Color Green;
		static const Bolt::Color Blue;

		static const Bolt::Color Yellow;
		static const Bolt::Color Cyan;
		static const Bolt::Color Magenta;

		static const Bolt::Color CornflowerBlue;
		static const Bolt::Color Purple;
		static const Bolt::Color Indigo;
		static const Bolt::Color Lavender;
		static const Bolt::Color Violet;
		static const Bolt::Color Brown;
		static const Bolt::Color SaddleBrown;
		static const Bolt::Color Tan;
		static const Bolt::Color Gold;
		static const Bolt::Color Orange;
		static const Bolt::Color Navy;
		static const Bolt::Color RoyalBlue;
		static const Bolt::Color SkyBlue;
		static const Bolt::Color DeepSkyBlue;
		static const Bolt::Color Olive;
		static const Bolt::Color DarkGreen;
		static const Bolt::Color LawnGreen;
		static const Bolt::Color Pink;
		static const Bolt::Color HotPink;
		static const Bolt::Color DeepPink;
		static const Bolt::Color Teal;
		static const Bolt::Color Turquoise;

	public:
		float r;
		float g;
		float b;
		float a;

	public:
		constexpr Color(byte r, byte g, byte b, byte a = 255)
			: r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f)
		{
		
		}

		constexpr Color(byte c) : Color(c, c, c, 255)
		{
		
		}

		constexpr Color() : Color(255, 255, 255, 255)
		{
		
		}

		float GetBrightness() const;
		Vector4f rgba() const;
		Vector4f abgr() const;
		Vector4f bgra() const;
		Vector3f rgb() const;
		Vector3f bgr() const;
		Vector2f rg() const;
		Vector2f gb() const;
		Vector2f rb() const;
		Vector4fr rgba();
		Vector4fr abgr();
		Vector4fr bgra();
		Vector3fr rgb();
		Vector3fr bgr();
		Vector2fr rg();
		Vector2fr gb();
		Vector2fr rb();

		uint32_t ToInt() const;
		Vector4<byte> ToBytes() const;

		bool operator==(const Color& other) const;
		bool operator!=(const Color& other) const;

		friend Color operator+(const Color& left, const Color& right);
		friend Color operator+(const Color& left, float right);
		friend Color operator+(float left, const Color& right);
		friend Color operator*(const Color& left, const Color& right);
		friend Color operator*(const Color& left, float right);
		friend Color operator*(float left, const Color& right);

		friend std::ostream& operator<<(std::ostream& stream, const Color& color);

	public:
		static Color FromFloats(float r, float g, float b, float a = 1.0f);
		// Hue [0, 360], Saturation [0, 1], Value [0, 1]
		static Color FromHSV(float hue, float saturation, float value);
		static Color Random();

	};

}

namespace std
{

	template<>
	struct hash<Bolt::Color>
	{
	public:
		size_t operator()(const Bolt::Color& color) const
		{
			return (size_t)color.ToInt();
		}
	};

}