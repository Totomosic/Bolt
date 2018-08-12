#pragma once
#include "Types.h"
#include "Math\__Math__.h"

namespace Bolt
{

	struct BLT_API Color
	{
	public:
		inline static const Bolt::Color White = Bolt::Color(255, 255, 255, 255);
		inline static const Bolt::Color Black = Bolt::Color(0, 0, 0, 255);

		inline static const Bolt::Color Red = Bolt::Color(255, 0, 0, 255);
		inline static const Bolt::Color Green = Bolt::Color(0, 255, 0, 255);
		inline static const Bolt::Color Blue = Bolt::Color(0, 0, 255, 255);

		inline static const Bolt::Color Yellow = Bolt::Color(255, 255, 0, 255);
		inline static const Bolt::Color Cyan = Bolt::Color(0, 255, 255, 255);
		inline static const Bolt::Color Magenta = Bolt::Color(255, 0, 255, 255);

		inline static const Bolt::Color CornflowerBlue = Bolt::Color(100, 149, 237, 255);
		inline static const Bolt::Color Purple = Bolt::Color(128, 0, 128, 255);
		inline static const Bolt::Color Indigo = Bolt::Color(75, 0, 130, 255);
		inline static const Bolt::Color Lavender = Bolt::Color(230, 230, 250, 255);
		inline static const Bolt::Color Violet = Bolt::Color(238, 130, 238, 255);
		inline static const Bolt::Color Brown = Bolt::Color(165, 42, 42, 255);
		inline static const Bolt::Color SaddleBrown = Bolt::Color(139, 69, 19, 255);
		inline static const Bolt::Color Tan = Bolt::Color(210, 180, 140, 255);
		inline static const Bolt::Color Gold = Bolt::Color(255, 215, 0, 255);
		inline static const Bolt::Color Orange = Bolt::Color(255, 165, 0, 255);
		inline static const Bolt::Color Navy = Bolt::Color(0, 0, 128, 255);
		inline static const Bolt::Color RoyalBlue = Bolt::Color(65, 105, 225, 255);
		inline static const Bolt::Color SkyBlue = Bolt::Color(135, 206, 235, 255);
		inline static const Bolt::Color DeepSkyBlue = Bolt::Color(0, 191, 255, 255);
		inline static const Bolt::Color Olive = Bolt::Color(128, 128, 0, 255);
		inline static const Bolt::Color DarkGreen = Bolt::Color(0, 100, 0, 255);
		inline static const Bolt::Color LawnGreen = Bolt::Color(124, 252, 0, 255);
		inline static const Bolt::Color Pink = Bolt::Color(255, 192, 203, 255);
		inline static const Bolt::Color HotPink = Bolt::Color(255, 105, 180, 255);
		inline static const Bolt::Color DeepPink = Bolt::Color(255, 20, 147, 255);
		inline static const Bolt::Color Teal = Bolt::Color(0, 128, 128, 255);
		inline static const Bolt::Color Turquoise = Bolt::Color(64, 224, 208, 255);

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

		size_t ToInt() const;
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
		// Hue (0, 360), Saturation (0, 1), Value(0, 1)
		static Color FromHSV(float hue, float saturation, float value);
		static Color Random();

	};

}