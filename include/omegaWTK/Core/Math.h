#include "Core.h"
#include <cmath>

#ifndef OMEGAWTK_CORE_MATH_H
#define OMEGAWTK_CORE_MATH_H

namespace OmegaWTK::Core {
    namespace Math {
        template<class _Num_Ty,typename _Angle_Ty>
        class Vector2D_Base {
            _Num_Ty i;
            _Num_Ty j;
            public:
            Vector2D_Base(_Num_Ty _i,_Num_Ty _j):i(_i),j(_j){};
            Vector2D_Base(_Num_Ty mag,_Angle_Ty angle){
                i = cos(angle) * mag;
                j = sin(angle) * mag;
            };
            _Num_Ty getI(){
                return i;
            };
            _Num_Ty getJ(){
                return j;
            };
            /// Get magnitude
            _Num_Ty magnitude(){
                return sqrt(pow(i,2) + pow(j,2));
            };
            /// Get angle relative to `i` !
            _Angle_Ty angle(){
                return atan(j/i);
            };
            private:
            void add(const Vector2D_Base<_Num_Ty,_Angle_Ty> & vector2d){
                i += vector2d.i;
                j += vector2d.j;
            };
            void subtract(const Vector2D_Base<_Num_Ty,_Angle_Ty> & vector2d){
                i -= vector2d.i;
                j -= vector2d.j;
            };
            public:
            Vector2D_Base<_Num_Ty,_Angle_Ty> operator+(const Vector2D_Base<_Num_Ty,_Angle_Ty> & vec){
                add(std::move(vec));
            };
            Vector2D_Base<_Num_Ty,_Angle_Ty> operator-(const Vector2D_Base<_Num_Ty,_Angle_Ty> & vec){
                subtract(std::move(vec));
            };
            void operator+=(const Vector2D_Base<_Num_Ty,_Angle_Ty> & vec){
                add(std::move(vec));
            };
            void operator-=(const Vector2D_Base<_Num_Ty,_Angle_Ty> & vec){
                subtract(std::move(vec));
            };
            Vector2D_Base<_Num_Ty,_Angle_Ty> dot(const Vector2D_Base<_Num_Ty,_Angle_Ty> & vec){
                return (i * vec.i) + (j * vec.j);
            };
        };
        
        typedef Vector2D_Base<float,float> Vector2D;
    };
};

#endif