#ifdef GL_ES
precision highp float;
#endif


uniform vec4 coefficient; /* inverted, radius, gradient, brightness. */
uniform vec4 color;

uniform vec2 touchPoint;
uniform vec2 resolution;

uniform sampler2D CC_Texture0;

float getMask(float radius, vec2 pos, vec2 centre)
{
    float dist = distance(pos, centre);
    
    if (dist < radius) {
        float dd = dist/radius;
        
        return smoothstep(0.0, coefficient.z, 1.0 - pow(dd, coefficient.w));
    }
    
    return 0.0;
}

void main(void)
{
    float aspe = resolution.x / resolution.y;
    
	vec2 uv = (gl_FragCoord.xy / resolution.xy) * 2.0 - 1.0;
    vec2 pos = uv;
    pos.x *= aspe;
    
    vec2 centre = (touchPoint.xy / resolution.xy) * 2.0 - 1.0;
    centre.x *= aspe;
    
    vec4 tc = texture2D(CC_Texture0, vec2(uv.x, 1.0-uv.y));
    
    float mask = getMask(coefficient.y, pos, centre);
    
    if (coefficient.x == 0.0)
        gl_FragColor = vec4(tc*mask*color);
    else
        gl_FragColor = vec4(tc*(1.0-coefficient.x*mask*color));
}