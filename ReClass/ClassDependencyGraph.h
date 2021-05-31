#pragma once
#include "CNodeClass.h"

#include <assert.h>
#include <atlbase.h>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>

class DependencyNode;

enum class DependencyType {
	POINTER,
	INSTANCE
};

struct DependencyEdge {
	const DependencyNode* dependency;
	const DependencyType	edgeType;

	DependencyEdge(const DependencyNode* dependency, const DependencyType edgeType) : dependency(dependency), edgeType(edgeType) {};
	bool operator==(const DependencyEdge& other) const {
		return this->dependency == other.dependency && this->edgeType == other.edgeType;
	}
};

class DependencyNode {
	const CNodeClass* cClass = nullptr;
	std::vector<DependencyEdge> dependencies;
	std::vector<DependencyEdge> inEdges;
public:
	DependencyNode() {};
	DependencyNode(const CNodeClass* cClass) : cClass(cClass) {};
	const std::vector<DependencyEdge>* GetDependencies() const;
	const std::vector<DependencyEdge>* GetInEdges() const { return &this->inEdges; }
	const CNodeClass* GetCClass() const { return this->cClass; }
	bool AddDependency(const DependencyNode* dep, const DependencyType edgeType);
	bool AddInEdge(const DependencyNode* parent, const DependencyType edgeType);
};

class ClassDependencyGraph {
	std::map<const CNodeClass*, DependencyNode> nodes;
	
	std::vector<DependencyNode*> GetLeafNodes();
	std::vector<DependencyNode*> GetInstanceNodes();
	enum class NodeGenerationStatus {
		UNPROCESSED,
		PROCESSING,
		PROCESSED,
		INSTANCED
	};
	int ProcessDependencies(DependencyEdge *node, std::map<const DependencyNode*, NodeGenerationStatus> &nodeStatus, std::set<const CNodeClass*>& forwardDeclarations, std::vector<const CNodeClass*> &classDefinitions);


public:
	bool AddNode(const CNodeClass* node) {
		if (this->nodes.find(node) == this->nodes.end()) {
			this->nodes[node] = DependencyNode(node);
			return true;
		}
		return false;
	}

	bool AddEdge(const CNodeClass *dependingClass, const CNodeClass* dependency, DependencyType depType) {
		// Ignore simple recursion in the dependency graph.  It won't help in our analysis and will just muddy
		// up the graph.
		if (dependingClass == dependency) {
			return false;
		}
		DependencyNode* dependingNode = &this->nodes[dependingClass];
		DependencyNode* dependencyNode = &this->nodes[dependency];
		dependencyNode->AddInEdge(dependingNode, depType);
		return dependingNode->AddDependency(dependencyNode, depType);
	}
	std::string ToDot(std::string graphLabel = "");
	int OrderClassesForGeneration(std::set<const CNodeClass*>& forwardDeclarations, std::vector<const CNodeClass*> &classDefinitions);
};