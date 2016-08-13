//
//  SmearFrag.h
//  GLDemo
//
//  Created by tangbowen on 16/7/28.
//smearPositonTextureColor_frag
//

#define STRINGIFY(A)  #A
const char* SmearRGBAPositonTextureColor_frag = STRINGIFY(
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
uniform int b_reverse_shape;
                                                          
uniform sampler2D s_texture_shape;
uniform vec2 v_texSize_shape;
                                                           
uniform vec4 v_color_target;
uniform int v_taret_type_color;
                                                          
void main()
{
    vec4 texColor = texture2D(s_texture_paint, v_texCoord);
    
    // mimic: glAlphaFunc(GL_GREATER)
    // pass if ( incoming_pixel >= f_alpha_value_paint ) => fail if incoming_pixel < f_alpha_value_paint
    
    if ( texColor.a <= f_alpha_value_paint )
        discard;
    
    vec4 texColor_target = v_color_target;
    if(0 == v_taret_type_color){// texture
        vec2 v_texCoord2 = (v_texCoord_target + v_texCoord * v_texSize_paint) / v_texSize_target;
        texColor_target = texture2D(s_texture_target, v_texCoord2);
    }
    
    // mimic: glAlphaFunc(GL_GREATER)
    // pass if ( incoming_pixel >= f_alpha_value_paint ) => fail if incoming_pixel < f_alpha_value_paint
    //    if ( texColor_target.a < f_alpha_value_target )
    //        discard;
    vec2 v_texCoord2 = (v_texCoord_target + v_texCoord * v_texSize_paint) / v_texSize_shape;
    vec4 texColor_shape = texture2D(s_texture_shape, v_texCoord2);
    
    vec4 lColor;
    if(b_reverse_shape == 0 && 0.0 == texColor_shape.a){// normal
        discard;
    } else if(b_reverse_shape == 1 && 0.0 != texColor_shape.a){// reverse
        discard;
    } else {
        lColor = texColor_target;
        if(1 == b_paint_anti_aliasing){
            lColor *= texColor.a;
        }
    }
    
    
    lColor *= v_fragmentColor;
    
    if(0 != b_solid_enable){
        gl_FragColor = lColor;
    } else {
        float lRate = distance(v_texCoord, vec2(0));
        lRate *= 0.5;
        gl_FragColor =  vec4(lColor.r, lColor.g, lColor.b, lColor.a * lRate);
    }
}
);
