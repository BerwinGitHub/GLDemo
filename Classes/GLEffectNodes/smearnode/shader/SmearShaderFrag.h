//
//  SmearFrag.h
//  GLDemo
//
//  Created by tangbowen on 16/7/28.
//
//

#define STRINGIFY(A)  #A
const char* smearPositonTextureColor_frag = STRINGIFY(
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
uniform float f_solid_rate;
uniform int b_paint_anti_aliasing;

void main()
{
    vec4 texColor = texture2D(s_texture_paint, v_texCoord);
    if ( texColor.a <= f_alpha_value_paint )						
        discard;
    
    vec2 v_texCoord2 = (v_texCoord_target + v_texCoord * v_texSize_paint) / v_texSize_target;
    vec4 texColor1 = texture2D(s_texture_target, v_texCoord2);
    if ( texColor1.a < f_alpha_value_target )
        discard;
    
    vec4 lColor = v_fragmentColor * texColor1;
    if(1 == b_paint_anti_aliasing)// 1 = the paint atni
        lColor *= texColor.a;
    
    if(0 != b_solid_enable) {
        gl_FragColor = lColor;// solid
    } else {
        gl_FragColor = vec4(0, 0, 0, f_solid_rate);// soft transition
    }
}
);


