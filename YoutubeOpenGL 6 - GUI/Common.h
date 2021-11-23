#ifndef Common_h_INCLUDED
#define Common_h_INCLUDED
#define OPENGL_CHECK() if (auto xfdfdf=glGetError()) std::cout << "Opengl error " << xfdfdf << "! in line " << __LINE__ << "\n";
namespace core {
	class Pixel {
	public:
		union {
			uint32_t n;
			struct { uint8_t r, g, b, a; };
		};
		Pixel(uint32_t N=0):n(N) {}
		Pixel(uint8_t R, uint8_t G, uint8_t B, uint8_t A):r(R), g(G), b(B), a(A) {}
	};
	template <typename T>
	class Vecx3 {
	public:
		T x, y, z;
		Vecx3(T X=0, T Y=0, T Z=0) :x(X), y(Y), z(Z) {}
		friend Vecx3 operator+(const Vecx3& left, const Vecx3& right) { return { left.x + right.x, left.y + right.y, left.z + right.z }; }
		~Vecx3() {}
	};
	typedef Vecx3<float> Vec3f;
	typedef Vec3f Vec3;
	template <typename T>
	class Vecx2 {
	public:
		T x, y;
		Vecx2(T X=0, T Y=0) :x(X), y(Y) {}
		friend Vecx2 operator+(const Vecx2& left, const Vecx2& right) { return { left.x + right.x, left.y + right.y }; }
		~Vecx2() {}
	};
	typedef Vecx2<float> Vec2f;
	typedef Vec2f Vec2;
}
#endif