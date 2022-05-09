#ifndef _PHYSICS2D_H_
#define _PHYSICS2D_H_
#include <vector>

namespace phys2d
{
	template<typename _Ty>
	class Ref
	{
	private:
		_Ty* m_ref;
	public:
		Ref(_Ty& _v)
			:m_ref(&_v) {}

		const _Ty& value() const { return *m_ref; }
		_Ty& value() { return *m_ref; }
	};
	template<typename _Ty>
	class ConstRef
	{
	private:
		const _Ty* m_ref;
	public:
		ConstRef(const _Ty& _v)
			:m_ref(&_v) {}
		ConstRef(const Ref<_Ty>& _v)
			:ConstRef(_v.value()) {}

		const _Ty& value() const { return *m_ref; }
	};

	using value_t = float;

	using ValueRef = Ref<value_t>;
	using ValueConstRef = ConstRef<value_t>;

	// Vector

	class VectorRef
	{
	public:
		ValueRef x;
		ValueRef y;

		VectorRef(const ValueRef& _x, const ValueRef& _y)
			:x(_x), y(_y) {}
	};

	class VectorConstRef
	{
	public:
		ValueConstRef x;
		ValueConstRef y;

		VectorConstRef(const ValueConstRef& _x, const ValueConstRef& _y)
			:x(_x), y(_y) {}
		VectorConstRef(const VectorRef& _vec)
			: VectorConstRef(_vec.x, _vec.y) {}
	};

	class Vector : public VectorRef
	{
	private:
		value_t m_x;
		value_t m_y;

	public:
		Vector(value_t _x = 0, value_t _y = 0)
			:m_x(_x), m_y(_y), VectorRef(m_x, m_y) {}
	};

	// Line

	class LineRef
	{
	public:
		VectorRef p0;
		VectorRef p1;

		LineRef(const VectorRef& _p0, const VectorRef& _p1)
			:p0(_p0), p1(_p1) {}
	};

	class LineConstRef
	{
	public:
		VectorConstRef p0;
		VectorConstRef p1;

		LineConstRef(const VectorConstRef& _p0, const VectorConstRef& _p1)
			:p0(_p0), p1(_p1) {}
		LineConstRef(const LineRef& _line)
			:LineConstRef(_line.p0, _line.p1) {}
	};

	class Line : public LineRef
	{
	private:
		Vector m_p0, m_p1;
	public:
		Line(const Vector& _p0, const Vector& _p1)
			:m_p0(_p0), m_p1(_p1), LineRef(m_p0, m_p1) {}
	};

	// Border
	enum class BorderDir
	{
		None,
		Left,
		Right,
		Both
	};

	class BorderRef : public LineRef
	{
	public:
		Ref<BorderDir> dir;

		BorderRef(const LineRef& _line, const Ref<BorderDir>& _dir)
			:LineRef(_line), dir(_dir) {}
	};

	class BorderConstRef : public LineConstRef
	{
	public:
		ConstRef<BorderDir> dir;

		BorderConstRef(const LineConstRef& _line, const ConstRef<BorderDir>& _dir)
			:LineConstRef(_line), dir(_dir) {}
	};

	class Border : public BorderRef
	{
	private:
		Line m_line;
		BorderDir m_dir;
	public:
		Border(const Line& _line, BorderDir _dir)
			:m_line(_line), m_dir(_dir), BorderRef(m_line, m_dir) {}
	};

	// Collision
	class Collision
	{
	public:
		static bool is_hit(const LineConstRef& _line0, const LineConstRef& _line1)
		{
			auto tc1 = (_line0.p0.x.value() - _line0.p1.x.value()) * (_line1.p0.y.value() - _line0.p0.y.value()) + (_line0.p0.y.value() - _line0.p1.y.value()) * (_line0.p0.x.value() - _line1.p0.x.value());
			auto tc2 = (_line0.p0.x.value() - _line0.p1.x.value()) * (_line1.p1.y.value() - _line0.p0.y.value()) + (_line0.p0.y.value() - _line0.p1.y.value()) * (_line0.p0.x.value() - _line1.p1.x.value());
			auto td1 = (_line1.p0.x.value() - _line1.p1.x.value()) * (_line0.p0.y.value() - _line1.p0.y.value()) + (_line1.p0.y.value() - _line1.p1.y.value()) * (_line1.p0.x.value() - _line0.p0.x.value());
			auto td2 = (_line1.p0.x.value() - _line1.p1.x.value()) * (_line0.p1.y.value() - _line1.p0.y.value()) + (_line1.p0.y.value() - _line1.p1.y.value()) * (_line1.p0.x.value() - _line0.p1.x.value());
			return tc1 * tc2 < 0 && td1 * td2 < 0;
		}
	};
}

#endif // !_PHYSICS2D_H_