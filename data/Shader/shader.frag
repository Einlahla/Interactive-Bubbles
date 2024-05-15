#version 120

uniform vec2 u_resolution;
uniform float u_time;

#define M_PI 3.1415926535897932384626433832795

/* plot関数 */
float plot(vec2 st, float pct){
    return  smoothstep( pct-0.01, pct, st.y) -
    smoothstep( pct, pct+0.01, st.y);
}


/* 乱数 その1 */
float n11(vec2 uv) {
    return fract(uv.x*5764.287135 + uv.y*6453.54181) ;
}
/* 乱数 その2 */
float n12(float n) {
    return fract(n*5764.287135 + n*6453.54181) ;
}

/* 正弦波を重ね合わせる */
vec3 multisin(vec2 uv, float n ) {
    //float i = 1.;
    vec3 col = vec3(0.0) ;
    float t = u_time ;
    float alpha = 7494.680418;
    float y = 0.0;
    const float s = 2.5/10.0;
    float fadeio = abs(cos(t*0.20)*cos(t*0.20)*cos(t*0.20)) ;
    vec3 basecol = vec3(0.55,0.0,0.6) ;
    
    for(float i=0.; i < 1. ; i+=s) {
        y = 0.33*exp(i*1.5*n12(i))
        *sin( uv.x/n*10.0 + t*0.3*i*0.5 + n12(n)
        - t*0.2*fract(i*40393.02745) + M_PI*float(int(i/s)/4) )  ;
        
        col += plot(vec2(uv.x, uv.y+i*0.1), y)
        *(basecol + vec3(0,i/5.0*n*0.5 ,n*0.075) )
         ;
    }
    return col;
    
}
/*
 背景色を塗る関数 
vec3 bgcolor(vec2 uv){
    float m1=0.;
    m1=clamp(0.0,0.5,uv.y*uv.y);
    return vec3(0.0,0.0,m1);
}
*/
void main(){
    vec2 uv=gl_FragCoord.xy/u_resolution.xy;
    uv-=0.5;
    float asp=u_resolution.x/u_resolution.y;
    uv.x*=asp;
    
    vec3 col=vec3(0.0);
    //col+=bgcolor(uv);
    
    col+=multisin(uv,8.0);
    
    gl_FragColor=vec4(col,1.0);
}