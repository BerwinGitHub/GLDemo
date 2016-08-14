//
//  SmearFrag.h
//  GLDemo
//
//  Created by tangbowen on 16/7/28.
//
//

#define STRINGIFY(A)  #A
const char* SmearSegmentPositonTextureColor_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision highp float;
\n#endif\n

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D s_texture_paint;
uniform sampler2D s_texture_target;
uniform float f_alpha_value_paint;
uniform float f_alpha_value_target;
uniform vec2 v_texCoord_target;
uniform vec2 v_texSize_paint;
uniform vec2 v_texSize_target;
uniform int b_solid_enable;
uniform int b_paint_anti_aliasing;
                                                          
uniform sampler2D s_texture_segment;
uniform vec2 v_texSize_segment;
                                                             
uniform vec4 v_color_segment;
uniform vec4 v_color_target;
uniform int b_color_enable;
                                                             
bool vec3equal(vec3 v1, vec3 v2);
                                                             
void main()
{
    vec4 texColor = texture2D(s_texture_paint, v_texCoord);
    if ( texColor.a <= f_alpha_value_paint )
        discard;
    
    vec4 texColor_target = v_color_target;
    vec2 v_texCoord2 = (v_texCoord_target + v_texCoord * v_texSize_paint) / v_texSize_segment;
    vec4 texColor_segment = texture2D(s_texture_segment, v_texCoord2);
    
    if(!vec3equal(texColor_segment.rgb, v_color_segment.rgb * texColor_segment.a))
        discard;
    
    vec4 lColor;
    if(0.0 == texColor_segment.a) {
        discard;
    } else {
        if(0 == b_color_enable){
            vec2 v_texCoord3 = (v_texCoord_target + v_texCoord * v_texSize_paint) / v_texSize_target;
            v_texCoord3 = vec2(clamp(v_texCoord3.x, 0.0, 1.0), clamp(v_texCoord3.y, 0.0, 1.0));
            lColor = texture2D(s_texture_target, v_texCoord3);
        } else if (1 == b_color_enable){
            lColor = texColor_target;
        }
    }
    
    if(1 == b_paint_anti_aliasing){
        lColor *= texColor.a;
    }
    
    lColor *= texColor_segment.a;//涂画对象边缘抗锯齿
    lColor *= v_fragmentColor;
    gl_FragColor = lColor;
}
                                                             
 bool vec3equal(vec3 v1, vec3 v2){
//     float EPSINON = 0.00392;//0的逼近值应该落在正负最小表示值之间
     //4112.1;//4112.0;//4096.0;//0.001;//1.0/255 ～～ 0.00392，0.00001;
//     float tolerance = EPSINON > f_tolerance ? EPSINON : f_tolerance;
     float tolerance = 0.00392;
     vec3 t = v1 - v2;
     if(abs(t.r) <= tolerance && abs(t.g) <= tolerance && abs(t.b) <= tolerance){
         return true;
     }
     return false;
 }
);

/*
#define STRINGIFY(A)  #A
const char* SmearSegmentPositonTextureColor_frag = STRINGIFY(
 \n#ifdef GL_ES\n
 precision highp float;
 \n#endif\n
 
 varying vec4 v_fragmentColor;
 varying vec2 v_texCoord;
 
 uniform sampler2D s_texture_paint;
 uniform sampler2D s_texture_target;
 uniform float f_alpha_value_paint;
 uniform float f_alpha_value_target;
 uniform vec2 v_texCoord_target;
 uniform vec2 v_texSize_paint;
 uniform vec2 v_texSize_target;
 uniform int b_solid_enable;
 uniform int b_paint_anti_aliasing;
 
 uniform sampler2D s_texture_segment;
 uniform vec2 v_texSize_segment;
 
 uniform vec4 v_color_segment;
 uniform vec4 v_color_target;
 uniform int b_color_enable;
 
 bool vec3equal(vec3 v1, vec3 v2);
 
 void main(){//10  252  244
     vec4 texColor = texture2D(s_texture_paint, v_texCoord);
     if ( texColor.a <= f_alpha_value_paint )
         discard;
     
     vec4 texColor_target = v_color_target;
     vec2 v_texCoord2 = (v_texCoord_target + v_texCoord * v_texSize_paint) / v_texSize_segment;
     vec4 texColor_segment = texture2D(s_texture_segment, v_texCoord2);
     
     if(!vec3equal(texColor_segment.rgb, v_color_segment.rgb * texColor_segment.a
                   )){
         discard;
     }
     
     vec4 lColor;
     if(0.0 == texColor_segment.a){
         discard;
     }
     else{
         if(0 == b_color_enable){
             vec2 v_texCoord2 = (v_texCoord_target + v_texCoord * v_texSize_paint) / v_texSize_target;
             v_texCoord2 = vec2(clamp(v_texCoord2.x, 0.0, 1.0), clamp(v_texCoord2.y, 0.0, 1.0));
             lColor = texture2D(s_texture_target, v_texCoord2);
         }
         else if(1 == b_color_enable){
             lColor = texColor_target;
         }
     }
     if(1 == b_paint_anti_aliasing){//画刷抗锯齿
         lColor *= texColor.a;
     }
     lColor *= texColor_segment.a;//涂画对象边缘抗锯齿
     lColor *= v_fragmentColor;
     gl_FragColor = lColor;
 }
 
 bool vec3equal(vec3 v1, vec3 v2){
     //     float EPSINON = 1.0 / 255.0;//1.0/4096.0;//0的逼近值应该落在正负最小表示值之间
     //4112.1;//4112.0;//4096.0;//0.001;//1.0/255 ～～ 0.00392，0.00001;
     //     float lTolerance = EPSINON > f_tolerance ? EPSINON : f_tolerance;
     float tolerance = 0.00392;
     vec3 t = v1 - v2;
     if(abs(t.r) <= tolerance && abs(t.g) <= tolerance && abs(t.b) <= tolerance){
         return true;
     }
     return false;
 }
 );*/

