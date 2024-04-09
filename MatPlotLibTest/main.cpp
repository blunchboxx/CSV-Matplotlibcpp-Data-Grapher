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

void readFile(int sampleNum, int yCol, int xCol, string path, vector<TestSample>& testObjectList)
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

		getline(csvFile, newLine);  // Get and discard header row

		int count = 0;

		while (getline(csvFile, newLine))
		{
			count = 0;

			istringstream ss(newLine); // Send next line to stream

			while (getline(ss, token, ','))  // Loop through entire stream
			{

				if (count == yCol) // Load (force) data
				{
					yValues.push_back(stof(token));
				}

				else if (count == xCol)
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
		plt::title(fileName); 
		plt::legend(); // { {"label", "log(x)"} }
		plt::xlabel("Displacement (mm)");
		plt::ylabel("Force (N)");
		//plt::xlim(0, 10 * 2); // Set x-axis range
		plt::grid(true);

		plt::save(tempPath);
		tempPath.clear();

		cout << "Graph " << nextSample._sampleNumber << " saved to folder." << endl;
	}

	cout << endl;
}

void getFileNames(string path, vector<string>& nameList)
{
	for (const auto& file : fs::directory_iterator(path)) {
		
		string fileName = file.path().generic_string();
		fs::path filePath = fileName;

		cout << "Filename : " << fileName << endl;

		if (filePath.extension() == ".csv")
			nameList.push_back(fileName);
		else
			cout << fileName << " is invalid file type. File not added to queue. Use .csv type files only." << endl;
	}
}

void readInput(string& inPath, string& outPath, int& xCol, int& yCol)
{
	cout << "Enter name of folder storing CSV files (type QUIT to exit): ";

	cin >> inPath;
	cout << endl;

	if ((inPath != "QUIT") and (inPath != "quit") and (inPath != "Quit") and (inPath != "0"))
	{
		cout << "Enter name of folder to save graphs in: ";
		cin >> outPath;
		cout << endl;

		cout << "Enter column number for x axis values (first column = 0): ";
		cin >> xCol;
		cout << endl;
		cout << "Enter column number for y axis values (first column = 0): ";
		cin >> yCol;
		cout << endl;
	}


}

int main()
{
	vector<string> fileList;
	vector<TestSample> testSampleVector;
	string filePath;
	string outputFilePath;
	int _numCols = 0;
	int _yCol = 0;
	int _xCol = 0;

	cout << "Welcome to the CSV Data Grapher!" << endl;

	readInput(filePath, outputFilePath, _xCol, _yCol);

	while ((filePath != "QUIT") and (filePath != "quit") and (filePath != "Quit") and (filePath != "0"))
	{

		getFileNames(filePath, fileList);

		for (int i = 0; i < fileList.size(); i++)
		{
			readFile(i + 1, _yCol, _xCol, fileList[i], testSampleVector);
		}

		graphObject(outputFilePath, testSampleVector);

		// Clear variables for next iteration
		fileList.clear();
		testSampleVector.clear();
		filePath.clear();
		outputFilePath.clear();

		readInput(filePath, outputFilePath, _xCol, _yCol);

		if ((filePath == "QUIT") or (filePath == "quit") or (filePath == "Quit") or (filePath == "0"))
			return 0;

	}

	return 0;
}

