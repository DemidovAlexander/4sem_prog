#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <map>
#include <utility>
#include <cstdio>

const long double PI = 3.141592;
const long double EPS = 1E-7;

class TypePoint {
private:
	long double x;
	long double y;
	long double z;
	size_t pointNumber;

public:
	TypePoint() {
		//do nothing
	}

	TypePoint(long double newX, long double newY, long double newZ) :
		x(newX), y(newY), z(newZ) {
		
		//do nothing
	}

	TypePoint(long double newX, long double newY, long double newZ, size_t i) :
		x(newX), y(newY), z(newZ), pointNumber(i) {
		
		//do nothing
	}

	long double getX() const {
		return x;
	}

	long double getY() const {
		return y;
	}

	long double getZ() const {
		return z;
	}

	size_t getPointNumber() const {
		return pointNumber;
	}

	const TypePoint operator-(const TypePoint& other) {
		return TypePoint(x - other.x, y - other.y, z - other.z);
	};

	const TypePoint operator+(const TypePoint& other) {
		return TypePoint(x + other.x, y + other.y, z + other.z);
	};

	const bool operator<(const TypePoint& other) const {
		if (y < other.y) {
			return true;
		}
		if (abs(y - other.y) < EPS) {
			if (z < other.z) {
				return true;
			}
			if (abs(z - other.z) < EPS) {
				if (x < other.x) {
					return true;
				}
				return false;
			}
			return false;
		}
		return false;
	};

	const bool operator!=(const TypePoint& other) const {
		return (abs(x - other.x) >= EPS) || (abs(y - other.y) >= EPS) || (abs(z - other.z) >= EPS);
	};
};

class TypeVector {
private:
	long double x;
	long double y;
	long double z;

public:
	TypeVector() {
		//do nothing
	}

	TypeVector(const TypePoint &firstPoint, const TypePoint &secondPoint) {
		x = secondPoint.getX() - firstPoint.getX();
		y = secondPoint.getY() - firstPoint.getY();
		z = secondPoint.getZ() - firstPoint.getZ();
	}

	TypeVector(const long double newX, const long double newY, const long double newZ) :
		x(newX), y(newY), z(newZ) {
		
		//do nothing
	}

	long double getX() const {
		return x;
	}

	long double getY() const {
		return y;
	}

	long double getZ() const {
		return z;
	}

	long double length() const {
		return sqrt(x * x + y * y + z * z);
	}

	long double scalarProduct(const TypeVector &vector) const {
		return vector.x * x + vector.y * y + vector.z * z;
	}

	void crossProduct(const TypeVector &firstVector, const TypeVector &secondVector) {
		x = firstVector.y * secondVector.z - firstVector.z * secondVector.y;
		y = secondVector.x * firstVector.z - firstVector.x * secondVector.z;
		z = firstVector.x * secondVector.y - firstVector.y * secondVector.x;
	}

};

class TypeSegment {
private:
	TypePoint firstPoint;
	TypePoint secondPoint;
	TypePoint otherPoint;
	TypeVector normal;
	bool closed;

public:
	TypeSegment() :
		closed(false) {
		
		//do nothing
	}

	TypeSegment(const TypePoint &newFirstPoint, const TypePoint& newSecondPoint) :
		firstPoint(newFirstPoint), secondPoint(newSecondPoint), normal(), closed(false) {
		
		//do nothing
	}

	TypeSegment(const TypePoint &newFirstPoint, const TypePoint &newSecondPoint, const TypePoint &newOtherPoint, const TypeVector &newNormal) :
		firstPoint(newFirstPoint), secondPoint(newSecondPoint), otherPoint(newOtherPoint), normal(newNormal), closed(false) {
		
		//do nothing
	}

	TypeVector makeVectorFromSegment(const bool invert) {
		if (!invert) {
			return TypeVector(secondPoint.getX() - firstPoint.getX(),
				secondPoint.getY() - firstPoint.getY(),
				secondPoint.getZ() - firstPoint.getZ());
		} else {
			return TypeVector(firstPoint.getX() - secondPoint.getX(),
				firstPoint.getY() - secondPoint.getY(),
				firstPoint.getZ() - secondPoint.getZ());
		}
	}

