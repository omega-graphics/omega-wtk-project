#include "omegaWTK/Composition/Brush.h"

namespace OmegaWTK::Composition {

Color Color::create8Bit(std::uint8_t r,std::uint8_t g,std::uint8_t b,std::uint8_t a)
{
    Color color;
    color.r = float(r)/0xFF;
    color.g = float(g)/0xFF;
    color.b = float(b)/0xFF;
    color.a = float(a)/0xFF;
    return color;
};

Color Color::create16Bit(std::uint16_t r,std::uint16_t g,std::uint16_t b,std::uint16_t a)
{
    Color color;
    color.r = float(r)/0xFFFF;
    color.g = float(g)/0xFFFF;
    color.b = float(b)/0xFFFF;
    color.a = float(a)/0xFFFF;
    return color;
};

Color Color::create8Bit(std::uint32_t hex_color,std::uint8_t alpha){
    uint8_t mask = 0xFF;
    Color c {};
    /// BB
    c.b = float(hex_color & mask)/float(mask);
    hex_color = hex_color >> 8;
    /// GG
    c.g = float(hex_color & mask)/float(mask);
    hex_color = hex_color >> 8;
    /// RR
    c.r = float(hex_color & mask)/float(mask);

    c.a = float(alpha)/float(mask);
    return c;
};

Color Color::create16Bit(std::uint64_t hex_color,std::uint16_t alpha){
    uint16_t mask = 0xFFFF;
    Color c {};
    /// BB
    c.b = float(hex_color & mask)/float(mask);
    hex_color = hex_color >> 16;
    /// GG
    c.g = float(hex_color & mask)/float(mask);
    hex_color = hex_color >> 16;
    /// RR
    c.r = float(hex_color & mask)/float(mask);

    c.a = float(alpha)/float(mask);
    return c;
};

bool Color::compare(const Color &other){
    return (r == other.r) && (g == other.g) &&(b == other.b) && (a == other.a);
};

const Color Color::Transparent = Color::create8Bit(Color::Eight::Black8,0x00);

Gradient::GradientStop Gradient::Stop(float pos, Color color){
    return {pos,color};
};

Gradient Gradient::Linear(std::initializer_list<GradientStop> stops,float angle){
    Gradient grad;
    grad.stops = stops;
    grad.arg = angle;
    grad.type = Type::Linear;
    return grad;
};

Gradient Gradient::Radial(std::initializer_list<GradientStop> stops,float radius){
    Gradient grad;
    grad.stops = stops;
    grad.arg = radius;
    grad.type = Type::Radial;
    return grad;
};

Brush::Brush(const Color & color):isColor(true),color(color),isGradient(false){
    
};

Brush::Brush(const Gradient & gradient):isColor(false),color(Color::create8Bit(0,0)),isGradient(true),gradient(gradient){
    
};

Core::SharedPtr<Brush> ColorBrush(const Color & color){
    return std::make_shared<Brush>(color);
};

Core::SharedPtr<Brush> GradientBrush(const Gradient & gradient){
    return std::make_shared<Brush>(gradient);
};

}
