#include "Core.h"
#include <cmath>

#ifndef OMEGAWTK_CORE_MATH_H
#define OMEGAWTK_CORE_MATH_H

namespace OmegaWTK::Core {
    namespace Math {
    
    const long double PI = std::acos(-1);
    
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
        
        typedef Vector2D_Base<float,float> FVector2D;
        typedef Vector2D_Base<int,float> IVector2D;

        struct Point2D {
            unsigned x,y;
        };
    
        struct Arc {
            Point2D center;
            float radians;
            unsigned radius_x;
            unsigned radius_y;
        };
        struct Point3D {
            unsigned x,y,z;
        };


        template<class _Pt_Ty,class _Vec_Ty>
        class VectorPath_Base {
            public:
            struct Node {
                _Pt_Ty *pt;
                Node *next = nullptr;
            };

            struct Segment {
                _Pt_Ty * pt_A;
                _Pt_Ty * pt_B;
            };

            class Path_Iterator {
                Node *pt_A;
                Node *pt_B;
                public:
                Path_Iterator(Node *_data):pt_A(_data),pt_B(pt_A->next){
                    
                };
                Path_Iterator operator++(){
                    pt_A = pt_A->next;
                    pt_B = pt_B->next;
                };
                Path_Iterator operator+(unsigned num){
                    while(num > 0){
                        operator++();
                        --num;
                    };
                };
                Segment operator*(){
                    return Segment({pt_A->pt,pt_B->pt});
                };

            };
            using size_ty = unsigned;
            private:
            Node *first;
            size_ty len = 0;
            public:
            using iterator = Path_Iterator;
            iterator begin(){
                return iterator(first);
            };
            iterator end(){
                return iterator(first) + len;
            };
            const size_ty & size(){ return len;};
            private:
            void _push_pt(const _Pt_Ty & pt){
                
            };
            public:
            void append(const _Pt_Ty &pt){
                return _push_pt(pt);
            };
            void append(_Pt_Ty &&pt){
                return _push_pt(pt);
            };
        };

        
    };
};

#endif
