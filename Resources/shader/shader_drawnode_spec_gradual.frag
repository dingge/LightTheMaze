#ifdef GL_ES
#extension GL_OES_standard_derivatives : enable

varying mediump vec4 v_color;
varying mediump vec2 v_texcoord;
#else
varying vec4 v_color;
varying vec2 v_texcoord;
#endif

void main()
{
    float n = 2.0/(dot(v_texcoord-vec2(0.0,1.0), vec2(1.0,-1.0))/2.0);
    float r = dot(v_texcoord, vec2(1.0,1.0))/2.0*n-(n-1.0)/2.0;
    gl_FragColor = v_color*clamp(r, 0.0, 1.0);
    //gl_FragColor = v_color*clamp(1.0 - length(v_texcoord), 0.0, 1.0);
}