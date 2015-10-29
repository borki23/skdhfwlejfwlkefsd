#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>

using namespace std;

/*****************************************FILENAME DEFINITION***/
const char oname[16]="test.bmp";

/*******************************************STRUCT DEFINITION***/
struct color{
	//v[0]=red, v[1]=green, v[2]=blue
	unsigned char v[3];
	
	color(unsigned char r, unsigned char g, unsigned char b){
		v[0]=r;
		v[1]=g;
		v[2]=b;
	}
};

/********************************************SIZE DEFINITIONS***/
const unsigned hgrid = 256,//x dimension of the grid (can be anything >= 1)
			   vgrid = hgrid;//y dimension of the grid (can be anything >= 1 (hgrid and vgrid can differ); I only do this to make a square image)

/*********************************************SEED DEFINITION***/
const int seed = time(NULL);

/*****************************************FUNCTION PROTOTYPES***/
float random(int x, int y);
color lerp(color c1, color c2, float value);//LERP = Linear intERPolation

void printMap(float min, float max);//bitmap part
void printPage(time_t beginning, time_t end);//webpage part

float smoothedNoise(float x,float y);//this calls one of the below

	float linear(float x1, float x2, float a);//linear interpolation
	float cosine(float x1, float x2, float a);//cosine interpolation
	float cubic(float x1, float x2, float x3, float x4, float a);//cubic interpolation

/********************************************************MAIN***/
int main()
{
	time_t beginning = time(NULL),//these two are used to time our algorithm
		   end;
	
	float min = 0.2f,//these two specify the upper and lower bounds for the terrain
		  max = 1.6f;

	//now that we have an interesting map, create a .BMP file
	printMap(min, max);
	
	//finally, output a webpage
	end = time(NULL);//set it to now
	printPage(beginning, end);
	
	return (0);
}

/***************************************FUNCTION DECLARATIONS***/
color lerp(color c1, color c2, float value){
	color tcolor(0,0,0);
	
	for (int g=0;g<3;g++){
		if (c1.v[g]>c2.v[g])
			tcolor.v[g]=c2.v[g]+(unsigned char)(float(c1.v[g]-c2.v[g])*value);

		else
			tcolor.v[g]=c1.v[g]+(unsigned char)(float(c2.v[g]-c1.v[g])*value);
	}
	
	return (tcolor);
}

float random(int x, int y){
	int r;
    float s;

	srand(y*hgrid+x+seed);

	r=rand();

    s = (float)(r & 0x7fff)/(float)0x7fff;

    return (s);
}

float smoothedNoise(float x, float y)
{
	int int_x = (int)x,
		int_y = (int)y;
		
	float rem_x = x-(float)int_x,
		  rem_y = y-(float)int_y,
		  v1,v2,v3,v4,t1,t2;
	
	/////Just uncomment whichever smoothing algorithm you want to use/////
	
	/*
	//linear (LERP)
	v1 = random(int_x,int_y);
	v2 = random(int_x+1,int_y);
	v3 = random(int_x,int_y+1);
	v4 = random(int_x+1,int_y+1);
	

	t1 = linear(v1,v2,rem_x);
	t2 = linear(v3,v4,rem_x);
	return (linear(t1,t2,rem_y));
	
	
	//cosine
	v1 = random(int_x,int_y);
	v2 = random(int_x+1,int_y);
	v3 = random(int_x,int_y+1);
	v4 = random(int_x+1,int_y+1);
	
	t1 = cosine(v1,v2,rem_x);
	t2 = cosine(v3,v4,rem_x);
	return (cosine(t1,t2,rem_y));
	*/
	
	//cubic
	//as above, we must interpolate twice on the x-axis, then once between the two results on the y-axis
	//this is much more difficult than before because cubic interpolation requires 4 vertices each time,
	//so we really have to interpolate 4 times on the x-axis, then once between the 4 results on the y-axis
	float t3,t4;
	
	//	y-1
	v1 = random(int_x-1,int_y-1);
	v2 = random(int_x,int_y-1);
	v3 = random(int_x+1,int_y-1);
	v4 = random(int_x+2,int_y-1);
	
	t1 = cubic(v1,v2,v3,v4,rem_x);
	
	//	y
	v1 = random(int_x-1,int_y);
	v2 = random(int_x,int_y);
	v3 = random(int_x+1,int_y);
	v4 = random(int_x+2,int_y);
	
	t2 = cubic(v1,v2,v3,v4,rem_x);
	
	//	y+1
	v1 = random(int_x-1,int_y+1);
	v2 = random(int_x,int_y+1);
	v3 = random(int_x+1,int_y+1);
	v4 = random(int_x+2,int_y+1);
	
	t3 = cubic(v1,v2,v3,v4,rem_x);
	
	//	y+2
	v1 = random(int_x-1,int_y+2);
	v2 = random(int_x,int_y+2);
	v3 = random(int_x+1,int_y+2);
	v4 = random(int_x+2,int_y+2);
	
	t4 = cubic(v1,v2,v3,v4,rem_x);
	
	//now, interpolate between all these
	return(cubic(t1,t2,t3,t4,rem_y));
	
}

