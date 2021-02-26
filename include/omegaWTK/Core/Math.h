#include "Core.h"
#include <cmath>
#include <iostream>
#include <algorithm>

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


        template<class _Pt_Ty>
        class VectorPath_Base {
            public:
            struct Node {
                _Pt_Ty *pt;
                Node *next = nullptr;
                Node(_Pt_Ty *pt):pt(pt){};
                ~Node(){
                    delete pt;
                    if(next){
                        delete next;
                    };
                };
            };

            struct Segment {
                _Pt_Ty ** pt_A;
                _Pt_Ty ** pt_B;
            };

            class Path_Iterator {
                Node *pt_A;
                Node *pt_B;
                unsigned pos;
                public:
                Path_Iterator(Node *_data):pt_A(_data),pt_B(pt_A->next),pos(0){
                    
                };
                Path_Iterator operator++(){
                    pt_A = pt_A->next;
                    pt_B = pt_B->next;
                    ++pos;
                    return *this;
                };
                bool operator==(const Path_Iterator & r){
//                    std::cout << "Self Pos:" << pos << ", Other Pos:" << r.pos << std::endl;
                    return pos == r.pos;
                };
                bool operator!=(const Path_Iterator & r){
                    return !operator==(r);
                };
                Path_Iterator operator+(unsigned num){
                    while(num > 0){
                        operator++();
                        --num;
                    };
                    return *this;
                };
                Segment operator*(){
                    return Segment({&pt_A->pt,&pt_B->pt});
                };

            };
            using size_ty = unsigned;
            private:
            Node *first;
            float numPoints = 0;
            size_ty len = 0;
            public:
            using iterator = Path_Iterator;
            iterator begin(){
                return iterator(first);
            };
            iterator end(){
                return iterator(first) + (len);
            };
            const size_ty & size(){ return len;};
            private:
            void _push_pt(const _Pt_Ty & pt){
                Node ** pt_b = &first->next;
                unsigned idx = len;
                while(idx > 0){
                    pt_b = &((*pt_b)->next);
                    --idx;
                };
                *(pt_b) = new Node(new _Pt_Ty(std::move(pt)));
                ++len;
            };
            public:
            void append(const _Pt_Ty &pt){
                return _push_pt(pt);
            };
            void append(_Pt_Ty &&pt){
                return _push_pt(pt);
            };
            Core::String toStr(){
                std::ostringstream out_;
                out_ << "VectorPath Size:" << size() << std::endl;
                auto it = begin();
                
                while(it != end()){
                    auto segment = *it;
                    _Pt_Ty *pt_A = *segment.pt_A;
                    _Pt_Ty *pt_B = *segment.pt_B;
                    if(sizeof(_Pt_Ty) == sizeof(Point2D)){
                        out_ << "Segment {" << "[x:" << pt_A->x << ",y:" << pt_A->y << "] [x:" << pt_B->x << ",y:" << pt_B->y << "] }"<< std::endl;
                    }
                    else if(sizeof(_Pt_Ty) == sizeof(Point3D)){
//                        out_ << "Segment {" << "[x:" << pt_A->x << ",y:" << pt_A->y << ",z:" << pt_A->z << "] [x:" << pt_B->x << ",y:" << pt_B->y << ",z:" << pt_B->z << "] }"<< std::endl;
                    };
                    ++it;
                };
                return out_.str();
            };
            VectorPath_Base(const _Pt_Ty & start):first(new Node(new _Pt_Ty(std::move(start)))),len(0),numPoints(1){};
            VectorPath_Base(_Pt_Ty &&start):first(new Node(new _Pt_Ty(std::move(start)))),len(0),numPoints(1){};
            VectorPath_Base(VectorPath_Base<_Pt_Ty> & other){
                Node *& pt_a = other.first;
                first = new Node(new _Pt_Ty(*(pt_a->pt)));
                Node *&next = first->next;
                Node *& pt_b = other.first->next;
                next = new Node(new _Pt_Ty(*(pt_b->pt)));
                unsigned idx = other.len-1;
                while(idx > 0){
                    next = next->next;
                    if(pt_b) {
                        pt_b = pt_b->next;
                        if(pt_b)
                            next = new Node(new _Pt_Ty(*(pt_b->pt)));
                    }
                    --idx;
                };
                len = other.len;
                
            };
            VectorPath_Base() = delete;
            VectorPath_Base(const VectorPath_Base<_Pt_Ty> &) = delete;
            VectorPath_Base(VectorPath_Base<_Pt_Ty> && other):first(other.first),len(other.len){};
            ~VectorPath_Base(){
                delete first;
            };
        };
    
        typedef VectorPath_Base<Point2D> VectorPath2D;
        typedef VectorPath_Base<Point3D> VectorPath3D;

    };

};

#endif
