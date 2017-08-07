
varying vec4 v_fragmentColor;	
varying vec2 v_texCoord;	

void main()
{
    vec4 v_orColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
    float gray = v_orColor.r*0.299 + v_orColor.g*0.587 + v_orColor.b*0.114;
    gl_FragColor = vec4(gray,gray,gray,v_orColor.a);
}
