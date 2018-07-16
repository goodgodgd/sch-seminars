uniform mediump mat4 in_mvpmat;
uniform mediump vec3 in_lightpos;
attribute mediump vec4 in_posit;
attribute mediump vec3 in_normal;
attribute mediump float in_ptsize;
attribute mediump vec3 in_color;
varying mediump vec4 out_color;

void main(void)
{
	float highest = max(max(in_color.x, in_color.y), in_color.z);
	vec3 mid_color;
	if(highest < 0.3)
	{
		float addval = 0.3 - highest;
		mid_color = in_color + vec3(addval, addval, addval);
	}
	else
	{
		float mulval = min(highest*1.2, 1.0) / highest;
		mid_color = in_color*mulval;
	}
	
	vec3 lightdir = in_lightpos - in_posit.xyz;
	lightdir = normalize(lightdir);
    float dp = dot(lightdir, in_normal);
    float shading = 0.9 + 0.1*dp;

	out_color.xyz = mid_color*shading;
	out_color.w = 1.0;
    gl_Position = in_mvpmat * in_posit;
    gl_PointSize = in_ptsize;
}

