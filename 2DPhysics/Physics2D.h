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
		Line(const VectorConstRef& _p0, const VectorConstRef& _p1)
			:m_p0(_p0.x.value(),_p0.y.value()), m_p1(_p1.x.value(), _p1.y.value()), LineRef(m_p0, m_p1) {}
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
		BorderConstRef(const BorderRef& _border)
			:BorderConstRef(LineConstRef(_border.p0, _border.p1), _border.dir) {}
	};

	class Border : public BorderRef
	{
	private:
		Line m_line;
		BorderDir m_dir;
	public:
		Border(const LineConstRef& _line, BorderDir _dir)
			:m_line(_line.p0,_line.p1), m_dir(_dir), BorderRef(m_line, m_dir) {}
	};

	// Collision
	class Collision
	{
	public:
		static Vector add(const VectorConstRef& _v0, const VectorConstRef& _v1)
		{
			return Vector(_v0.x.value() + _v1.x.value(), _v0.y.value() + _v1.y.value());
		}

		static Vector sub(const VectorConstRef& _v0, const VectorConstRef& _v1)
		{
			return Vector(_v0.x.value() - _v1.x.value(), _v0.y.value() - _v1.y.value());
		}

		static Vector times(const VectorConstRef& _v, const ValueConstRef& _a)
		{
			return Vector(_v.x.value() * _a.value(), _v.y.value() * _a.value());
		}

		static value_t dot_product(const VectorConstRef& _v0, const VectorConstRef& _v1)
		{
			return _v0.x.value() * _v1.x.value() + _v0.y.value() * _v1.y.value();
		}

		static value_t cross_product(const VectorConstRef& _v0, const VectorConstRef& _v1)
		{
			return _v0.x.value() * _v1.y.value() - _v1.x.value() * _v0.y.value();
		}

		static bool is_hit(const LineConstRef& _line0, const LineConstRef& _line1)
		{
			auto atb = sub(_line0.p1, _line0.p0);
			auto tc1 = cross_product(atb, sub(_line1.p0, _line0.p0));
			auto tc2 = cross_product(atb, sub(_line1.p1, _line0.p0));

			auto ctd = sub(_line1.p1, _line1.p0);
			auto td1 = cross_product(ctd, sub(_line0.p0, _line1.p0));
			auto td2 = cross_product(ctd, sub(_line0.p1, _line1.p0));

			return tc1 * tc2 < 0 && td1 * td2 < 0;
		}

		static value_t hit_time(const VectorConstRef& _p, const VectorConstRef& _v, const LineConstRef& _line, value_t _max_t)
		{
			if (!is_hit(Line(_p, add(_p, times(_v, _max_t))), _line)) return _max_t;
			auto v = sub(_line.p0, _p);
			auto v1 = sub(_line.p1, _line.p0);
			return cross_product(v, v1) / cross_product(v1, _v);
		}
	};
}

#endif // !_PHYSICS2D_H_