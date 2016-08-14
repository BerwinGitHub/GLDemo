//
//  Coloring.frag
//
//
//  Created by renhong on 7/24/14.
//
//

const char* Coloring_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision highp float;
\n#endif\n
                                                    
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D s_texture_color;//start color
uniform sampler2D s_texture_color_end;//end color
uniform sampler2D s_texture_color_model;//coloring model
uniform sampler2D s_texture_shape;//draw target shape(shape by dif color)
uniform sampler2D s_texture_shape_bounds;//draw target bound box
uniform vec2 v_texture_shape_size;
                                        
const vec2 kColorModelPlat = vec2(0,0);
const vec2 kColorModelSmoothH = vec2(1,0);
const vec2 kColorModelSmoothV = vec2(0,1);
const vec2 kColorModelSmoothP = vec2(1,1);

vec2 pointCross(const vec2 begin, const vec2 end, const vec2 pt);
                                        
void main(){
    vec4 texColor = texture2D(CC_Texture0, v_texCoord);
    vec4 texShapeColor = texture2D(s_texture_shape, v_texCoord);
    vec2 texColorCoord = texShapeColor.rg;
    
    vec4 texColorBrush  = texture2D(s_texture_color, texColorCoord);
    vec4 texColorModel = texture2D(s_texture_color_model, texColorCoord);
    if(kColorModelPlat != texColorModel.rg){
        vec4 texColorBrushEnd = texture2D(s_texture_color_end, texColorCoord);
        if(texColorBrushEnd.rg != texColorBrush.rg || texColorBrushEnd.ba != texColorBrush.ba){//????
            vec4 shape_bounds = texture2D(s_texture_shape_bounds, texColorCoord);
            //if(v_texCoord.x > 0.4668 && v_texCoord.x < 0.57 ){// && v_texCoord.x < shape_bounds.g
//            if(v_texCoord.x > shape_bounds.r && v_texCoord.x < shape_bounds.g && v_texCoord.y > shape_bounds.b && v_texCoord.y < shape_bounds.a){
                vec3 deltaColor = (texColorBrushEnd - texColorBrush).rgb;
                if(kColorModelSmoothH == texColorModel.rg){//H-X
                    float unitX = 1.0 / (shape_bounds.g - shape_bounds.r);
                    texColorBrush = vec4(texColorBrush.rgb + deltaColor * (v_texCoord.x - shape_bounds.r) * unitX, 1.0);
                }
                else if(kColorModelSmoothV == texColorModel.rg){//V-Y
                    float unitY = 1.0 / (shape_bounds.a - shape_bounds.b);
                    texColorBrush = vec4(texColorBrush.rgb + deltaColor * (v_texCoord.y - shape_bounds.b) * unitY, 1.0);
                }
                else if(kColorModelSmoothP == texColorModel.rg){//P-XY
                    vec2 lefttop = vec2(shape_bounds.r * v_texture_shape_size.x, shape_bounds.b * v_texture_shape_size.y);
                    vec2 rightbottom = vec2(shape_bounds.g * v_texture_shape_size.x, shape_bounds.a * v_texture_shape_size.y);
                    vec2 cur = vec2(v_texCoord.x * v_texture_shape_size.x, v_texCoord.y * v_texture_shape_size.y);
                    vec2 ptCross = pointCross(lefttop, rightbottom, cur);
                    float width = (shape_bounds.g - shape_bounds.r) * v_texture_shape_size.x;
                    float height = (shape_bounds.a - shape_bounds.b) * v_texture_shape_size.y;
                    
                    ptCross = vec2(ptCross.x / v_texture_shape_size.x, ptCross.y / v_texture_shape_size.y);

                    if(height <= width){//横向矩形或正方形
                        texColorBrush = vec4(texColorBrush.rgb + (ptCross.x - shape_bounds.r) / (shape_bounds.g - shape_bounds.r) * deltaColor, 1.0);
                    }
                    else if(height > width){//纵向矩形或
                        texColorBrush = vec4(texColorBrush.rgb + (ptCross.y - shape_bounds.b) / (shape_bounds.a - shape_bounds.b) * deltaColor, 1.0);
                    }
                }
//            }
        }
    }
    gl_FragColor = texColorBrush * v_fragmentColor;
}
                                      
// 二维空间点到直线的垂足
vec2 pointCross(const vec2 begin, const vec2 end, const vec2 pt){
    vec2 retVal;
    
    float dx = begin.x - end.x;
    float dy = begin.y - end.y;
    if(abs(dx) < 0.00000001 && abs(dy) < 0.00000001 ){
        retVal = begin;
        return retVal;
    }
    
    float u = (pt.x - begin.x)*(begin.x - end.x) + (pt.y - begin.y)*(begin.y - end.y);
    u = u/((dx*dx)+(dy*dy));
    
    retVal.x = begin.x + u*dx;
    retVal.y = begin.y + u*dy;
    
    return retVal;
}
);