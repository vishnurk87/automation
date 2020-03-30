// code for following a predefined path : author vishnu

#include <iostream>
#include<fstream>
#include "image_transfer5.h"
#include <conio.h>
#include<cmath>
#include "timer.h"
#include "vision.h"
#include <windows.h>
#include "serial_com.h"

using namespace std;

#define KEY(a) (GetAsyncKeyState((int)(a)) & (SHORT)0x8000)
#define PI 3.141592654

int main(){

	//declarations and initialisations
	int i, speed  = 1, len; // speed  = baud rate 1 = 115200
	const int n = 1000; // number of x/y values
	double posx[n], posy[n], dx[n], dy[n], th2, th1, theta[n], v, u, ms1, ms2, t0, t1, dt, d;
	double wheel = 21.7;

	char *p, *p_ar; // pointer to shared memory and buffer of arduino
	float *q, *q_ar;// pointer to an array of float



	//for serial communication
	HANDLE h1;
	const int NMAX = 64;
	char buffer[NMAX]; // buffer to send
	char COM_port[10] = "COM4";
	Sleep(100);

	//opening serial for communication
	open_serial(COM_port, h1, speed);
	cout << "\n press C to continue or X to terminate program";
	while (!KEY('C')) Sleep(1);



	//opening file to read
	ifstream fin;
	fin.open("outputx.txt");
	
	i = 0;
	for (i = 0; i < n; i++){
			fin >> posx[i];
		}
	fin.close();
	fin.open("outputy.txt");
	for (i = 0; i < n; i++){
		fin >> posy[i];
	}
	fin.close();
/*
	for (i = 0; i < n; i++){
		cout << posx[i] << "\t" << posy[i] << "\n";
	}*/
	th1 = 0;
	i = 0;
	while (i < n-1){
		dx[i] = posx[i + 1] - posx[i];
		dy[i] = posy[i + 1] - posy[i];
		th2 = atan(dx[i] / dy[i]);
		theta[i] = th2 - th1;
		//cout << theta[i] << "\n";
		i++;
		th1 = th2;   // th1 to previous theta
	}
	cout << "\nread files and updated arrays";


	u = 10; // initial velocity
	p_ar = buffer;
	q_ar = (float *)p_ar;
	// convert the character pointer to an float pointer	
	
	dt = 0;  // initailize dt as zero

	cout << "\nsending to ardunio";
	t0 = high_resolution_time();
	i = 0; //counter to take dx dy and theta
	len = 8;  // length of array to send
	t1 = high_resolution_time();
	float max = 0, min = 5, avg;
	while (i<n) {
		if (KEY(VK_UP)){
			u++;
		}
		if (KEY(VK_DOWN)){
			u--;
		}

		if (t1 >= t0 + dt) {
			d = sqrt(pow(dx[i], 2.0) + pow(dy[i], 2.0)); // distance by pythagoras theorem
			//cout << " dx: " << dx[i] << "  dy: " << dy[i] << " d " << d <<"\n";
			ms1 = 1500 + 4.717*u;
			dt = d / u;
			//cout << "\ndt " << dt << "\n";
			v = (d + (wheel*theta[i])) / dt;
			//cout << " u " << u << "v " << v << "\tthet" << theta[i] << "wheel*theta[i]" << wheel*theta[i] << "\n";
			ms2 = 1500 - 4.717*v;
			q_ar[0] = (float)ms1;
			q_ar[1] = (float)ms2;
			if (i == n - 2){
				q_ar[0] = 1500.00;
				q_ar[1] = 1500.00;
				break;
			}
			//cout << "qar0: " << q_ar[0] << "\t" << "qar1: " << q_ar[1] <<"\n";
			serial_send(buffer, len, h1);
			Sleep(10);
			i++;
			t0 = high_resolution_time();
			while (serial_available(h1) > 0) {
				serial_recv(buffer, 1, h1);
				//cout << (float)buffer[0];
			}

			
			if (dt < min) min = dt;
			if (dt > max) max = dt;
			// suspend program B for 10ms allowing other
			// programs to operate
//			_getch();

		}

		t1 = high_resolution_time();
		//cout <<"t1 - t0" <<  t1 - t0 << "\n";
		if (KEY('X')) break;
	}
	avg = (max - min) / 2.0;
	cout << "max " << max << "      min " << min << "      avg " << avg << "\n";
	close_serial(h1);

	Sleep(1000);

	cout << "\ndone.\n";
	_getch();
	
	return 0;


}