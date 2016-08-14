//
//  ScribbleShader.frag
//
//
//  Created by renhong on 7/24/14.
//
//

const char* ScribbleShapeShader_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision highp float;
\n#endif\n
                                                    
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D s_texture_brush;
uniform sampler2D s_texture_target;
uniform sampler2D s_texture_shape;
uniform float f_alpha_value_brush;
uniform float f_alpha_value_target;
uniform vec2 v_texCoord_target;
uniform vec2 v_texSize_brush;
uniform vec2 v_texSize_target;
uniform vec2 v_texSize_shape;
uniform int v_solid_brush;
uniform vec4 v_color_target;
uniform int v_taret_type_color;
uniform int anti_aliasing_brush;
                                                 
void main()
{
    vec4 texColor = texture2D(s_texture_brush, v_texCoord);
    
	// mimic: glAlphaFunc(GL_GREATER)						
	// pass if ( incoming_pixel >= f_alpha_value_brush ) => fail if incoming_pixel < f_alpha_value_brush                  
    
	if ( texColor.a <= f_alpha_value_brush )						
        discard;
    
    vec4 texColor_target = v_color_target;
    if(0 == v_taret_type_color){
        vec2 v_texCoord2 = (v_texCoord_target + v_texCoord * v_texSize_brush) / v_texSize_target;
        texColor_target = texture2D(s_texture_target, v_texCoord2);
    }
    
    // mimic: glAlphaFunc(GL_GREATER)
	// pass if ( incoming_pixel >= f_alpha_value_brush ) => fail if incoming_pixel < f_alpha_value_brush
//    if ( texColor_target.a < f_alpha_value_target )
//        discard;
    vec2 v_texCoord2 = (v_texCoord_target + v_texCoord * v_texSize_brush) / v_texSize_shape;
    vec4 texColor_shape = texture2D(s_texture_shape, v_texCoord2);
    
    vec4 lColor;
    if(0.0 == texColor_shape.a){
        discard;
    }
    else{
        lColor = texColor_target;
        if(1 == anti_aliasing_brush){
            lColor *= texColor.a;
        }
    }
    lColor *= v_fragmentColor;

    if(0 != v_solid_brush){
        gl_FragColor = lColor;
    }
    else{
        float lRate = distance(v_texCoord, vec2(0));
        lRate /= 2.0;
        gl_FragColor =  vec4(lColor.r, lColor.g, lColor.b, lColor.a * lRate);
    }
}
);
