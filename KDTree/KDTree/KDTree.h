#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

const double EPS = 1E-7;

struct TypePoint {
	std::vector<double> coordinates;
	int identifier;

	TypePoint() {
		//do nothing
	}

	TypePoint(int dimension, int pointIdentifier) :
		identifier(pointIdentifier)	{

		changeDimension(dimension);
	}

	void changeDimension(int newDimension) {
		coordinates.resize(newDimension, 0);
	}

	int getDimension() const {
		return coordinates.size();
	}
};

double distanceBetweenPoints(const TypePoint &firstPoint, const TypePoint &secondPoint) {
	double result = 0;

	for (int currentCoordiateNumber = 0; currentCoordiateNumber < firstPoint.getDimension(); ++currentCoordiateNumber) {
		double coordinatesDifference = (firstPoint.coordinates[currentCoordiateNumber] - secondPoint.coordinates[currentCoordiateNumber]);
		result += coordinatesDifference * coordinatesDifference;
	}

	return result;
}

class KDTree {
private:
	struct KDTreeNode {
		TypePoint leftPoint;
		TypePoint rightPoint;
		TypePoint middlePoint;

		KDTreeNode* leftChild;
		KDTreeNode* rightChild;

		std::vector<TypePoint> points;

		KDTreeNode(const TypePoint &newLeftPoint, const TypePoint &newRightPoint) :
			leftChild(NULL),
			rightChild(NULL),
			leftPoint(newLeftPoint),
			rightPoint(newRightPoint) {

			middlePoint.changeDimension(newLeftPoint.coordinates.size());
			for (size_t currentCoordinate = 0; currentCoordinate < middlePoint.coordinates.size(); ++currentCoordinate) {
				middlePoint.coordinates[currentCoordinate] = (leftPoint.coordinates[currentCoordinate] + rightPoint.coordinates[currentCoordinate]) / 2;
			}
		}
	};

	KDTreeNode* root_;

	void getBorderPoint(const std::vector<TypePoint> &points, bool lowerPoint, TypePoint* answer) const {
		(*answer) = points[0];
		for (size_t currentPointNumber = 1; currentPointNumber < points.size(); ++currentPointNumber) {
			for (int currentCoordinate = 0; currentCoordinate < (*answer).getDimension(); ++currentCoordinate) {
				if (points[currentPointNumber].coordinates[currentCoordinate] > (*answer).coordinates[currentCoordinate]) {
					if (!lowerPoint) {
						(*answer).coordinates[currentCoordinate] = points[currentPointNumber].coordinates[currentCoordinate];
					}
				}
				else {
					if (lowerPoint) {
						(*answer).coordinates[currentCoordinate] = points[currentPointNumber].coordinates[currentCoordinate];
					}
				}
			}
		}
	}

	int getMaxDimension(const TypePoint &leftPoint, const TypePoint &rightPoint) const {
		int dimensionIndex = 0;
		for (int currentCoordinate = 1; currentCoordinate < leftPoint.getDimension(); ++currentCoordinate) {
			if (rightPoint.coordinates[dimensionIndex] - leftPoint.coordinates[dimensionIndex] 
				< rightPoint.coordinates[currentCoordinate] - leftPoint.coordinates[currentCoordinate]) {

				dimensionIndex = currentCoordinate;
			}
		}
		return dimensionIndex;
	}

	void devidePoints(const std::vector<TypePoint> &points, std::vector<TypePoint> &leftPoints, std::vector<TypePoint> &rightPoints, const TypePoint &leftPoint, const TypePoint &rightPoint) const {
		int maxDimensionIndex = getMaxDimension(leftPoint, rightPoint);
		std::vector<std::pair<double, int> > sortedPoints(points.size());

		for (size_t currentPointNumber = 0; currentPointNumber < points.size(); ++currentPointNumber) {
			sortedPoints[currentPointNumber].second = currentPointNumber;
			sortedPoints[currentPointNumber].first = points[currentPointNumber].coordinates[maxDimensionIndex];
		}

		sort(sortedPoints.begin(), sortedPoints.end());

		rightPoints.resize(points.size() - points.size() / 2);
		for (size_t currentPoint = points.size() / 2; currentPoint < points.size(); ++currentPoint) {
			rightPoints[currentPoint - points.size() / 2] = points[sortedPoints[currentPoint].second];
		}

		leftPoints.resize(points.size() / 2);
		for (size_t currentPoint = 0; currentPoint < points.size() / 2; ++currentPoint) {
			leftPoints[currentPoint] = points[sortedPoints[currentPoint].second];
		}
	}

	KDTreeNode * buildTree(const std::vector<TypePoint> &points) {
		TypePoint leftPoint;
		TypePoint rightPoint;
		getBorderPoint(points, true, &leftPoint);
		getBorderPoint(points, false, &rightPoint);

		KDTreeNode* newNode = new KDTreeNode(leftPoint, rightPoint);

		if (points.size() <= 2) {
			newNode->points = points;
			return newNode;
		}

		std::vector<TypePoint> leftPoints, rightPoints;
		devidePoints(points, leftPoints, rightPoints, leftPoint, rightPoint);
		//std::cout << leftPoints.size() << ' ' << rightPoints.size() << std::endl;

		newNode->leftChild = buildTree(leftPoints);
		newNode->rightChild = buildTree(rightPoints);

		return newNode;
	}