	TypePoint getFirstPoint() {
		return firstPoint;
	}

	TypePoint getSecondPoint() {
		return secondPoint;
	}

	TypePoint getOtherPoint() {
		return otherPoint;
	}

	TypeVector getNormal() {
		return normal;
	}

	void close() {
		closed = true;
	}

	void addNormal(const TypeVector &newNormal) {
		normal = newNormal;
	}

	bool isClosed() {
		return closed;
	}

};

class TypePlane {
private:
	TypePoint a;
	TypePoint b;
	TypePoint c;
	TypeVector normal;

public:
	TypePlane() {
		//do nothing
	}

	TypePlane(const TypePoint &firstPoint, const TypePoint &secondPoint, const TypePoint &thirdPoint, const TypeVector &newNormal) :
		a(firstPoint), b(secondPoint), c(thirdPoint), normal(newNormal) {
		
		//do nothing
	}

	TypePoint getA() {
		return a;
	}

	TypePoint getB() {
		return b;
	}

	TypePoint getC() {
		return c;
	}

	TypeVector getNormal() {
		return normal;
	}

	void orderPoints() {
		TypeVector firstVector(b.getX() - a.getX(), b.getY() - a.getY(), b.getZ() - a.getZ());
		TypeVector secondVector(c.getX() - a.getX(), c.getY() - a.getY(), c.getZ() - a.getZ());

		TypeVector currentVector;
		currentVector.crossProduct(secondVector, firstVector);

		if (currentVector.scalarProduct(normal) > 0) {
			TypePoint bufferPoint;
			bufferPoint = b;
			b = c;
			c = bufferPoint;
		}

		if ((a.getPointNumber() < b.getPointNumber()) && (a.getPointNumber() < c.getPointNumber())) {
			return;
		}

		if ((b.getPointNumber() < a.getPointNumber()) && (b.getPointNumber() < c.getPointNumber())) {
			TypePoint a1 = a;
			TypePoint b1 = b;
			TypePoint c1 = c;
			a = b1;
			b = c1;
			c = a1;

			return;
		}

		if ((c.getPointNumber() < a.getPointNumber()) && (c.getPointNumber() < b.getPointNumber())) {
			TypePoint a1 = a;
			TypePoint b1 = b;
			TypePoint c1 = c;
			a = c1;
			b = a1;
			c = b1;

			return;
		}
	}

	void print() const {
		std::cout << 3 << " " << a.getPointNumber() << " " << b.getPointNumber() << " " << c.getPointNumber() << std::endl;
	}

	const bool operator<(const TypePlane& other) const {
		if (a.getPointNumber() < other.a.getPointNumber()) {
			return true;
		}
		if (a.getPointNumber() == other.a.getPointNumber()) {
			if (b.getPointNumber() < other.b.getPointNumber()) {
				return true;
			}
			if (b.getPointNumber() == other.b.getPointNumber()) {
				if (c.getPointNumber() < other.c.getPointNumber()) {
					return true;
				}
				return false;
			}
			return false;
		}
		return false;
	};

};

long double angleBetweenVectors(const TypeVector& firstVector, const TypeVector &secondVector) {
	return std::acos(firstVector.scalarProduct(secondVector) / (firstVector.length() * secondVector.length()));
}

void SwapSides(TypePlane &firstPlane, TypePlane& secondPlane) {
	TypePlane bufferPlane;
	bufferPlane = firstPlane;
	firstPlane = secondPlane;
	secondPlane = bufferPlane;
}

void Sort(std::vector<TypePlane> &planes, int l, int r) {
	TypePlane x = planes[l + (r - l) / 2];
	int i = l;
	int j = r;
	while (i <= j) {
		while (planes[i] < x) ++i;
		while (x < planes[j]) --j;
		if (i <= j) {
			SwapSides(planes[i], planes[j]);
			++i;
			--j;
		}
	}
	if (i < r)
		Sort(planes, i, r);
	if (l < j)
		Sort(planes, l, j);
}

