#include "TestSample.h"

TestSample::TestSample(int sampleNum, map<float, float>& dataValues, vector<float>& xVals, vector<float>& yVals)
{
	_sampleNumber = sampleNum;
	_xDataValues = xVals;
	_yDataValues = yVals;
	_rawDataMap = dataValues;
}