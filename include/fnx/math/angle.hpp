#pragma once

#include <ostream>

namespace fnx
{
	constexpr auto DEG_TO_RAD = 0.0174533;	/// convert degrees to radians
    constexpr auto RAD_TO_DEG = 57.2958;	/// convert radians to degrees
    
	/// @brief Ensures explicit declaration of Degree values for an angle.
	struct Degree
    {
        Degree(double deg) : _deg{ deg } {} 
        double _deg{0.0};
    };

    /// @brief Ensures explicit declaration of Radian values for an angle.
    struct Radian
    {
        Radian(double rad) : _rad{rad} {} 
		double _rad{0.0};
    };

    /// @brief Represents an angle in either degrees or radians.
    struct angle
    {
        angle() = default;
        ~angle() = default;
        explicit angle(const fnx::Degree& degree) : _rad_val{degree._deg * fnx::DEG_TO_RAD} {}
        explicit angle(const fnx::Radian& radian) : _rad_val{radian._rad} {}
        
        friend std::ostream& operator<<( std::ostream &out, const angle& angle ) { out << angle._rad_val; return out; }
        bool operator<(const angle& other) const { return _rad_val < other._rad_val; }
		bool operator== (const angle& rhs) { return !(rhs < *this) && !(*this < rhs); }
		bool operator!= (const angle& rhs) { return !(*this == rhs); }
		bool operator> (const angle& rhs) { return rhs < *this; }
		bool operator<= (const angle& rhs) { return !(rhs < *this); }
		bool operator>= (const angle& rhs) { return !(*this < rhs); }
        
        /// @brief Return the angle value as a Radian.
		/// @return radian value
        auto get_as_radian() const { return _rad_val; }

        /// @brief Return the angle value as a Degree.
        /// @return degree value
        auto get_as_degree() const { return _rad_val * RAD_TO_DEG; }
        
        angle operator+(const angle& other) const { return angle(Radian(_rad_val + other._rad_val)); }
        angle operator-(const angle& other) const { return angle(Radian(_rad_val - other._rad_val)); }
        angle operator*(const angle& other) const { return angle(Radian(_rad_val * other._rad_val)); }
        angle operator/(const angle& other) const { return angle(Radian(_rad_val / other._rad_val)); }
        
        angle operator+(const Radian& rad) const { return angle(Radian(_rad_val + rad._rad)); }
        angle operator-(const Radian& rad) const { return angle(Radian(_rad_val - rad._rad)); }
        angle operator*(const Radian& rad) const { return angle(Radian(_rad_val * rad._rad)); }
        angle operator/(const Radian& rad) const { return angle(Radian(_rad_val / rad._rad)); }
        
        angle operator+(const Degree& deg) const { return angle(Radian(_rad_val + deg._deg * DEG_TO_RAD)); }
        angle operator-(const Degree& deg) const { return angle(Radian(_rad_val - deg._deg * DEG_TO_RAD)); }
        angle operator*(const Degree& deg) const { return angle(Radian(_rad_val * deg._deg * DEG_TO_RAD)); }
        angle operator/(const Degree& deg) const { return angle(Radian(_rad_val / deg._deg * DEG_TO_RAD)); }
        
        angle& operator+=(const angle& other) { *this = *this + other; return *this; }
        angle& operator-=(const angle& other) { *this = *this - other; return *this; }
        angle& operator*=(const angle& other) { *this = *this * other; return *this; }
        angle& operator/=(const angle& other) { *this = *this / other; return *this; }
        
        angle& operator+=(const Radian& other) { *this = *this + other; return *this; }
        angle& operator-=(const Radian& other) { *this = *this - other; return *this; }
        angle& operator*=(const Radian& other) { *this = *this * other; return *this; }
        angle& operator/=(const Radian& other) { *this = *this / other; return *this; }
        
        angle& operator+=(const Degree& other) { *this = *this + other; return *this; }
        angle& operator-=(const Degree& other) { *this = *this - other; return *this; }
        angle& operator*=(const Degree& other) { *this = *this * other; return *this; }
        angle& operator/=(const Degree& other) { *this = *this / other; return *this; }
        
        private:
            double _rad_val{0.0};   /// The angle is stored in radians.
    };
}
