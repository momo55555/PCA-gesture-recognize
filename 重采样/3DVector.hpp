#ifndef _3DVECTOR_H
#define _3DVECTOR_H

#include <cmath>

/*
    Time: 12-04-2012
    @Author: Rudy Snow
    Description: A simple 3D Vector Class
*/

namespace Vector3
{
    class Vector3f
    {
        public:
        // Default constructor
        Vector3f(){}

        // Copy constructor
        Vector3f( const Vector3f& a): x(a.x), y(a.y), z(a.z){}

        // With the parameters of the constructor
        Vector3f( float nx, float ny, float nz): x(nx), y(ny), z(nz){}

        // Overload "=" operator
        Vector3f& operator = (const Vector3f& a)
        {
            x = a.x; y = a.y; z = a.z;
            return *this ;
        }

        // Overload "==" and "!=" operator
        bool operator == (const Vector3f& a) const
        {
            return x == a.x && y == a.y && z == a.z;
        }

        bool operator != (const Vector3f& a) const
        {
            return x != a.x || y != a.y || z != a.z;
        }

        // Set to zero vector
        void zero()
        {
            x = y = z = 0.0f;
        }

        // Overload one dollar "-" operator
        Vector3f operator -() const
        {
            return Vector3f(-x, -y, -z);
        }
   
        // Overload tow dollar "+" and "-" operator
        Vector3f operator +(const Vector3f& a) const
        {
            return Vector3f(x+a.x, y+a.y, z+a.z);
        }

        Vector3f operator -(const Vector3f& a) const
        {
            return Vector3f(x-a.x, y-a.y, z-a.z);
        }

        // By the scalar multiplication and division
        Vector3f operator *(float a) const
        {
            return Vector3f(x*a, y*a, z*a);
        }
   
        Vector3f operator /(float a) const
        {
            float temp = 1.0f/a;
            return Vector3f(x*temp, y*temp, z*temp);
        }

        // Overload reflexive operator
        Vector3f& operator +=(const Vector3f& a)
        {
            x += a.x; y += a.y; z += a.z;
            return *this ;
        }

        Vector3f& operator -=(const Vector3f& a)
        {
            x -= a.x; y -= a.y; z -= a.z;
            return *this ;
        }

        Vector3f& operator *=(float a)
        {
            x *= a; y *= a; z *= a;
            return *this ;
        }

        Vector3f& operator /=(float a)
        {
            float temp = 1.0f/a;
            x *= temp; y *= temp; z *= temp;
            return *this ;
        }

        // Vector normalize
        void normalize()
        {
            float magSq = x*x + y*y + z*z;
            if (magSq > 0.0f)
            {
                float temp = 1.0f/sqrt(magSq);
                x *= temp;
                y *= temp;
                z *= temp;
            }
        }

        // Vector-point multiply, overload multiply the standard operator
        float operator *(const Vector3f& a) const
        {
            return x*a.x + y*a.y + z*a.z;
        }

        public:
        float x,y,z;
    };

    /*
        Non member function, for vector mode
    */
    inline float vectorMag(const Vector3f& a)
    {
        return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
    }

    // Compute by the tow vectors fork
    inline Vector3f crossProduct(const Vector3f& a, const Vector3f& b)
    {
        return Vector3f(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    }

    // Left by the realization of scalar
    inline Vector3f operator *(float k, const Vector3f& v)
    {
        return Vector3f(k*v.x, k*v.y, k*v.z);
    }

    // Compute the distance between two points
    inline float distance(const Vector3f& a, const Vector3f& b)
    {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        float dz = a.z - b.z;
        return sqrt(dx*dx + dy*dy + dz*dz);
    }

	// Compute the angle between two vectors
	inline float angle(const Vector3f& a, const Vector3f& b)
	{
		return a * b / (vectorMag(a)*vectorMag(b)) * 50.0f;
	}
}

#endif