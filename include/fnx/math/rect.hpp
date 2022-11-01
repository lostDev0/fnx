#pragma once

namespace fnx
{
    /// @brief 2D Rectangle
    /// @note This rect acts like a 2d axis aligned bounding box. It cannot be skewed.
    struct rect
    {
        fnx::decimal _left{}, _top{}, _right{}, _bottom{};
        inline auto width() const { return _right - _left; }
        inline auto height() const { return _bottom - _top; }
        
        inline auto set_left(fnx::decimal left)
        {
            auto w = width();
            _left = left;
            _right = _left + w;
        }

        inline auto set_right(fnx::decimal right)
        {
            auto w = width();
            _right = right;
            _left = _right - w;
        }

        inline auto set_top(fnx::decimal top)
        {
            auto h = height();
            _top = top;
            _bottom = top - h;
        }

        inline auto set_bottom(fnx::decimal bottom)
        {
            auto h = height();
            _bottom = bottom;
            _top = bottom + h;
        }

    };
    
    /// @brief Checks that a point is within a coordinate system rect2d.
    /// @note 0,0 is top left of screen
    inline auto within(const rect& rect, const vector2& point)
    {
        auto x = point.x;
        auto y = point.y;
        auto l = rect._left;
        auto r = rect._right;
        auto t = rect._top;
        auto b = rect._bottom;
        return x >= l && x <= r && y >= t && y <= b;
    }

    inline auto align_within_rect(const fnx::rect& inner, const fnx::rect& outer, fnx::alignment align)
    {
        auto outer_width = outer._right;
        rect ret{ inner };
        if (align == alignment::left || align == alignment::top_left || align == alignment::bottom_left)
        {
            ret.set_left(outer._left);
        }
        else if (align == alignment::right || align == alignment::top_right || align == alignment::bottom_right)
        {
            ret.set_right(outer._right);
        }

        if (align == alignment::top || align == alignment::top_left || align == alignment::top_right)
        {
            ret.set_top(outer._top);
        }
        else if (align == alignment::bottom || align == alignment::bottom_left || align == alignment::bottom_right)
        {
            ret.set_bottom(outer._bottom);
        }

        if (align == alignment::center)
        {
            ret.set_left(outer._left + inner.width() / fnx::decimal{ 2 });
            ret.set_top(outer._top + inner.height() / fnx::decimal{ 2 });
        }
        return ret;
    }
}