void printPlanes(std::vector<TypePlane>& planes) {
	std::cout << planes.size() << std::endl;

	for (size_t currentPlaneNumber = 0; currentPlaneNumber < planes.size(); ++currentPlaneNumber) {
		planes[currentPlaneNumber].orderPoints();
	}

	Sort(planes, 0, planes.size() - 1);

	for (size_t currentPlaneNumber = 0; currentPlaneNumber < planes.size(); ++currentPlaneNumber) {
		planes[currentPlaneNumber].print();
	}
}

bool checkPointsRelativeToPlane(const TypePoint &firstPoint, const TypePoint &secondPoint, const TypePoint &thirdPoint, const std::vector<TypePoint> &points) {
	TypeVector firstVector(secondPoint.getX() - firstPoint.getX(), secondPoint.getY() - firstPoint.getY(), secondPoint.getZ() - firstPoint.getZ());
	TypeVector secondVector(thirdPoint.getX() - firstPoint.getX(), thirdPoint.getY() - firstPoint.getY(), thirdPoint.getZ() - firstPoint.getZ());
	
	TypeVector normal;
	normal.crossProduct(firstVector, secondVector);

	long double A = normal.getX();
	long double B = normal.getY();
	long double C = normal.getZ();
	long double D = -A * firstPoint.getX() - B * firstPoint.getY() - C * firstPoint.getZ();

	bool lessZero = false;
	bool biggerZero = false;
	for (size_t currentPointNumber = 0; currentPointNumber < points.size(); ++currentPointNumber) {
		if ((points[currentPointNumber] != firstPoint) && (points[currentPointNumber] != secondPoint) && (points[currentPointNumber] != thirdPoint)) {
			if (A * points[currentPointNumber].getX() + B * points[currentPointNumber].getY() + C * points[currentPointNumber].getZ() + D < 0) {
				lessZero = true;
			}
			else {
				biggerZero = true;
			}

			if (lessZero && biggerZero) {
				return false;
			}
		}
	}

	return true;
}

TypePoint getFirstPoint(const std::vector<TypePoint> &points) {
	TypePoint minPoint = points[0];

	for (size_t currentPointNumber = 1; currentPointNumber < points.size(); ++currentPointNumber) {
		if (points[currentPointNumber] < minPoint) {
			minPoint = points[currentPointNumber];
		}
	}

	return minPoint;
}

TypePoint getSecondPoint(const TypePoint &firstPoint, const std::vector<TypePoint> &points) {
	double maxAngle = 0;
	TypePoint maxPoint;
	TypeVector yAxis(0, 1, 0);

	for (size_t currentPointNumber = 0; currentPointNumber < points.size(); ++currentPointNumber) {
		TypeVector currentVector(points[currentPointNumber].getX() - firstPoint.getX(), 
					points[currentPointNumber].getY() - firstPoint.getY(), 
					points[currentPointNumber].getZ() - firstPoint.getZ());

		if (angleBetweenVectors(yAxis, currentVector) > maxAngle) {
			maxAngle = angleBetweenVectors(yAxis, currentVector);
			maxPoint = points[currentPointNumber];
		}
	}

	return maxPoint;
}

TypePoint getThirdPoint(const TypePoint &firstPoint, const TypePoint &secondPoint, const std::vector<TypePoint> &points) {
	for (size_t currentPointNumber = 0; currentPointNumber < points.size(); ++currentPointNumber) {
		if ((points[currentPointNumber] != firstPoint) && (points[currentPointNumber] != secondPoint)) {
			if (checkPointsRelativeToPlane(points[currentPointNumber], firstPoint, secondPoint, points)) {
				return points[currentPointNumber];
			}
		}
	}

	return TypePoint();
}

