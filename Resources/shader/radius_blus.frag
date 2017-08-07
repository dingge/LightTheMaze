varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec2  u_resolution;
uniform vec2  u_focus_pos;                  //焦点中心

const float c_offset_length_rate = 0.05;    //采样长度比（采样长度 = 距焦点距离*采样长度比）
const float c_split_num = 10.2;             //采样总数

void main(void)
{
    vec2 current_pos = v_texCoord * u_resolution;
    float current_angle = atan(current_pos.y - u_focus_pos.y, current_pos.x - u_focus_pos.x);
    float len = distance(current_pos, u_focus_pos);
    float offset_len = len * c_offset_length_rate;
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    vec2 normal_direction = vec2(cos(current_angle), sin(current_angle));
    for (float i = -offset_len; i < offset_len; i = i + offset_len * 2.0/c_split_num)
    {
        vec2 temp_offset = i * normal_direction;
        color += texture2D(CC_Texture0, (current_pos + temp_offset)/u_resolution)/c_split_num;
    }
    gl_FragColor = color;
}