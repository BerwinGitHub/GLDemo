//
//  ScribblePatternShader.frag
//
//
//  Created by renhong on 4/2/15.
//
//

const char* ScribblePatternShader_frag = STRINGIFY(
\n#ifdef GL_ES\n
precision highp float;
\n#endif\n
                                                    
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D s_texture_brush;
uniform sampler2D s_texture_pattern;
uniform float f_tolerance;
uniform sampler2D s_texture_target;
uniform float f_alpha_value_brush;
uniform vec2 v_texCoord_target;
uniform vec2 v_texSize_brush;
uniform vec2 v_texSize_pattern;
uniform vec4 v_color_target;
uniform vec4 v_pattern_color;
uniform vec2 v_texSize_target;
uniform int v_taret_type_color;
uniform int anti_aliasing_brush;
                                                   
bool vec3equal(vec3 pLeft, vec3 pRight);
                                                   
void main(){//10  252  244
    vec4 texColor = texture2D(s_texture_brush, v_texCoord);
	if ( texColor.a <= f_alpha_value_brush )
        discard;
    
    vec4 texColor_target = v_color_target;
    vec2 v_texCoord2 = (v_texCoord_target + v_texCoord * v_texSize_brush) / v_texSize_pattern;
    vec4 texColor_pattern = texture2D(s_texture_pattern, v_texCoord2);
    
    if(!vec3equal(texColor_pattern.rgb, v_pattern_color.rgb * texColor_pattern.a
                  )){
        discard;
    }
    
    vec4 lColor;
    if(0.0 == texColor_pattern.a){
        discard;
    }
    else{
        if(0 == v_taret_type_color){
            vec2 v_texCoord2 = (v_texCoord_target + v_texCoord * v_texSize_brush) / v_texSize_target;
            v_texCoord2 = vec2(clamp(v_texCoord2.x, 0.0, 1.0), clamp(v_texCoord2.y, 0.0, 1.0));
            lColor = texture2D(s_texture_target, v_texCoord2);
        }
        else if(1 == v_taret_type_color){
            lColor = texColor_target;
        }
    }
    if(1 == anti_aliasing_brush){//画刷抗锯齿
        lColor *= texColor.a;
    }
    lColor *= texColor_pattern.a;//涂画对象边缘抗锯齿
    lColor *= v_fragmentColor;
    gl_FragColor = lColor;
}
                                                   
bool vec3equal(vec3 pLeft, vec3 pRight){
    float EPSINON = 1.0 / 255.0;//1.0/4096.0;//0的逼近值应该落在正负最小表示值之间
    //4112.1;//4112.0;//4096.0;//0.001;//1.0/255 ～～ 0.00392，0.00001;
    float lTolerance = EPSINON > f_tolerance ? EPSINON : f_tolerance;
    vec3 lTmp = pLeft - pRight;
    if(abs(lTmp.r) <= lTolerance && abs(lTmp.g) <= lTolerance && abs(lTmp.b) <= lTolerance){
        return true;
    }
    return false;
}
);
