#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const double EPSILON = 1E-7;

class TypeVertex {
public:
	double x;
	double y;

	TypeVertex() {}

	TypeVertex(double xCoord, double yCoord) {
		x = xCoord;
		y = yCoord;
	}
};

class TypeVector {
public:
	double x;
	double y;

	TypeVector(const TypeVertex& firstVertex, const TypeVertex& secondVertex) {
		x = secondVertex.x - firstVertex.x;
		y = secondVertex.y - firstVertex.y;
	}

	double length() const {
		return sqrt(x * x + y * y);
	}
};

double CrossProduct(const TypeVector firstVector, const TypeVector secondVector) {
	return firstVector.x * secondVector.y - firstVector.y * secondVector.x;
}

int GetRotationSign(const TypeVertex& firstVertex, const TypeVertex& secondVertex, const TypeVertex& thirdVertex) {
	TypeVector firstVector(firstVertex, secondVertex);
	TypeVector secondVector(firstVertex, thirdVertex);
	if (CrossProduct(firstVector, secondVector) + EPSILON < 0) {
		return -1;
	} else if (CrossProduct(firstVector, secondVector) - EPSILON > 0) {
		return 1;
	} else {
		return 0;
	}
}

class TypeSegment {
public:
	TypeVertex firstBound;
	TypeVertex secondBound;
	
	TypeSegment() {}

	TypeSegment(const TypeVertex& newFirstBound, const TypeVertex& newSecondBound) {
		firstBound = newFirstBound;
		secondBound = newSecondBound;
	}

	bool checkSegmentIntersection(const TypeSegment& segment) {
		return ((GetRotationSign((*this).firstBound, (*this).secondBound, segment.firstBound) * GetRotationSign((*this).firstBound, (*this).secondBound, segment.secondBound) <= 0) 
			&& (GetRotationSign(segment.firstBound, segment.secondBound, (*this).firstBound) * GetRotationSign(segment.firstBound, segment.secondBound, (*this).secondBound) < 0));
	}
};

void InputPolygon(vector<TypeVertex>* polygon) {
	int numVetices;
	cin >> numVetices;
	(*polygon).resize(numVetices);
	for (int i = 0; i < numVetices; ++i) {
		cin >> (*polygon)[i].x >> (*polygon)[i].y;
	}
}

bool IsRotationClockwise(const TypeVertex& firstVertex, const TypeVertex& secondVertex, const TypeVertex& thirdVertex) {
	return (GetRotationSign(firstVertex, secondVertex, thirdVertex) < 0);
}

bool IsRotationCounterClockwise(const TypeVertex& firstVertex, const TypeVertex& secondVertex, const TypeVertex& thirdVertex) {
	return (GetRotationSign(firstVertex, secondVertex, thirdVertex) > 0);
}

bool CheckVertexInclusion(const vector<TypeVertex>& polygon, const TypeVertex& vertex) {
	if (IsRotationClockwise(polygon[0], polygon[polygon.size() - 1], vertex) || IsRotationCounterClockwise(polygon[0], polygon[1], vertex)) {
		return false;
	}

	int leftIndex = 1;
	int rightIndex = polygon.size() - 1;

	while (rightIndex - leftIndex > 1) {
		int midIndex = (leftIndex + rightIndex) / 2;

		if (IsRotationCounterClockwise(polygon[0], polygon[midIndex], vertex)) {
			rightIndex = midIndex;
		} else {
			leftIndex = midIndex;
		}
	}

	TypeSegment firstSegment(polygon[0], vertex);
	TypeSegment secondSegment(polygon[leftIndex], polygon[rightIndex]);
	if (firstSegment.checkSegmentIntersection(secondSegment)) {
		return false;
	} else {
		return true;
	}
}

bool CheckTopRight(const TypeVertex& firstVertex, const TypeVertex& secondVertex) {
	return ((firstVertex.y > secondVertex.y) || ((firstVertex.y == secondVertex.y) && (firstVertex.x > secondVertex.x)));
}

void GetMinkovskySum(const vector<TypeVertex>& firstPol, const vector<TypeVertex>& secondPol, vector<TypeVertex>* sum) {
	vector<TypeVertex> firstPolygon(firstPol);
	vector<TypeVertex> secondPolygon(secondPol);

	int firstIndex = 0;
	int secondIndex = 0;
	for (int i = 0; i < firstPolygon.size(); ++i) {
		if (CheckTopRight(firstPolygon[i], firstPolygon[firstIndex])) {
			firstIndex = i;
		}
	}
	for (int i = 0; i < secondPolygon.size(); ++i) {
		if (CheckTopRight(secondPolygon[i], secondPolygon[secondIndex])) {
			secondIndex = i;
		}
	}

	firstPolygon.push_back(firstPolygon[0]);
	secondPolygon.push_back(secondPolygon[0]);

	int firstStartIndex = firstIndex;
	int secondStartIndex = secondIndex;

	do {	
		TypeVertex vertex(firstPolygon[firstIndex].x + secondPolygon[secondIndex].x, firstPolygon[firstIndex].y + secondPolygon[secondIndex].y);
		(*sum).push_back(vertex);

		TypeVector firstVector(firstPolygon[firstIndex], firstPolygon[firstIndex + 1]);
		TypeVector secondVector(secondPolygon[secondIndex], secondPolygon[secondIndex + 1]);
		double crossProduct = CrossProduct(firstVector, secondVector);

		if (crossProduct > 0) {
			++secondIndex;		
		} else if (crossProduct < 0) {
			++firstIndex;
		} else {
			++firstIndex;
			++secondIndex;
		}

		if (firstIndex == firstPolygon.size() - 1) {
			firstIndex = 0;
		}

		if (secondIndex == secondPolygon.size() - 1) {
			secondIndex = 0;
		}
	} while ((firstIndex != firstStartIndex) || (secondIndex != secondStartIndex));
}

void GetMinkovskyDiff(const vector<TypeVertex>& firstPolygon, const vector<TypeVertex>& secondPolygon, vector<TypeVertex>* diff) {
	vector<TypeVertex> invertedSecondPolygon;
	invertedSecondPolygon.resize(secondPolygon.size());
	for (int i = 0; i < invertedSecondPolygon.size(); ++i) {
		invertedSecondPolygon[i].x = -secondPolygon[i].x;
		invertedSecondPolygon[i].y = -secondPolygon[i].y;
	}
	GetMinkovskySum(firstPolygon, invertedSecondPolygon, diff);
} 

bool CheckIntersection(const vector<TypeVertex>& firstPolygon, const vector<TypeVertex>& secondPolygon) {
	vector<TypeVertex> diff;
	GetMinkovskyDiff(firstPolygon, secondPolygon, &diff);

	TypeVertex nullVertex(0, 0);
	return CheckVertexInclusion(diff, nullVertex);
}

int main() {
	vector<TypeVertex> firstPolygon;
	vector<TypeVertex> secondPolygon;
	InputPolygon(&firstPolygon);
	InputPolygon(&secondPolygon);
	
	if (CheckIntersection(firstPolygon, secondPolygon)) {
		cout << "YES" << endl;
	} else {
		cout << "NO" << endl;
	}

 	return 0;
}