TypeVector getOutsideNormal(const TypePoint &firstPoint, const TypePoint &secondPoint, const TypePoint &thirdPoint,
							const TypePoint &checkPoint, const std::vector<TypePoint> &points) {

	TypeVector firstVector(secondPoint.getX() - firstPoint.getX(), secondPoint.getY() - firstPoint.getY(), secondPoint.getZ() - firstPoint.getZ());
	TypeVector secondVector(thirdPoint.getX() - firstPoint.getX(), thirdPoint.getY() - firstPoint.getY(), thirdPoint.getZ() - firstPoint.getZ());
	
	TypeVector firstNormal, secondNormal;
	firstNormal.crossProduct(firstVector, secondVector);
	secondNormal.crossProduct(secondVector, firstVector);

	long double A = firstNormal.getX();
	long double B = firstNormal.getY();
	long double C = firstNormal.getZ();
	long double D = -A * firstPoint.getX() - B * firstPoint.getY() - C * firstPoint.getZ();

	int sign = (int)(A * checkPoint.getX() + B * checkPoint.getY() + C * checkPoint.getZ() + D);

	TypePoint thirdToNormal(thirdPoint.getX() + firstNormal.getX(), thirdPoint.getY() + firstNormal.getY(), thirdPoint.getZ() + firstNormal.getZ());
	if (sign * (A * thirdToNormal.getX() + B * thirdToNormal.getY() + C * thirdToNormal.getZ() + D) < 0) {
		return firstNormal;
	} else {
		return secondNormal;
	}
}

TypePlane getFirstPlane(const std::vector<TypePoint>& points) {
	TypePoint firstPoint = getFirstPoint(points);
	TypePoint secondPoint = getSecondPoint(firstPoint, points);
	TypePoint thirdPoint = getThirdPoint(firstPoint, secondPoint, points);

	TypeVector firstVector(secondPoint.getX() - firstPoint.getX(), secondPoint.getY() - firstPoint.getY(), secondPoint.getZ() - firstPoint.getZ());
	TypeVector secondVector(thirdPoint.getX() - firstPoint.getX(), thirdPoint.getY() - firstPoint.getY(), thirdPoint.getZ() - firstPoint.getZ());
	
	TypeVector firstNormal;
	TypeVector secondNormal;
	firstNormal.crossProduct(firstVector, secondVector);
	secondNormal.crossProduct(secondVector, firstVector);

	long double A = firstNormal.getX();
	long double B = firstNormal.getY();
	long double C = firstNormal.getZ();
	long double D = -A * firstPoint.getX() - B * firstPoint.getY() - C * firstPoint.getZ();

	int sign;
	for (size_t currentPointNumber = 0; currentPointNumber < points.size(); ++currentPointNumber) {
		if ((points[currentPointNumber] != firstPoint) && (points[currentPointNumber] != secondPoint) && (points[currentPointNumber] != thirdPoint)) {
			sign = (int)(A * points[currentPointNumber].getX() + B * points[currentPointNumber].getY() + C * points[currentPointNumber].getZ() + D);
			break;
		}
	}

	TypePoint checkPoint(firstPoint.getX() + firstNormal.getX(), firstPoint.getY() + firstNormal.getY(), firstPoint.getZ() + firstNormal.getZ());

	if ((A * checkPoint.getX() + B * checkPoint.getY() + C * checkPoint.getZ() + D) * sign < 0) {		
		return TypePlane(firstPoint, secondPoint, thirdPoint, firstNormal);
	}
	else {
		return TypePlane(firstPoint, secondPoint, thirdPoint, secondNormal);
	}
}

void getNextPoint(TypePoint* nextPoint, TypeVector* newNormal, const TypePoint &firstPoint, const TypePoint& secondPoint, 
					const TypePoint &checkPoint, const TypeVector &currentNormal, const std::vector<TypePoint> &points) {

	long double minAngle = 4;
	for (size_t currentPointNumber = 0; currentPointNumber < points.size(); ++currentPointNumber) {
		if ((points[currentPointNumber] != firstPoint) && (points[currentPointNumber] != secondPoint)) {
			TypePoint currentPoint = points[currentPointNumber];
			TypeVector normal = getOutsideNormal(firstPoint, secondPoint, currentPoint, checkPoint, points);

			long double currAngle = angleBetweenVectors(normal, currentNormal);

			if ((currAngle <  minAngle) && (currAngle >= EPS)) {
				minAngle = currAngle;
				(*newNormal) = normal;					
				(*nextPoint) = currentPoint;
			}
		}
	}
}

