#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>


#include "TestSample.h"
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;
namespace fs = std::filesystem;

void readFile(int sampleNum, string path, vector<TestSample>& testObjectList)
{
	ifstream csvFile;
	vector<float> xValues;
	vector<float> yValues;
	map<float, float> dataValuesMap;

	csvFile.open(path);

	if (!csvFile.is_open())
		cout << "File could not be opened to read." << endl;
	else
	{
		cout << "Reading data from file " << path << "..." << endl;
		string newLine, token;
		string tempArray[17];

		getline(csvFile, newLine);  // Get and discard header row

		int count = 0;

		while (getline(csvFile, newLine))
		{
			count = 0;

			istringstream ss(newLine); // Send next line to stream

			while (getline(ss, token, ','))  // Loop through entire stream
			{
				/*if (count < 4)
				{
					cout << " Count: " << count << " token: " << token << endl;
				}*/

				if (count == 4) // Load (force) data
				{
					yValues.push_back(stof(token));
				}

				/*else if (count > 4 && count < 8)
				{
					continue;
				}*/

				else if (count == 8)
				{
					xValues.push_back(stof(token));
				}

				count++;
			}

			for (int i = 0; i < xValues.size(); i++)
			{
				dataValuesMap.emplace(xValues[i], yValues[i]);
			}

		}

		TestSample nextSample(sampleNum, dataValuesMap, xValues, yValues);// // Instantiate new Test Sample object

		testObjectList.push_back(nextSample); // Add object to vector

	}
}

void graphObject(string path, vector<TestSample>& testObjectList)
{
	for (TestSample nextSample : testObjectList) {
		
		cout << "Sample Number: " << nextSample._sampleNumber << endl;

		string tempPath = path;
		string fileName = "Test Sample ";
		fileName.append(to_string(nextSample._sampleNumber));

		tempPath.append("/");
		tempPath.append(fileName);

		// Set the size of output image to 1200x780 pixels
		plt::figure();
		plt::figure_size(1200, 780);

		// Plot line from given x and y data. Color is selected automatically.
		plt::plot(nextSample._xDataValues, nextSample._yDataValues, { { "label", "tensile force" } });  // add the label f(x)
		plt::title(fileName); //, { {"label", "log(x)"} }
		plt::legend();
		//plt::subplot(1, 1, 1);
		plt::xlabel("Displacement (mm)");
		plt::ylabel("Force (N)");
		//plt::xlim(0, 10 * 2); // Set x-axis range
		plt::grid(true);

		plt::save(tempPath);
		tempPath.clear();

		cout << "Graph " << nextSample._sampleNumber << " saved to folder." << endl;
	}
}

void getFileNames(string path, vector<string>& nameList)
{
	for (const auto& file : fs::directory_iterator(path)) {
		//cout << file << endl;
		//cout << typeid(file).name() << endl;
		//string fileName = file.path().filename().generic_string();
		string fileName = file.path().generic_string();

		cout << "Filename : " << fileName << endl;

		nameList.push_back(fileName);
	}
}

int main()
{
	vector<string> tensileFileList;
	//vector<string> torqueFileList;
	//vector<string> stiffnessFileList;
	vector<TestSample> testSampleVector;

	string tensilePath = "tensile";
	//string torquePath = "torque";
	//string stiffnessPath = "stiffness";

	getFileNames(tensilePath, tensileFileList);
	//getFileNames(torquePath, torqueFileList);
	//getFileNames(stiffnessPath, stiffnessFileList);

	//cout << "Number of Files: " << tensileFileList.size() << endl;

	for (int i = 0; i < tensileFileList.size(); i++)
	{
		readFile(i + 1, tensileFileList[i], testSampleVector);
	}

	graphObject("outputs", testSampleVector);

	/*
	for (TestSample nextSample : testSampleVector) {
		cout << "Sample Number: " << nextSample._sampleNumber << endl;
		cout << "Value x: " << nextSample._xDataValues[7] << " Value y: " << nextSample._yDataValues[7] << endl;

		string fileName = "outputs/Test Sample ";
		fileName.append(to_string(nextSample._sampleNumber));
		// Set the size of output image to 1200x780 pixels
		plt::figure_size(1200, 780);
		// Plot line from given x and y data. Color is selected automatically.
		plt::plot(nextSample._xDataValues, nextSample._yDataValues);
		plt::save(fileName);
		cout << "Graph " << nextSample._sampleNumber << " saved to folder." << endl;
	}
	*/

	return 0;
}

