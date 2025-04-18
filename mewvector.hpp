#ifndef MEW_VECTOR_SO2U
#define MEW_VECTOR_SO2U

#include <math.h>
#include "mewlib.h"

namespace mew {
	float lerp_value(float a, float b, float t) {
		return a + (t * (b - a));
	}
	
	template<uint L, typename T> struct vec {
		typedef vec<L, T> self_type;
		T data[L];

		self_type& operator+=(const self_type& l) {
			for (uint i = 0; i < L; ++i) {
				data[i] += l.data[i];
			}
			return *this;
		}
		self_type& operator-=(const self_type& l) {
			for (uint i = 0; i < L; ++i) {
				data[i] -= l.data[i];
			}
			return *this;
		}
		self_type& operator*=(const self_type& l) {
			for (uint i = 0; i < L; ++i) {
				data[i] *= l.data[i];
			}
			return *this;
		}
		self_type& operator/=(const self_type& l) {
			for (uint i = 0; i < L; ++i) {
				data[i] /= l.data[i];
			}
			return *this;
		}

		friend self_type operator+(const self_type& r, const self_type& l) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = r.data[i] + l.data[i];
			}
			return result;
		}
		friend self_type operator-(const self_type& r, const self_type& l) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = r.data[i] - l.data[i];
			}
			return result;
		}
		friend self_type operator*(const self_type& r, const self_type& l) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = r.data[i] * l.data[i];
			}
			return result;
		}
		friend self_type operator/(const self_type& r, const self_type& l) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = r.data[i] / l.data[i];
			}
			return result;
		}
		friend self_type operator+(const self_type& r, T l) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = r.data[i] + l;
			}
			return result;
		}
		friend self_type operator-(const self_type& r, T l) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = r.data[i] - l;
			}
			return result;
		}
		friend self_type operator*(const self_type& r, T l) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = r.data[i] * l;
			}
			return result;
		}
		friend self_type operator/(const self_type& r, T l) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = r.data[i] / l;
			}
			return result;
		}
		friend self_type operator+(T l, const self_type& r) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = l + r.data[i];
			}
			return result;
		}
		friend self_type operator-(T l, const self_type& r) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = l - r.data[i];
			}
			return result;
		}
		friend self_type operator*(T l, const self_type& r) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = l * r.data[i];
			}
			return result;
		}
		friend self_type operator/(T l, const self_type& r) {
			self_type result;
			for (uint i = 0; i < L; ++i) {
				result.data[i] = l / r.data[i];
			}
			return result;
		}
	};


	template<typename T>
	struct vec<3, T> {
		typedef vec<3, T> self_type;

		union {
			struct { T x, y, z; };
		};

		vec<2, T> xy() { return vec<2, T>{x, y}; }
		vec<2, T> xz() { return vec<2, T>{x, z}; }
		vec<2, T> yz() { return vec<2, T>{y, z}; }

		self_type& operator+=(const self_type& l) {
			x += l.x; 
			y += l.y; 
			z += l.z;
			return *this;
		}
		self_type& operator-=(const self_type& l) {
			x -= l.x; 
			y -= l.y; 
			z -= l.z;
			return *this;
		}
		self_type& operator*=(const self_type& l) {
			x *= l.x; 
			y *= l.y; 
			z *= l.z;
			return *this;
		}
		self_type& operator/=(const self_type& l) {
			x /= l.x; 
			y /= l.y; 
			z /= l.z;
			return *this;
		}
		
		static self_type Zero;

		static self_type lerp(const self_type& a, const self_type& b, T t) {
			return self_type(a.x + t * (b.x - a.x), a.y + t * (b.y - a.y), a.z + t * (b.z - a.z));
		}
		
		bool isZero(float limit = 0.001f) {
			return fabsf(x) < limit && fabsf(y) < limit && fabsf(z) < limit;
		}
		
		bool isZero(uint zero) {
			return x == 0 && y == 0 && z == 0;
		}

		bool isZero() {
			return isZero((T)(0));
		}

		self_type& zero() {
			x = 0;
			y = 0;
			z = 0;
			return *this;
		}

		friend self_type operator+(const self_type& r, const self_type& l) {
			return self_type{r.x + l.x, r.y + l.y, r.z + l.z};
		}
		friend self_type operator-(const self_type& r, const self_type& l) {
			return self_type{r.x - l.x, r.y - l.y, r.z - l.z};
		}
		friend self_type operator*(const self_type& r, const self_type& l) {
			return self_type{r.x * l.x, r.y * l.y, r.z * l.z};
		}
		friend self_type operator/(const self_type& r, const self_type& l) {
			return self_type{r.x / l.x, r.y / l.y, r.z / l.z};
		}
		friend self_type operator+(const self_type& r, T l) {
			return self_type{r.x + l, r.y + l, r.z + l};
		}
		friend self_type operator-(const self_type& r, T l) {
			return self_type{r.x - l, r.y - l, r.z - l};
		}
		friend self_type operator*(const self_type& r, T l) {
			return self_type{r.x * l, r.y * l, r.z * l};
		}
		friend self_type operator/(const self_type& r, T l) {
			return self_type{r.x / l, r.y / l, r.z / l};
		}
		friend self_type operator+(T l, const self_type& r) {
			return self_type{l + r.x, l + r.y, l + r.z};
		}
		friend self_type operator-(T l, const self_type& r) {
			return self_type{l - r.x, l - r.y, l - r.z};
		}
		friend self_type operator*(T l, const self_type& r) {
			return self_type{l * r.x, l * r.y, l * r.z};
		}
		friend self_type operator/(T l, const self_type& r) {
			return self_type{l / r.x, l / r.y, l / r.z};
		}
	};

	template<typename T>
	struct vec<2, T> {
		typedef vec<2, T> self_type;

		union {
			struct { T x, y; };
		};

		self_type& operator+=(const self_type& l) {
			x += l.x; 
			y += l.y;
			return *this;
		}
		self_type& operator-=(const self_type& l) {
			x -= l.x; 
			y -= l.y;
			return *this;
		}
		self_type& operator*=(const self_type& l) {
			x *= l.x; 
			y *= l.y;
			return *this;
		}
		self_type& operator/=(const self_type& l) {
			x /= l.x; 
			y /= l.y;
			return *this;
		}
		self_type& operator+=(T l) {
			x += l; 
			y += l;
			return *this;
		}
		self_type& operator-=(T l) {
			x -= l; 
			y -= l;
			return *this;
		}
		self_type& operator*=(T l) {
			x *= l; 
			y *= l;
			return *this;
		}
		self_type& operator/=(T l) {
			x /= l; 
			y /= l;
			return *this;
		}

		self_type& lerp(self_type target, T t) {
			while (t > 1) { t /= 10; }
			x += t * (target.x - x);
			y += t * (target.y - y);
			return *this;
		}
		

		friend self_type operator+(const self_type& r, const self_type& l) {
			return self_type(r.x+l.x, r.y+l.y);
		}
		friend self_type operator-(const self_type& r, const self_type& l) {
			return self_type(r.x-l.x, r.y-l.y);
		}
		friend self_type operator*(const self_type& r, const self_type& l) {
			return self_type(r.x*l.x, r.y*l.y);
		}
		friend self_type operator/(const self_type& r, const self_type& l) {
			return self_type(r.x/l.x, r.y/l.y);
		}
		friend self_type operator+(const self_type& r, T l) {
			return self_type(r.x+l, r.y+l);
		}
		friend self_type operator-(const self_type& r, T l) {
			return self_type(r.x-l, r.y-l);
		}
		friend self_type operator*(const self_type& r, T l) {
			return (self_type){(T)r.x*l, (T)r.y*l};
		}
		friend self_type operator/(const self_type& r, T l) {
			return (self_type){(T)r.x/l, (T)r.y/l};
		}
		friend self_type operator+(T l, const self_type& r) {
			return self_type(r.x+l, r.y+l);
		}
		friend self_type operator-(T l, const self_type& r) {
			return self_type(r.x-l, r.y-l);
		}
		friend self_type operator*(T l, const self_type& r) {
			return self_type(r.x*l, r.y*l);
		}
		friend self_type operator/(T l, const self_type& r) {
			return self_type(r.x/l, r.y/l);
		}

		static self_type Zero;

		static self_type lerp(const self_type& a, const self_type& b, T t) {
			return self_type(a.x + t * (b.x - a.x), a.y + t * (b.y - a.y));
		}
		
		bool isZero(float limit = 0.001f) {
			return fabsf(x) < limit && fabsf(y) < limit;
		}
		
		bool isZero(uint zero) {
			return x == 0 && y == 0;
		}

		bool isZero() {
			return isZero((T)(0));
		}


		self_type& zero() {
			x = 0;
			y = 0;
			return *this;
		}
	};
	typedef vec<2, float> vec2;
	template<>
	vec2 vec2::Zero{0.0f,0.0f};
	typedef vec<2, uint> vec2u;
	template<>
	vec2u	vec2u::Zero{0,0};
	template<typename T>
	using vec3 = vec<3, T>;
	
	void test() {
		vec3<float> v1{1.0f, 2.0f, 3.0f};
		vec3<float> v2{4.0f, 5.0f, 6.0f};
		vec3<float> v3 = v1 + v2;	
	}

	template<typename T>
	struct Buffer {
		T* data;
		uint size;
		uint capacity;

		Buffer(uint cap) : size(0), capacity(cap) {
			data = new T[capacity];
		}
		~Buffer() {
			delete[] data;
		}
		void push_back(const T& value) {
			if (size < capacity) {
				data[size++] = value;
			}
		}
		void clear() {
			size = 0;
		}
	};
	
	struct RGBA {
		uint8_t r, g, b, a;
	};

	struct RGB {
		uint8_t r, g, b;
	};

	struct HSL {
		float h, s, l;
	};

	struct HSV {
		float h, s, v;
	};


	struct FontAtlas {
		Buffer<RGBA> atlas;
		uint charWidth, charHeight;
		uint columns, rows;

		FontAtlas(uint atlasWidth, uint atlasHeight, uint charW, uint charH)
			: atlas(atlasWidth * atlasHeight), charWidth(charW), charHeight(charH) {
			columns = atlasWidth / charWidth;
			rows = atlasHeight / charHeight;
		}

		void setChar(uint index, const RGBA* charData) {
			uint x = (index % columns) * charWidth;
			uint y = (index / columns) * charHeight;

			for (uint i = 0; i < charHeight; ++i) {
				for (uint j = 0; j < charWidth; ++j) {
					atlas.data[(y + i) * (columns * charWidth) + (x + j)] = charData[i * charWidth + j];
				}
			}
		}

		void getChar(uint index, RGBA* charData) const {
			uint x = (index % columns) * charWidth;
			uint y = (index / columns) * charHeight;

			for (uint i = 0; i < charHeight; ++i) {
				for (uint j = 0; j < charWidth; ++j) {
					charData[i * charWidth + j] = atlas.data[(y + i) * (columns * charWidth) + (x + j)];
				}
			}
		}
	};
	
	struct CompressedFontAtlas {
		Buffer<uint8_t> atlas; // Compressed data
		uint charWidth, charHeight;
		uint columns, rows;

		CompressedFontAtlas(uint atlasWidth, uint atlasHeight, uint charW, uint charH)
			: atlas(atlasWidth * atlasHeight / 8), charWidth(charW), charHeight(charH) {
			columns = atlasWidth / charWidth;
			rows = atlasHeight / charHeight;
		}

		void setChar(uint index, const uint8_t* charData) {
			uint x = (index % columns) * charWidth;
			uint y = (index / columns) * charHeight;

			for (uint i = 0; i < charHeight; ++i) {
				for (uint j = 0; j < charWidth; ++j) {
					uint bitIndex = (y + i) * (columns * charWidth) + (x + j);
					uint byteIndex = bitIndex / 8;
					uint bitOffset = bitIndex % 8;

					if (charData[i * charWidth + j]) {
						atlas.data[byteIndex] |= (1 << bitOffset);
					} else {
						atlas.data[byteIndex] &= ~(1 << bitOffset);
					}
				}
			}
		}

		void getChar(uint index, uint8_t* charData) const {
			uint x = (index % columns) * charWidth;
			uint y = (index / columns) * charHeight;

			for (uint i = 0; i < charHeight; ++i) {
				for (uint j = 0; j < charWidth; ++j) {
					uint bitIndex = (y + i) * (columns * charWidth) + (x + j);
					uint byteIndex = bitIndex / 8;
					uint bitOffset = bitIndex % 8;

					charData[i * charWidth + j] = (atlas.data[byteIndex] & (1 << bitOffset)) ? 1 : 0;
				}
			}
		}
	};
	

	struct Image {
		Buffer<RGBA> pixels;
		uint width, height;
		Image(uint w, uint h) : width(w), height(h), pixels(w * h) {}
		void fill(RGBA color) {
			for (uint i = 0; i < pixels.size; ++i) {
				pixels.data[i] = color;
			}
		}
		void clear() {
			pixels.clear();
		}
		void resize(uint w, uint h) {
			if (w * h > pixels.capacity) {
				delete[] pixels.data;
				pixels.data = new RGBA[w * h];
				pixels.capacity = w * h;
			}
			width = w;
			height = h;
		}
		void setPixel(uint x, uint y, RGBA color) {
			if (x < width && y < height) {
				pixels.data[y * width + x] = color;
			}
		}
		RGBA getPixel(uint x, uint y) const {
			if (x < width && y < height) {
				return pixels.data[y * width + x];
			}
			return {0, 0, 0, 0}; // Return transparent black if out of bounds
		}
		void drawLine(int x0, int y0, int x1, int y1, RGBA color) {
			int dx = abs(x1 - x0);
			int dy = abs(y1 - y0);
			int sx = (x0 < x1) ? 1 : -1;
			int sy = (y0 < y1) ? 1 : -1;
			int err = dx - dy;

			while (true) {
				setPixel(x0, y0, color);
				if (x0 == x1 && y0 == y1) break;
				int err2 = err * 2;
				if (err2 > -dy) {
					err -= dy;
					x0 += sx;
				}
				if (err2 < dx) {
					err += dx;
					y0 += sy;
				}
			}
		}
		void drawRect(int x, int y, int w, int h, RGBA color) {
			drawLine(x, y, x + w - 1, y, color); // Top
			drawLine(x, y, x, y + h - 1, color); // Left
			drawLine(x + w - 1, y, x + w - 1, y + h - 1, color); // Right
			drawLine(x, y + h - 1, x + w - 1, y + h - 1, color); // Bottom
		}
		void drawCircle(int x0, int y0, int radius, RGBA color) {
			int x = radius;
			int y = 0;
			int err = 0;

			while (x >= y) {
				setPixel(x0 + x, y0 + y, color);
				setPixel(x0 + y, y0 + x, color);
				setPixel(x0 - y, y0 + x, color);
				setPixel(x0 - x, y0 + y, color);
				setPixel(x0 - x, y0 - y, color);
				setPixel(x0 - y, y0 - x, color);
				setPixel(x0 + y, y0 - x, color);
				setPixel(x0 + x, y0 - y, color);

				if (err <= 0) {
					y += 1;
					err += 2 * y + 1;
				}
				if (err > 0) {
					x -= 1;
					err -= 2 * x + 1;
				}
			}
		}
		void drawEllipse(int x0, int y0, int a, int b, RGBA color) {
			int x = 0;
			int y = b;
			int a2 = a * a;
			int b2 = b * b;
			int err = 0;

			while (x * b2 < y * a2) {
				setPixel(x0 + x, y0 + y, color);
				setPixel(x0 - x, y0 + y, color);
				setPixel(x0 + x, y0 - y, color);
				setPixel(x0 - x, y0 - y, color);

				err += 2 * b2 * (x + 1);
				if (err > 2 * a2 * (y - 1)) {
					y -= 1;
					err -= 2 * a2 * y;
				}
				x += 1;
			}

			x = a;
			y = 0;
			err = 0;

			while (x * b2 > y * a2) {
				setPixel(x0 + x, y0 + y, color);
				setPixel(x0 - x, y0 + y, color);
				setPixel(x0 + x, y0 - y, color);
				setPixel(x0 - x, y0 - y, color);

				err += 2 * a2 * (y + 1);
				if (err > 2 * b2 * (x - 1)) {
					x -= 1;
					err -= 2 * b2 * x;
				}
				y += 1;
			}
		}
		void drawImage(int x, int y, const Image& img) {
			for (uint i = 0; i < img.width; ++i) {
				for (uint j = 0; j < img.height; ++j) {
					setPixel(x + i, y + j, img.getPixel(i, j));
				}
			}
		}
		void drawText(int x, int y, const char* text, RGBA color, int scale = 1) {
			// Placeholder implementation for drawing text
			// This assumes a simple 8x8 pixel font stored in a bitmap or similar
			// You would need to implement or integrate a font rendering system for actual text rendering
			for (size_t i = 0; i < strlen(text); ++i) {
				int charX = x + i * 8 * scale;
				int charY = y;
				// Render each character as a placeholder rectangle
				drawRect(charX, charY, 8 * scale, 8 * scale, color);
			}
		}
		void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, RGBA color) {
			drawLine(x0, y0, x1, y1, color);
			drawLine(x1, y1, x2, y2, color);
			drawLine(x2, y2, x0, y0, color);
		}
	};
}

#endif