void printConvexHull(const std::vector<TypePoint> &points) {
	std::vector<TypePlane> planes;
	std::vector<TypeSegment> segments;

	std::map<std::pair<int, int>, int> segmentsMap;

	TypePlane firstPlane = getFirstPlane(points);
	planes.push_back(firstPlane);

	segments.push_back(TypeSegment(firstPlane.getA(), firstPlane.getB(), firstPlane.getC(), firstPlane.getNormal()));
	segments.push_back(TypeSegment(firstPlane.getA(), firstPlane.getC(), firstPlane.getB(), firstPlane.getNormal()));
	segments.push_back(TypeSegment(firstPlane.getB(), firstPlane.getC(), firstPlane.getA(), firstPlane.getNormal()));

	segmentsMap[std::make_pair(firstPlane.getA().getPointNumber(), firstPlane.getB().getPointNumber())] = 0;
	segmentsMap[std::make_pair(firstPlane.getB().getPointNumber(), firstPlane.getA().getPointNumber())] = 0;
	segmentsMap[std::make_pair(firstPlane.getA().getPointNumber(), firstPlane.getC().getPointNumber())] = 1;
	segmentsMap[std::make_pair(firstPlane.getC().getPointNumber(), firstPlane.getA().getPointNumber())] = 1;
	segmentsMap[std::make_pair(firstPlane.getB().getPointNumber(), firstPlane.getC().getPointNumber())] = 2;
	segmentsMap[std::make_pair(firstPlane.getC().getPointNumber(), firstPlane.getB().getPointNumber())] = 2;

	size_t currentSegment = 0;
	while (currentSegment < segments.size()) {
		if (segments[currentSegment].isClosed()) {
			++currentSegment;
			continue;
		}

		TypePoint firstPoint = segments[currentSegment].getFirstPoint();
		TypePoint secondPoint = segments[currentSegment].getSecondPoint();
		TypePoint thirdPoint = segments[currentSegment].getOtherPoint();

		TypePoint nextPoint;
		TypeVector newNormal;
		getNextPoint(&nextPoint, &newNormal, firstPoint, secondPoint, thirdPoint, segments[currentSegment].getNormal(), points);

		segments[currentSegment].close();

		TypeSegment segment1(firstPoint, nextPoint, secondPoint, newNormal);
		TypeSegment segment2(secondPoint, nextPoint, firstPoint, newNormal);

		if (segmentsMap.find(std::make_pair(firstPoint.getPointNumber(), nextPoint.getPointNumber())) != segmentsMap.end()) {
			segments[segmentsMap[std::make_pair(firstPoint.getPointNumber(), nextPoint.getPointNumber())]].close();
		} else {
			segments.push_back(segment1);
			segmentsMap[std::make_pair(firstPoint.getPointNumber(), nextPoint.getPointNumber())] = segments.size() - 1;
			segmentsMap[std::make_pair(nextPoint.getPointNumber(), firstPoint.getPointNumber())] = segments.size() - 1;
		}

		if (segmentsMap.find(std::make_pair(secondPoint.getPointNumber(), nextPoint.getPointNumber())) != segmentsMap.end()) {
			segments[segmentsMap[std::make_pair(secondPoint.getPointNumber(), nextPoint.getPointNumber())]].close();
		} else {
			segments.push_back(segment2);
			segmentsMap[std::make_pair(secondPoint.getPointNumber(), nextPoint.getPointNumber())] = segments.size() - 1;
			segmentsMap[std::make_pair(nextPoint.getPointNumber(), secondPoint.getPointNumber())] = segments.size() - 1;
		}

		TypePlane newPlane(firstPoint, secondPoint, nextPoint, newNormal);
		planes.push_back(newPlane);

		++currentSegment;
	}

	printPlanes(planes);
}

void inputPoints(std::vector<TypePoint>* points) {
	size_t pointsNumber;
	std::cin >> pointsNumber;

	for (size_t currentPointNumber = 0; currentPointNumber < pointsNumber; ++currentPointNumber) {
		double x;
		double y;
		double z;

		std::cin >> x;
		std::cin >> y;
		std::cin >> z;

		(*points).push_back(TypePoint(x, y, z, currentPointNumber));
	}
}

int main() {
	std::vector<TypePoint> points;
	inputPoints(&points);

	printConvexHull(points);
	
	return 0;
}