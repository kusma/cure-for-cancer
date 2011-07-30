/*
class Wave : public MeshPlugin{
public:
	float wobble;
	float detail;
	Wave(){
		this->detail = 0.1f;
	}
	void Apply(){
		for(int i=0; i<this->owner->vertex_count; i++){
			Vector* v = &this->owner->current_vertex_list[i].point;
			this->vertex_list[i].point.x = v->x + (float)sin( sin(wobble*0.3f+v->z) );//this->detail;
			this->vertex_list[i].point.y = v->y + (float)sin( sin(wobble*0.8f*2+v->y) );//*this->detail;
			this->vertex_list[i].point.z = v->z + (float)sin( sin(wobble*0.1f*4+v->x+v->y) );//*this->detail;
		}
		this->owner->current_vertex_list = this->vertex_list;
		this->owner->GenerateNormals();
	}
};
*/
/*
class Wave : public MeshPlugin{
public:
	float wobble;
	float detail;
	Wave(){
		this->detail = 0.1f;
	}
	void Apply(){
		for(int i=0; i<this->owner->vertex_count; i++){
			Vector* v = &this->owner->current_vertex_list[i].point;
			this->vertex_list[i].point.x = v->x + (float)sin( sin(wobble*0.3f+v->z)*0.1f )*this->detail;
			this->vertex_list[i].point.y = v->y + (float)sin( sin(wobble*0.8f*2+v->y)*0.1f )*this->detail;
			this->vertex_list[i].point.z = v->z + (float)sin( sin(wobble*0.1f*4+v->x+v->y)*0.1f )*this->detail;
		}
		this->owner->current_vertex_list = this->vertex_list;
		this->owner->GenerateNormals();
	}
};
*/

class Blobb : public MeshPlugin{
public:
	Vector wobble;
	float power;
	int wavex;
	int wavey;
	int wavez;
	Blobb( int wavex, int wavey, int wavez ){
		this->wavex = wavex;
		this->wavey = wavey;
		this->wavez = wavez;
		this->power = 0.2f;
		vertex_list = NULL;
		this->owner = NULL;
	}
	void Wobble(Vector wobble){
		this->wobble = wobble;
	}
	void Apply(){

		if(power==0.0f) return;
		for(int i=0; i<this->owner->vertex_count; i++){

			Vector* v = &this->owner->current_vertex_list[i].point;
			float xdeg = 1+(float)cos( wobble.x+atan2(v->z,v->y)*this->wavex )*this->power;
			float ydeg = 1+(float)cos( wobble.y+atan2(v->x,v->z)*this->wavey )*this->power;
			float zdeg = 1+(float)cos( wobble.z+atan2(v->y,v->x)*this->wavez )*this->power;

			this->vertex_list[i] = this->owner->current_vertex_list[i];

			this->vertex_list[i].point.x = v->x*ydeg*zdeg;
			this->vertex_list[i].point.y = v->y*zdeg*xdeg;
			this->vertex_list[i].point.z = v->z*xdeg*ydeg;

		}
		this->owner->current_vertex_list = this->vertex_list;
		this->owner->GenerateNormals();

	}
};

/*
class Blobb : public MeshPlugin{
public:
	float wobble;
	float power;
	int wave;
	Blobb( int wave ){
		this->wave = wave;
		this->power = 0.2f;
	}
	void Apply(){
		for(int i=0; i<this->owner->vertex_count; i++){

			Vector* v = &this->owner->current_vertex_list[i].point;
			float ang = atan2(v->x,v->y);
			float dist = v->magnitude();

			this->vertex_list[i].point.x = v->x;
			this->vertex_list[i].point.y = v->y;
			this->vertex_list[i].point.z = v->z;
		}
		this->owner->current_vertex_list = this->vertex_list;
		this->owner->GenerateNormals();
	}
};
*/
/*
så vanlig sfærisk system består av rho, phi og theta
rho er din dist
theta er atan2(y/x) 
phi er atan2(z/(sqrt(x^2+y^2))

men pass på rangen til theta og phi
theta:0...2Pi
phi: -Pi/2..Pi/2
det er mulig du må bruke en switch for theta, for å passe på når (x,y) ikke ligger i første kvadrant
hmmm...atan2 ser ut til passe på det selv da

x = dist*cos(theta)*cos(phi) 
y = dist*sin(theta)*cos(phi) 
z = dist*sin(phi)

float theta = atan2(v->y,v->x);
float phi = atan2(v->z,sqrt(v->x*v->x+v->y*v->y));
float rho = v->magnitude();
this->vertex_list[i].point.x = cos(theta)*cos(phi)*rho;
this->vertex_list[i].point.y = sin(theta)*cos(phi)*rho;
this->vertex_list[i].point.z = sin(phi)*rho;

*/
/*
class Twirl : public MeshPlugin{
public:
	float wobble;
	float power;
	void Apply(){
		for(int i=0; i<this->owner->vertex_count; i++){
			Vector* v = &this->owner->current_vertex_list[i].point;
			float theta = (float)atan2(v->y,v->x);
			float phi = (float)atan2(v->z,sqrt(v->x*v->x+v->y*v->y));
			float rho = v->magnitude();
			this->vertex_list[i].point.x = v->x+(float)cos(theta)*(float)cos(phi)*rho;
			this->vertex_list[i].point.y = v->y+(float)sin(theta)*(float)cos(phi)*rho;
			this->vertex_list[i].point.z = v->z+(float)sin(phi)*rho;
		}
		this->owner->current_vertex_list = this->vertex_list;
		this->owner->GenerateNormals();
	}
};
*/