/*
 * Collision Simulation Lab
 * Victor Guo, Fardin Islam, Rong Tao Luo, Andrew Yuan
 * 10/15/2018
 */

#include<bits/stdc++.h>
#define PI 3.14159265358
using namespace std;
const double maxn = 1000000.0;

struct ball{
	double posX,posY,velX,velY; //position and velocity
	double radius; //radius of ball
	double mass; //mass of ball
	double angle; //angle of change in velocity with respect to x axis
	double accel; //acceleration of ball
	double accelInit; //acceleration of ball at initial instant of time interval
	double k; //elasticity / spring constant of ball
	
	//method to find distance between two balls
	double findDistance(ball b1){
		return sqrt(pow(posX - b1.posX,2) + pow(posY - b1.posY, 2));//return distance between centers (distance formula)
	}
};

struct sim{
	double delta; //delta represents small change in time
	double simTime; // simTime represents total time for simulation
	double time; //timer to keep track of simulation run time
	
	//constructor for sim class
	sim(double t) : time(0) {
		simTime = t; //initialize simulation run time
		delta = simTime/(1<<25); // Divides into about 33 million time intervals
	}
	
	//method to run the simulation
	void startSim(ball b1, ball b2){
		while(time < simTime){//continue loop until simulation time finishes
			if(b1.findDistance(b2) < b1.radius+b2.radius){ //if the balls are in contact with one another
				b2.angle = atan((b2.posY - b1.posY)/(b2.posX - b1.posX));//compute angle of deflection of ball #2
				b1.angle = b2.angle + PI;//compute angle of deflection of ball #1
					
				//compute acceleration for ball #2
				b2.accel = b2.accelInit + b2.k*(b1.radius+b2.radius - b1.findDistance(b2))/(2*b2.mass);//sum of initial and final accel. in time interval
				b2.accel /= 2;//evaluate the average acceleration (acceleration assumed to grow linearly)
				b2.accelInit = b2.k*(b1.radius+b2.radius - b1.findDistance(b2))/(2*b2.mass);//set equal to acceleration at final instant of time
				
				//compute acceleration for ball #1
				b1.accel = b1.accelInit + b1.k*(b1.radius+b2.radius - b1.findDistance(b2))/(2*b1.mass);//sum of initial and final accel. in time interval
				b1.accel /= 2;//evaluate the average acceleration (acceleration assumed to grow linearly)
				b1.accelInit = b1.k*(b1.radius+b2.radius - b1.findDistance(b2))/(2*b1.mass);//set equal to acceleration at final instant of time
				
				//compute position and velocity for ball #2
				//evaluate new position based on constant acceleration
				b2.posX += b2.velX*delta + 1/2 * b2.accel * delta* delta * cos(b2.angle);
				b2.posY += b2.velY*delta + 1/2 * b2.accel * delta* delta * sin(b2.angle);
				//evaluate new velocity based on constant acceleration
				b2.velX += b2.accel * cos(b2.angle) * delta;
				b2.velY += b2.accel * sin(b2.angle) * delta;
				
				//compute position and velocity for ball #1
				//evaluate new position based on constant acceleration
				b1.posX += b1.velX*delta + 1/2 * b1.accel * delta* delta * cos(b1.angle);
				b1.posY += b1.velY*delta + 1/2 * b1.accel * delta* delta * sin(b1.angle);
				//evaluate new velocity based on constant acceleration
				b1.velX += b1.accel * cos(b1.angle) * delta;
				b1.velY += b1.accel * sin(b1.angle) * delta;
			}
			else{
				//Two balls not in contact so no force or acceleration
				//Position is incremented based on constant velocity
				
				//Evaluate position of ball #1
				b1.posX += b1.velX * delta;
				b1.posY += b1.velY * delta;
				
				//Evaluate position of ball #2
				b2.posX += b2.velX * delta;
				b2.posY += b2.velY * delta;
			}
			time += delta;//increment timer
		}
		//output final data
		printf("Final Position and Velocities at %f seconds:\n", simTime);
		printf("Velocity of Ball #1 (m/s): (%f, %f)\n", b1.velX, b1.velY);
		printf("Velocity of Ball #2 (m/s): (%f, %f)\n", b2.velX, b2.velY);
		printf("Position of Ball #1 (m): (%f, %f)\n", b1.posX, b1.posY);
		printf("Position of Ball #2 (m): (%f, %f)", b2.posX, b2.posY);
	}
};

//method to receive input
void input(ball &b1){
	//receive basic input
	cout<<"Mass (kg): ";
	cin >> b1.mass;
	cout<<"Radius (m): ";
	cin >> b1.radius;
	cout<<"Spring Constant (N/m) (input -1 if completely elastic): ";
	cin >> b1.k;
	cout<<"Initial velocity (m/s) (x-direction): ";
	cin >> b1.velX;
	cout<<"Initial velocity (m/s) (y-direction): ";
	cin >> b1.velY;
	cout<<"Initial position (m) (x-coordinate): ";
	cin >> b1.posX;
	cout<<"Initial position (m) (y-coordinate): ";
	cin >> b1.posY;
	
	//set initial and average acceleration to be 0
	b1.accel = 0;
	b1.accelInit = 0;
	if(b1.k == -1)//if user says the ball is elastic
		b1.k = maxn;//set elasticity to be very high
	cout<<"\n";
}

int main(){
	double t;
	ball b1,b2;//initialize two ball objects
	printf("This is a two ball Collision Simulation Program\n\n");//prompt user
	printf("Enter the properties of Ball #1\n");
	input(b1);//receive input for initial ball
	printf("Enter the properties of Ball #2\n");
	input(b2);//receive input for target ball
	
	//find simulation run time
	cout<<"How long would you like to run the simulation (s)? \n";
	cin >> t;
	cout<<"\n";
	
	sim s = sim(t);//initialize simulation
	s.startSim(b1,b2);//run simulation
	
	return 0;
}