void printMap(float min, float max)
{
//set up some variables
	float diff = max-min,
		  flood=0.5f,//flood level
		  mount=0.85f;//mountain level
	
	flood*=diff;
	mount*=diff;
	
	int i,j,k;
	
	char c;
	
	//these can be changed for interesting results
	color landlow(0,64,0),
		  landhigh(116,182,133),
		  waterlow(55,0,0),
		  waterhigh(106,53,0),
		  mountlow(147,157,167),
		  mounthigh(226,223,216);

//3.0 output to file
	//3.1 Begin the file
	//3.1.1 open output file
	ofstream out;
	out.open(oname, ofstream::binary);
	if (!(out.is_open())){
		cout << "Target file opening error"<<endl;
		exit(0);
	}
	
	//3.1.2 output the header
		//3.1.2.1 magic number
		out.put(char(66));
		out.put(char(77));
		
		//3.1.2.2 filsize/unused space
		for (i=0;i<8;i++)
			out.put(char(0));
		
		//3.1.2.3 data offset
		out.put(char(54));
		
		//3.1.2.4 unused space
		for (i=0;i<3;i++)
			out.put(char(0));
		
		//3.1.2.5 header size
		out.put(char(40));
		
		//3.1.2.6 unused space
		for (i=0;i<3;i++)
			out.put(char(0));
		
		//3.1.2.7 file width (trickier)
		out.put(char(hgrid%256));
		out.put(char((hgrid>>8)%256));
		out.put(char((hgrid>>16)%256));
		out.put(char((hgrid>>24)%256));
		
		//3.1.2.8 file height (trickier)
		out.put(char(vgrid%256));
		out.put(char((vgrid>>8)%256));
		out.put(char((vgrid>>16)%256));
		out.put(char((vgrid>>24)%256));
		
		//3.1.2.9 color planes
		out.put(char(1));
		out.put(char(0));
		
		//3.1.2.10 bit depth
		out.put(char(24));
		
		//3.1.2.11 the rest
		for (i=0;i<25;i++)
			out.put(char(0));
	
	//set up some variables
	int	octaves = 12;//octaves (levels of detail or number of passes)
	
	float persistance=0.65f,//modifier for the amplitude
		  total,frequency,amplitude;//used for calculations
	
	//3.2 put in the elements of the array
	color newcolor(0,0,0);
	for (i=(vgrid-1);i>=0;i--){//bitmaps start with the bottom row, and work their way up...
		for (j=0;j<hgrid;j++){//...but still go left to right
			//first, get the height of this point
			total = 0.0f;
			frequency = 1.0f/(float)hgrid;
			amplitude = persistance;
			
			for (k = 0; k < octaves; ++k)
			{
				total += smoothedNoise((float)j * frequency, (float)i * frequency) * amplitude;
				
				frequency *= 2;
				amplitude *= persistance;
			}
			
			//bounds-checking
			if (total < min)
				total = 2.0f* min - total;
			else if (total > max)
				total = 2.0f* max - total;
			
			total -= min;
			
			//if this point is below the floodline...
			if (total<flood)
				newcolor=lerp(waterlow,waterhigh,total/flood);
			
			//if this is above the mountain line...
			else if (total>mount)
				newcolor=lerp(mountlow,mounthigh,(total-mount)/(diff-mount));
			
			//if this is regular land
			else
				newcolor=lerp(landlow,landhigh,(total-flood)/(mount-flood));
			
			out.put(char(newcolor.v[0]));//blue
			out.put(char(newcolor.v[1]));//green
			out.put(char(newcolor.v[2]));//red
		}
		//round off the row
		for (k=0;k<(hgrid%4);k++)
			out.put(char(0));
	}
	
	//3.3 end the file
	out.close();
}

void printPage(time_t beginning, time_t end)
{
	double timeTaken = end-beginning;
	cout<<"Content-Type: text/html\n\n"
		<<"<html><head><title>FTG Page</title></head>\n"
		<<"<body>\n"
		<<"<h2>Fractal Terrain Generator Page</h2>\n"
		<<"<img src=\"test.bmp\" /><br />\n"
		<<"This took " << timeTaken << " seconds to create.<br />\n"
		<<"The seed was " << seed << "<br />\n"
		<<"</body>\n"
		<<"</html>\n";
}

float linear(float x1, float x2,float a)
{
	return (x1 * (1 - a) + x2 * a);
}

float cosine(float x1, float x2, float a)
{
	//not 100% sure how this one works
	float temp;
	temp = (1.0f-cos(a*3.1415927f))/2.0f;
	
	return (x1*(1.0f-temp)+x2*temp);
}

float cubic(float x1, float x2, float x3, float x4, float a)
{
	//I honestly have no idea how this works
	float c1,c2,c3,c4;
	c1=x4-x3-x1+x2;
	c2 = x1-x2-c1;
	c3 = x3-x1;
	c4 = x2;
	
	return(c1*a*a*a+c2*a*a+c3*a+c4);
}