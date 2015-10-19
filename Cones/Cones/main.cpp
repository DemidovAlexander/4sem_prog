#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

const double EPSILON = 1E-7;

class TypeVertex {
public:
	double x;
	double y;
	double z;

	TypeVertex() {}

	TypeVertex(double xCoord, double yCoord, double zCoord) {
		x = xCoord;
		y = yCoord;
		z = zCoord;
	}
};

class TypeVector {
public:
	double x;
	double y;
	double z;

	TypeVector(const TypeVertex& firstVertex, const TypeVertex& secondVertex) {
		x = secondVertex.x - firstVertex.x;
		y = secondVertex.y - firstVertex.y;
		z = secondVertex.z - firstVertex.z;
	}

	double length() const {
		return sqrt(x * x + y * y + z * z);
	}

	void changeLength(double newLength) {
		double currentLength = length();
		if (currentLength != 0) {
			double factor = newLength / currentLength;
			x = x * factor;
			y = y * factor;
			z = z * factor;
		}
	}
};

class TypeSegment {
public:
	TypeVertex firstBound;
	TypeVertex secondBound;
	
	TypeSegment() {}

	TypeSegment(const TypeVertex& newFirstBound, const TypeVertex& newSecondBound) {
		firstBound = newFirstBound;
		secondBound = newSecondBound;
	}

	void projectionXY(TypeSegment* segment) const {
		(*segment).firstBound = firstBound;
		(*segment).secondBound = secondBound;
		(*segment).firstBound.z = 0;
		(*segment).secondBound.z = 0;
	}
};

double ScalarProduct(const TypeVector firstVector, const TypeVector secondVector) {
	return firstVector.x * secondVector.x + firstVector.y * secondVector.y + firstVector.z * secondVector.z;
}

bool GetReferenceVertex(const TypeVertex& vertex, const TypeSegment& segment, const TypeSegment& segmentProjection, TypeVertex* newVertex) {
	TypeVector firstVector(segmentProjection.firstBound, vertex);
	TypeVector secondVector(segmentProjection.firstBound, segmentProjection.secondBound);
		
	double scalarProduct = ScalarProduct(firstVector, secondVector);	
	double distanceFromFirstBound = scalarProduct / secondVector.length();
	
	if ((scalarProduct - EPSILON < 0) || (distanceFromFirstBound + EPSILON > secondVector.length())) {
		return false;
	}

	TypeVector thirdVector(segment.firstBound, segment.secondBound);
	thirdVector.changeLength(thirdVector.length() * distanceFromFirstBound / secondVector.length());

	(*newVertex).x = thirdVector.x + segment.firstBound.x;
	(*newVertex).y = thirdVector.y + segment.firstBound.y;
	(*newVertex).z = thirdVector.z + segment.firstBound.z;

	return true;
}

class Cone {
public:
	TypeVertex baseCenter;
	double radius;
	double height;

	bool checkSegmentIntersection(const TypeSegment& currentSegment) const {
		if (checkVertexInclusion(currentSegment.firstBound) || checkVertexInclusion(currentSegment.secondBound)) {
			return true;
		}

		TypeSegment segmentProjection;
		currentSegment.projectionXY(&segmentProjection);
		
		TypeVertex referenceVertex;

		if (!GetReferenceVertex(baseCenter, currentSegment, segmentProjection, &referenceVertex)) {
			return false;
		}

		if (checkVertexInclusion(referenceVertex)) {
			return true;
		}

		return false;
	}

private:
	bool checkVertexInclusion(const TypeVertex& vertex) const {
		if (vertex.z - EPSILON > height) {
			return false;
		}
		TypeVertex vertexProjection(vertex.x, vertex.y, 0);
		TypeVector vectorProjection(baseCenter, vertexProjection);
		if (vectorProjection.length() - EPSILON > radius * (height - vertex.z) / height) {
			return false;
		}
		return true;
	}
};

void InputCones(vector<Cone>* cones) {
	int numCones;
	cin >> numCones;
	(*cones).resize(numCones);
	for (int i = 0; i < numCones; ++i) {
		cin >> (*cones)[i].baseCenter.x >> (*cones)[i].baseCenter.y >> (*cones)[i].radius >> (*cones)[i].height;
		(*cones)[i].baseCenter.z = 0;
	}
}

bool WayExists(const vector<Cone>& cones, int firstConeNumber, int secondConeNumber) {
	TypeVertex firstVertex(cones[firstConeNumber].baseCenter.x, cones[firstConeNumber].baseCenter.y, cones[firstConeNumber].height);
	TypeVertex secondVertex(cones[secondConeNumber].baseCenter.x, cones[secondConeNumber].baseCenter.y, cones[secondConeNumber].height);
	TypeSegment currentSegment(firstVertex, secondVertex);
	for (int i = 0; i < cones.size(); ++i) {
		if ((i != firstConeNumber) && (i != secondConeNumber) && (cones[i].checkSegmentIntersection(currentSegment))) {
			return false;
		}
	}
	return true;
}

int GetNumberOfSegments(const vector<Cone>& cones) {
	int step = 0;
	vector<bool> used(cones.size(), false);
	queue<int> currentLevel;
	currentLevel.push(0);
	used[0] = true;

	while (!currentLevel.empty()) {
		++step;
		int levelSize = currentLevel.size();
		for (int i = 0; i < levelSize; ++i) {
			int currentConeNumber = currentLevel.front();
			currentLevel.pop();
			for (int conNumber = 0; conNumber < cones.size(); ++conNumber) {
				if ((conNumber != currentConeNumber) && (!used[conNumber]) && (WayExists(cones, currentConeNumber, conNumber))) {
					if (conNumber == cones.size() - 1) {
						return step;
					}
					currentLevel.push(conNumber);
					used[conNumber] = true;
				}
			}
		}
	}

	return 0;
}

int main() {
	vector<Cone> cones;
	InputCones(&cones);

	cout << GetNumberOfSegments(cones);

	return 0;
}