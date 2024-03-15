#version 330 core


float M_PI = radians(180.0);

in vec4 gl_FragCoord;

uniform vec3 cameraPos;
uniform vec3 camera_target;

// vec3 cameraPos = vec3(0,10,10);
// vec3 camera_target = vec3(0,0,0);

int SCR_WIDTH = 1800;
int SCR_HEIGHT = 1000;

vec3 camera_up = vec3(0.0, 1.0, 0.0);

float camera_fovy =  45.0;
float focalHeight = 1.0;
float aspect = float(SCR_WIDTH)/float(SCR_HEIGHT);
float focalWidth = focalHeight * aspect; //Height * Aspect ratio
float focalDistance = focalHeight/(2.0 * tan(camera_fovy * M_PI/(180.0 * 2.0)));

float SMALLEST_DIST = 1e-4;
float FLT_MAX =  3.402823466e+38;
float t = FLT_MAX;

out vec4 color;

struct World{
    vec3 bgcolor;
    vec3 ambient_color;
    float ambience;
};
World world;

struct Ray {
        vec3 origin;
        vec3 direction;
        float t;
        bool hit;
        int hit_object_index;
};

struct PointLight{
    vec3 position;
    int lightMaterial_index;
};

const int num_lights = 2;
PointLight light_set[num_lights];
struct LightMaterial{
    vec3 intensity;
    vec3 color;
};
const int num_lightMaterial = 2;

LightMaterial lightMaterial_set[num_lightMaterial];    
struct Sphere {
        vec3 centre;
        float radius;
        int type_enum;
        int material_index;
};

const int num_objects = 4;
Sphere object_set[num_objects+num_lights];
// All lights are at the end of array

const int LIGHT = 0;
const int BLINN_PHONG = 1;
const int REFLECTIVE = 2;
const int REFRACTIVE = 3;


struct BlinnPhongMaterial{

    float k_a;
    vec3 c_a; //ambient color
    
    //Diffuse
    float k_d;
    vec3 c_r; //diffuse reflectance
    
    //Specular
    float k_s; 
    vec3 c_p; // phong highlight
    int n; // phong exponent
};
const int num_blinnPhongMaterial = 2;
BlinnPhongMaterial blinnPhong_set[num_blinnPhongMaterial];



void intersect(inout Ray r, int s);
vec4 shade_blinn_phong(inout Ray r);
vec4 shade_light(inout Ray r);

void main() {

    world.bgcolor = vec3(0.28,0.28,0.28);
    world.ambient_color = vec3(0.28,0.28,0.28);
    world.ambience = 0.8;

    blinnPhong_set[0].k_a = 0.3;
    blinnPhong_set[0].c_a =  vec3(1.0, 0.43, 0.14);
    blinnPhong_set[0].k_d = 0.8;
    blinnPhong_set[0].c_r = vec3(1.0, 0.43, 0.14); 
    blinnPhong_set[0].k_s = 0.2; 
    blinnPhong_set[0].c_p = vec3(1.0,1.0,1.0); 
    blinnPhong_set[0].n = 2; 

    blinnPhong_set[1].k_a = 0.3;
    blinnPhong_set[1].c_a =  vec3(1.0, 0.0, 0.9);
    blinnPhong_set[1].k_d = 0.2;
    blinnPhong_set[1].c_r = vec3(0.0, 0.0, 1.0); 
    blinnPhong_set[1].k_s = 0.9; 
    blinnPhong_set[1].c_p = vec3(1.0,1.0,1.0); 
    blinnPhong_set[1].n = 100; 

    light_set[0].position = vec3(0,10,10);
    light_set[0].lightMaterial_index = 0;
    lightMaterial_set[0].intensity = vec3(1.0,1.0,1.0);
    lightMaterial_set[0].color = vec3(1.0,1.0,1.0);

    light_set[1].position = vec3(-10,10,10);
    light_set[1].lightMaterial_index = 1;
    lightMaterial_set[1].intensity = vec3(1.0,1.0,1.0);
    lightMaterial_set[1].color = vec3(1.0,1.0,1.0);

    object_set[0].centre = vec3(0, 0, 0);
    object_set[0].radius = 1;
    object_set[0].type_enum = BLINN_PHONG;
    object_set[0].material_index = 0;

    object_set[1].centre = vec3(-2, 0, 0);
    object_set[1].radius = 1;
    object_set[1].type_enum = BLINN_PHONG;
    object_set[1].material_index = 1;   

    object_set[2].centre = vec3(0, 2, 2);
    object_set[2].radius = 1;
    object_set[2].type_enum = BLINN_PHONG;
    object_set[2].material_index = 1;   
    
    object_set[3].centre = camera_target;
    object_set[3].radius = 0.1;
    object_set[3].type_enum = BLINN_PHONG;
    object_set[3].material_index = 0;   

    object_set[4].centre = light_set[0].position;
    object_set[4].radius = 0.1;
    object_set[4].type_enum = LIGHT;
    object_set[4].material_index = 0;   
    
    object_set[5].centre = light_set[1].position;
    object_set[5].radius = 0.1;
    object_set[5].type_enum = LIGHT;
    object_set[5].material_index = 1;   

    vec3 line_of_sight = camera_target - cameraPos;
    vec3 w = -normalize(line_of_sight);
    vec3 u = normalize(cross(camera_up, w));
    vec3 v = normalize(cross(w, u));
    float i = gl_FragCoord.x;
    float j = gl_FragCoord.y;
    vec3 dir = vec3(0.0, 0.0, 0.0);
	dir += -w * focalDistance;
	float xw = aspect*(i - SCR_WIDTH/2.0 + 0.5)/SCR_WIDTH;
	float yw = (j - SCR_HEIGHT/2.0 + 0.5)/SCR_HEIGHT;
	dir += u * xw;
	dir += v * yw;
    
    Ray r;
    r.origin = cameraPos;
    r.direction = normalize(dir);
    r.t = FLT_MAX;
    r.hit = false;
    r.hit_object_index = -1;
    
    for(int i = 0 ; i < num_objects+num_lights; i++){
        intersect(r,i);
    }

    if (r.hit){
        switch(object_set[r.hit_object_index].type_enum){
            
            case BLINN_PHONG:
                color = shade_blinn_phong(r);
                break;

            case  LIGHT:
                color = shade_light(r);
        }
    } else {
        color = vec4(world.bgcolor,1.0);
    }
};

