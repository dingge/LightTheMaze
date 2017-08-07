varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main(void)
{
    vec4 v_orColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
    gl_FragColor = vec4(0,0,0,1.0-v_orColor.a)*v_fragmentColor.a;
}
