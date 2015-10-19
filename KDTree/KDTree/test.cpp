#include "KDTree.h"

#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include <vector>

const int POINTS_NUMBER = 10000;
const int REQUESTS_NUMBER = 10000;
const int DIMENSION = 10;

const double MIN_COORDINATE_VALUE = -100.0;
const double MAX_COORDINATE_VALUE = 100.0;

std::default_random_engine engine(time(NULL));
std::uniform_real_distribution<> randomGenerator(MIN_COORDINATE_VALUE, MAX_COORDINATE_VALUE);

void genPoints(std::vector<TypePoint>* points, int pointsNumber) {
	(*points).resize(pointsNumber);

	for (int currentPointNumber = 0; currentPointNumber < pointsNumber; ++currentPointNumber) {
		(*points)[currentPointNumber].changeDimension(DIMENSION);
		(*points)[currentPointNumber].identifier = currentPointNumber;

		for (int currentCoordinate = 0; currentCoordinate < DIMENSION; ++currentCoordinate) {
			(*points)[currentPointNumber].coordinates[currentCoordinate] = randomGenerator(engine);
		}
	}
}

double simpleAlgoritm(const std::vector<TypePoint> &points, const TypePoint &requestPoint) {
	double distance = distanceBetweenPoints(points[0], requestPoint);

	for (int currentPointNumber = 1; currentPointNumber < POINTS_NUMBER; ++currentPointNumber) {
		double newDistance = distanceBetweenPoints(points[currentPointNumber], requestPoint);
		if (newDistance + EPS < distance) {
			distance = newDistance;
		}
	}

	return sqrt(distance);
}

bool checkDistances(double firstDistance, double secondDistance) {
	if ((firstDistance < secondDistance + EPS) && (firstDistance + EPS > secondDistance)) {
		return true;
	} else {
		return false;
	}
}

void processRequests(const KDTree& tree, const std::vector<TypePoint> &points, const std::vector<TypePoint> &requestPoints) {
	std::vector<double> KDTReeResultDistances(REQUESTS_NUMBER);
	bool resultsCorrect = true;

	for (size_t currentRequestNumber = 0; currentRequestNumber < requestPoints.size(); ++currentRequestNumber) {
		double curKDTReeResultDistance = 0;
		tree.getMinDistanceIdentifier(requestPoints[currentRequestNumber], curKDTReeResultDistance);
		KDTReeResultDistances[currentRequestNumber] = curKDTReeResultDistance;
	}

	for (size_t currentRequestNumber = 0; currentRequestNumber < requestPoints.size(); ++currentRequestNumber) {
		double simpleAlgoritmResult = simpleAlgoritm(points, requestPoints[currentRequestNumber]);

		if (!checkDistances(simpleAlgoritmResult, KDTReeResultDistances[currentRequestNumber])) {
			std::cout << std::setprecision(15) << simpleAlgoritmResult << ' ';
			std::cout << std::setprecision(15) << KDTReeResultDistances[currentRequestNumber] << std::endl;
			resultsCorrect = false;
		}
	}


	if (resultsCorrect) {
		std::cout << "results are correct" << std::endl;
	} else {
		std::cout << "results are incorrect" << std::endl;
	}
}

int main() {
	std::vector<TypePoint> points;
	genPoints(&points, POINTS_NUMBER);

	KDTree tree(points);

	std::vector<TypePoint> requestPoints;
	genPoints(&requestPoints, REQUESTS_NUMBER);

	processRequests(tree, points, requestPoints);

	return 0;
}