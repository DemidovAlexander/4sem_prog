#include <iostream>
#include <vector>
#include <algorithm>

const double PI = 3.14159265359;

typedef std::pair<double, double> TypeVertex;

class TypeVector {
public:
	double x;
	double y;

	TypeVector(TypeVertex firstVertex, TypeVertex secondVertex) {
		x = secondVertex.first - firstVertex.first;
		y = secondVertex.second - firstVertex.second;
	}

	void update(TypeVertex firstVertex, TypeVertex secondVertex) {
		x = secondVertex.first - firstVertex.first;
		y = secondVertex.second - firstVertex.second;
	}

	double length() {
		return sqrt(x * x + y * y);
	}
};

class TypeLine {
private:
	double A;
	double B;
	double C;

public:
	TypeLine(TypeVertex firstVertex, TypeVertex secondVertex) {
		A = firstVertex.second - secondVertex.second;
		B = secondVertex.first - firstVertex.first;
		C = - A * secondVertex.first - B * secondVertex.second;
	}

	bool checkVertex(TypeVertex vertex) const {
		if (A * vertex.first + B * vertex.second + C > 0) {
			return true;
		} else {
			return false;
		}
	}
};

void InputPoints(int numVertices, std::vector<TypeVertex>* vertices) {
	(*vertices).resize(numVertices);
	for (int i = 0; i < numVertices; ++i) {
		std::cin >> (*vertices)[i].first >> (*vertices)[i].second;
	}
}

double CrossProduct(const TypeVector firstVector, const TypeVector secondVector) {
	return firstVector.x * secondVector.y - firstVector.y * secondVector.x;
}

double GetPartHullPerimeter(const std::vector<TypeVertex> &vertices, int partIndicator) { //bottom part: partIndicator < 0; top part: partIndicator> 0 
	TypeVector previousVector(vertices[0], vertices[1]);
	std::vector<TypeVertex> convexHull;
	convexHull.push_back(vertices[0]);
	convexHull.push_back(vertices[1]);

	double result = previousVector.length();

	for (size_t vertexNumber = 2; vertexNumber < vertices.size(); ++vertexNumber) {
		TypeVector currentVector(convexHull[convexHull.size() - 1], vertices[vertexNumber]);
		while ((partIndicator * CrossProduct(previousVector, currentVector) >= 0) && (convexHull.size() > 2)) {
			result -= previousVector.length();
			convexHull.pop_back();
			previousVector.update(convexHull[convexHull.size() - 2], convexHull[convexHull.size() - 1]);
			currentVector.update(convexHull[convexHull.size() - 1], vertices[vertexNumber]);
		}
		if (partIndicator * CrossProduct(previousVector, currentVector) >= 0) {
			convexHull.pop_back();
			previousVector.update(convexHull[0], vertices[vertexNumber]);
			result = previousVector.length();
		} else {
			result += currentVector.length();
			previousVector = currentVector;
		}
		convexHull.push_back(vertices[vertexNumber]);
	}

	return result;
}

double GetConvexHullPerimeter(std::vector<TypeVertex>* vertices) {
	std::sort((*vertices).begin(), (*vertices).end());

	TypeLine line((*vertices)[0], (*vertices)[(*vertices).size() - 1]);

	std::vector<TypeVertex> topPart;
	std::vector<TypeVertex> bottomPart;
	topPart.push_back((*vertices)[0]);
	bottomPart.push_back((*vertices)[0]);

	for (size_t pointNumber = 1; pointNumber < (*vertices).size() - 1; ++pointNumber) {
		if (line.checkVertex((*vertices)[pointNumber])) {
			topPart.push_back((*vertices)[pointNumber]);
		} else {
			bottomPart.push_back((*vertices)[pointNumber]);
		}
	}

	topPart.push_back((*vertices)[(*vertices).size() - 1]);
	bottomPart.push_back((*vertices)[(*vertices).size() - 1]);

	return GetPartHullPerimeter(bottomPart, -1) + GetPartHullPerimeter(topPart, 1);
}

int main() {
	std::vector<TypeVertex> vertices;
	int verticesNum;
	int distanceFromCastle;
	std::cin >> verticesNum >> distanceFromCastle;
	InputPoints(verticesNum, &vertices);

	std::cout << (int) (GetConvexHullPerimeter(&vertices) + 2 * PI * distanceFromCastle + 0.5);

	return 0;
}