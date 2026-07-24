$input v_texcoord0

#include <bgfx_shader.sh>

uniform vec4 u_uvRect;
SAMPLER2D(s_texColor, 0);

void main()
{
    vec2 uv = mix(u_uvRect.xy, u_uvRect.zw, v_texcoord0);
    gl_FragColor = texture2D(s_texColor, uv);
}
