/*	**** IMPORTANT NOTICE ****
*
*	Copyright 2015,
*	David Bennehag and Yanuar Tri Aditya Nugraha
*		This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	**************************
*/

/*Kalman stuff from mherb*/
#include "SystemModel.hpp"
//#include "OrientationMeasurementModel.hpp"
//#include "PositionMeasurementModel.hpp"

#include <ExtendedKalmanFilter.hpp>
#include <UnscentedKalmanFilter.hpp>

/* Regular stuff*/
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>

/* Our headers */
#include "Model.h" 				
#include "Sensor.h" 			
#include "Gaussian.h"


// TO COMPILE WITH EIGEN
//g++ -I ~/Dropbox/Projects/C++/Vehicle-State-Estimator/eigen App.cpp Model.cpp Sensor.cpp Gaussian.cpp -o app -std=gnu++11

using namespace std;
using namespace Eigen;

int main()
{
	Model model;
	Vector3d state;
	Gaussian gaussian;
	Sensor sensor;

	sensor.openFile("velocity.txt", "omega.txt");

	ifstream xFile("x.txt");
	ifstream yFile("y.txt");

	ofstream resultFile("result.txt");

	string xLine, yLine;
	double x = 0, 
		   y = 0, 
		   xTotal = 0, 
		   yTotal = 0;
	int linesRead = 0;
	while (sensor.readFile() == 1)
	{
		getline(xFile, xLine);
		getline(yFile, yLine);

		model.setSpeed(sensor.getVelocity());
		model.setOmega(sensor.getOmega());

		//PREDICT NEXT STATE USING MOTION MODEL
		state = model.updateState();

		//cout << endl << "Computed: X = " << state(0) << "\t, Y = " << state(1) << endl
		//			   << "Measured: X = " << x	       << "\t, Y = " << y		 << endl << endl;

		//Decrease the number of data points, graphs are too dense.
		if(linesRead % 5 == 0)
			resultFile << state(0) << "\t" << state(1) << "\t" << xLine << "\t" << yLine << "\n";

		linesRead++;
		x = strtod(xLine.c_str(), NULL);
		y = strtod(yLine.c_str(), NULL);

		xTotal += pow((state(0) - x), 2);
		yTotal += pow((state(1) - y), 2);
	}
	cout << "X error: " << xTotal / linesRead << endl;
	cout << "Y error: " << yTotal / linesRead << endl;

	sensor.closeFile();
	cout << "Nr of lines read: " << linesRead << endl;
	xFile.close();
	yFile.close();

	//sensor.openFile("gps-2column.txt");
	//pair<float, float> position = sensor.readFile(lineNumber++);
	//for (int i = 1; i <= 100; i++)
	{
		//pair<float, float> position = sensor.readFile();

		//cout << "X = " << position.first << ",\t Y = " << position.second << endl;

		//cout << i << ", " << Gaussian::gaussianRandNum(70, 10) << endl;
	}



	return 0;
}
