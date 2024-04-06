#pragma once
#include <vector>
#include <map>
using namespace std;


struct TestSample
{
	int _sampleNumber;
	vector<float> _xDataValues;
	vector<float> _yDataValues;
	map<float, float> _rawDataMap;
	//map<float, float> _rawStiffnessData;
	//map<float, float> _rawTorqueData;

	//Constructor
	TestSample(int sampleNum, map<float, float>& dataValues, vector<float>& xVals, vector<float>& yVals);
};