	bool checkSubtree(const TypePoint &leftPoint, const TypePoint &rightPoint, const TypePoint &point, double distance) const {
		double curDistance = sqrt(distance);
		for (int currentCoordinate = 0; currentCoordinate < leftPoint.getDimension(); ++currentCoordinate) {
			if (!((leftPoint.coordinates[currentCoordinate] - curDistance <= point.coordinates[currentCoordinate]) 
				&& (rightPoint.coordinates[currentCoordinate] + curDistance >= point.coordinates[currentCoordinate]))) {

				return false;
			}
		}

		return true;
	}
	
	void getMinDistance(KDTreeNode* currentNode, const TypePoint &point, double &distance, int &identifier) const {
		if ((currentNode->leftChild == NULL) && (currentNode->rightChild == NULL)) {
			for (size_t currentPointNumber = 0; currentPointNumber < currentNode->points.size(); ++currentPointNumber) {
				double newDistance = distanceBetweenPoints(currentNode->points[currentPointNumber], point);
				if (newDistance < distance + EPS) {
					identifier = currentNode->points[currentPointNumber].identifier;
					distance = newDistance;
				}
			}

			return;
		}

		if (distanceBetweenPoints(currentNode->leftChild->middlePoint, point) < distanceBetweenPoints(currentNode->rightChild->middlePoint, point)) {
			getMinDistance(currentNode->leftChild, point, distance, identifier);
			if (checkSubtree(currentNode->rightChild->leftPoint, currentNode->rightChild->rightPoint, point, distance)) {
				getMinDistance(currentNode->rightChild, point, distance, identifier);
			}
		} else {
			getMinDistance(currentNode->rightChild, point, distance, identifier);
			if (checkSubtree(currentNode->leftChild->leftPoint, currentNode->leftChild->rightPoint, point, distance)) {
				getMinDistance(currentNode->leftChild, point, distance, identifier);
			}
		}
	}

	/*double getFirstDistance(KDTreeNode* currentNode, const TypePoint &point, int &identifier) const {
		if ((currentNode->leftChild == NULL) && (currentNode->rightChild == NULL)) {
			identifier = currentNode->points[0].identifier;
			return distanceBetweenPoints(point, currentNode->points[0]) + EPS;
		}

		if (distanceBetweenPoints(currentNode->leftChild->middlePoint, point) < distanceBetweenPoints(currentNode->rightChild->middlePoint, point)) {
			return getFirstDistance(currentNode->leftChild, point, identifier);
		} else {
			return getFirstDistance(currentNode->rightChild, point, identifier);
		}
	}

	void getMinDistance(KDTreeNode* currentNode, const TypePoint &point, double &distance, int &identifier) const {
		if ((currentNode->leftChild == NULL) && (currentNode->rightChild == NULL)) {
			for (size_t currentPointNumber = 0; currentPointNumber < currentNode->points.size(); ++currentPointNumber) {
				double newDistance = distanceBetweenPoints(currentNode->points[currentPointNumber], point);
				if (newDistance < distance) {
					identifier = currentNode->points[currentPointNumber].identifier;
					distance = newDistance;
				}
			}

			return;
		}

		if (checkSubtree(currentNode->rightChild->leftPoint, currentNode->rightChild->rightPoint, point, distance)) {
			getMinDistance(currentNode->rightChild, point, distance, identifier);
		}

		if (checkSubtree(currentNode->leftChild->leftPoint, currentNode->leftChild->rightPoint, point, distance)) {
			getMinDistance(currentNode->leftChild, point, distance, identifier);
		}
	}
	*/

	void destroyTree(KDTreeNode* currentNode) {
		if (currentNode->leftChild != NULL) {
			destroyTree(currentNode->leftChild);
		}

		if (currentNode->rightChild != NULL) {
			destroyTree(currentNode->rightChild);
		}

		delete currentNode;
	}

public:
	KDTree(const std::vector<TypePoint> &points) {
		root_ = buildTree(points);
	}

	~KDTree() {
		destroyTree(root_);
	}

	int getMinDistanceIdentifier(const TypePoint &point, double &distance) const {
		int resultIdentifier;

		distance = 10000000;//getFirstDistance(root_, point, resultIdentifier);
		getMinDistance(root_, point, distance, resultIdentifier);

		distance = sqrt(distance);
		return resultIdentifier;
	}
};

void inputPoints(std::vector<TypePoint>* points) {
	int pointsNumber;
	int dimension;

	std::cin >> pointsNumber >> dimension;
	(*points).resize(pointsNumber);

	for (int currentPointNumber = 0; currentPointNumber < pointsNumber; ++currentPointNumber) {
		(*points)[currentPointNumber].changeDimension(dimension);
		(*points)[currentPointNumber].identifier = currentPointNumber;

		for (int currentCoordinate = 0; currentCoordinate < dimension; ++currentCoordinate) {
			std::cin >> (*points)[currentPointNumber].coordinates[currentCoordinate];
		}
	}
}

void answerRequests(const KDTree& tree, int dimension) {
	int requestsNumber;
	std::cin >> requestsNumber;

	for (int currentRequestNumber = 0; currentRequestNumber < requestsNumber; ++currentRequestNumber) {
		TypePoint currentPoint(dimension, 0);

		for (int currentCoordinate = 0; currentCoordinate < currentPoint.getDimension(); ++currentCoordinate) {
			std::cin >> currentPoint.coordinates[currentCoordinate];
		}

		double distance = 0;
		std::cout << tree.getMinDistanceIdentifier(currentPoint, distance) << ' ';
		std::cout << std::setprecision(15) << distance << std::endl;
	}
}