vec4 shade_light(inout Ray r){
    Sphere object = object_set[r.hit_object_index];
    LightMaterial material = lightMaterial_set[object.material_index];
    vec3 color  = material.intensity*material.color;
    return vec4(color,1.0);
}

vec4 shade_blinn_phong(inout Ray r){

    Sphere object = object_set[r.hit_object_index];
    BlinnPhongMaterial material = blinnPhong_set[object.material_index];
    
    vec3 color = vec3(0.0,0.0,0.0);
    vec3 intersectionPosition = r.origin + r.t*r.direction;
    vec3 v = normalize(r.origin-intersectionPosition);
    vec3 n = normalize(intersectionPosition - object.centre);
    for(int i = 0; i < num_lights; i++ ){
        PointLight light = light_set[i]; 
        LightMaterial lightMaterial = lightMaterial_set[light.lightMaterial_index];

        vec3 l = normalize(light.position - intersectionPosition);
        vec3 h = normalize(v+l);
        
        Ray shadow_ray;
        shadow_ray.origin = intersectionPosition;
        shadow_ray.direction = l;
        shadow_ray.t = FLT_MAX;
        shadow_ray.hit = false;
        shadow_ray.hit_object_index = -1;

        for(int i = 0 ; i < num_objects; i++){
           intersect(shadow_ray,i);
        }
        
        float diff = max(dot(n,l),0);
        vec3 diff_color = material.c_r*lightMaterial.color*diff;

        vec3 ambient_color = material.c_r*material.c_a + world.ambience*world.ambient_color;

        float spec = pow(max(dot(n,h),0),material.n);  
        vec3 spec_color = spec*material.c_p*lightMaterial.color;

        vec3 L = lightMaterial.intensity*(material.k_a*ambient_color + 
                  material.k_d*diff_color + material.k_s*spec_color);
        
        if (shadow_ray.hit){
            color = color + material.k_a*ambient_color;
        } else {
            color = color + L;
        }
    }
    return vec4(color,1.0);
}
void intersect(inout Ray r, int index) {
    Sphere s = object_set[index];
    float a = dot(r.direction,r.direction);
    float b = dot(r.direction, 2.0 * (r.origin-s.centre));
    float c = dot(s.centre, s.centre) + dot(r.origin,r.origin) +
              -2.0*dot(r.origin,s.centre) - (s.radius*s.radius);
    
    float disc = b*b + (-4.0)*a*c;
    
    if (disc < 0){
        return;
    }
    
    float D = sqrt(disc);
	float t1 = (-b +D)/(2.0*a);
	float t2 = (-b -D)/(2.0*a);

    if(t1 < r.t && t1 > SMALLEST_DIST){
        r.hit = true;
        r.t = t1;
        r.hit_object_index = index;
    }

    if(t2 < r.t && t2 > SMALLEST_DIST){
        r.hit = true;
        r.t = t2;
        r.hit_object_index = index;
    }
}