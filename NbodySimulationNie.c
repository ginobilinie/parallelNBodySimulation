#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <iostream>
#include <fstream> 
using namespace std;
#define G 6.673e-11
#define PI 3.1416

const int k=4;
const double dt=0.001;
const double radius=1.0;

int n = 5000,p = 8;
int i,j;

//Define a tuple, and define overridden operator 
struct Tuple
{
public:
    double x,y;
    Tuple(){x = 0; y = 0;}
    Tuple(double x, double y)
	{
        this->x = x;
        this->y = y;
    }

    Tuple operator+(Tuple t)
	{
        return Tuple(x+t.x,y+t.y);
    }
    Tuple operator-(Tuple t)
	{
        return Tuple(x-t.x,y-t.y);
    }
    Tuple operator*(double t)
	{
        return Tuple(x*t,y*t);
    }
    Tuple operator/(double t)
	{
        return Tuple(x/t,y/t);
    }
 
    double norm()
	{
		return x*x + y*y;
	}
  }* forces = NULL;


struct Body
{
public:
    Tuple location;
    Tuple velocity;
    double mass;

    Body(){}
    Body(Tuple location,Tuple velocity, double mass)
	{
        this->location = location;
        this->velocity = velocity;
        this->mass = mass;
    }
};

void updateAll(Body* pBody, int n);
void updateAllNewton3(Body* pBody, int n);
double computeMomentum(Body* pBody, int n);

int main(int argc, char **argv) 
{

    double tStart,tEnd;
    if (argc>1)
	{
		n = atoi(argv[1]);
		p = atoi(argv[2]);
    }else
	{
		printf("please specify the parameters: numberOfBodies, numberOfThreads\n");
	}
     Body* pBody = new Body[n];
     forces = new Tuple[n];
	/************************algorithm without newton law****************************************/
    omp_set_dynamic(0);
    omp_set_num_threads(p);
    #pragma omp parallel private(i)
    { 
		#pragma omp for
		for (int i = 0; i < n; i++)
		{
				double px = radius * cos(i * PI / n);
				double py = radius * sin(i * PI / n);
				double mass = rand()/(double)RAND_MAX;
				double vx = rand()/(double)RAND_MAX;
				double vy = rand()/(double)RAND_MAX;

				pBody[i].location = Tuple(px,py);
				pBody[i].velocity = Tuple(vx,vy);
				pBody[i].mass = mass;
		}

		#pragma omp master
		tStart = omp_get_wtime();

		for (int iter = 0; iter < k; iter++)
					updateAll(pBody,n);

	}
    tEnd = omp_get_wtime();
    double seconds = tEnd - tStart;
    double interaction=(double)k*n*n/seconds;
    ofstream ofile;
    ofile.open("records.txt",std::ofstream::out | std::ofstream::app);
    ofile<<n<<' '<<p<<' '<<seconds<<' '<<interaction<<endl;
    ofile.close();
	
	/************************algorithm with newton law****************************************/
	 // Body* pBody = new Body[n];
     //forces = new Tuple[n];
    omp_set_dynamic(0);
    omp_set_num_threads(p);
    #pragma omp parallel private(i)
    { 
		#pragma omp for
		for (int i = 0; i < n; i++)
		{
				double px = radius * cos(i * PI / n);
				double py = radius * sin(i * PI / n);
				double mass = rand()/(double)RAND_MAX;
				double vx = rand()/(double)RAND_MAX;
				double vy = rand()/(double)RAND_MAX;

				pBody[i].location = Tuple(px,py);
				pBody[i].velocity = Tuple(vx,vy);
				pBody[i].mass = mass;
		}

		#pragma omp master
		tStart = omp_get_wtime();

		for (int iter = 0; iter < k; iter++)
			updateAllNewton3(pBody,n);

	}
    tEnd = omp_get_wtime();
    seconds = tEnd - tStart;
    interaction=(double)k*n*n/seconds;
    ofstream ofile3;
    ofile3.open("records3.txt",std::ofstream::out | std::ofstream::app);
    ofile3<<n<<' '<<p<<' '<<seconds<<' '<<interaction<<endl;
    ofile3.close();
	
	delete forces;
	delete pBody;
    return 0;
}


//not using newton 3rd law
void updateAll(Body* pBody, int n){

    #pragma omp for
    for (int i = 0; i < n; i++) {
        Tuple allForces(0,0);

           for (int j = 0; j < n; j++) {
            if (i == j) continue;
			Tuple vec = pBody[j].location - pBody[i].location;
			
			double dcubic = vec.norm();
			dcubic=dcubic*sqrt(dcubic);
			double multiplier = G * pBody[i].mass * pBody[j].mass / dcubic;
			Tuple force =  vec * multiplier;
			allForces = allForces + force;         
        }

		pBody[i].velocity = pBody[i].velocity + allForces / pBody[i].mass * dt;
        pBody[i].location = pBody[i].location + pBody[i].velocity * dt;
        }
}

//using newton 3rd law
void updateAllNewton3(Body* pBody, int n){

    #pragma omp for
    for (int i = 0; i < n; i++){
        forces[i].x = 0;
        forces[i].y = 0;
    }

    #pragma omp for
    for (int i = 0; i < n-1; i++) {
      for (int j = i + 1; j < n; j++) {
            Tuple vec = pBody[j].location - pBody[i].location;
            double dcubic = vec.norm();
        	dcubic=dcubic*sqrt(dcubic);
            double multiplier = G * pBody[i].mass * pBody[j].mass / dcubic;
            Tuple force =  vec * multiplier;
            forces[i] = forces[i] + force;
            forces[j] = forces[j] - force;
        }

		pBody[i].velocity = pBody[i].velocity + forces[i] / pBody[i].mass * dt;
        pBody[i].location = pBody[i].location + pBody[i].velocity * dt;
        }
}
double computeMomentum(Body* pBody, int n)
{
    double momentumx = 0, momentumy = 0;
	#pragma omp for
    for (int i = 0; i < n; i++)
    {
        momentumx += pBody[i].mass * pBody[i].velocity.x;
        momentumy += pBody[i].mass * pBody[i].velocity.y;
    }
    return sqrt(momentumx * momentumx + momentumy * momentumy);
}
