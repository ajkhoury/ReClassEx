#include "stdafx.h"
#include "ClassDependencyGraph.h"


bool DependencyNode::AddInEdge(const DependencyNode* parent, const DependencyType edgeType) {
	DependencyEdge newEdge(parent, edgeType);
	//
	// We do not allow parallel edges, so we check to make sure this edge doesn't already exist.
	// We COULD have used a map for the dependencies container.  But, most classes probably won't
	// have a ton of dependencies, so a vector makes sense for our more average case.
	//
	for (auto edge : this->inEdges) {
		if (edge == newEdge) {
			return false;
		}
	}
	this->inEdges.push_back(newEdge);
	return true;
}

const std::vector<DependencyEdge>* DependencyNode::GetDependencies() const {
	return &this->dependencies;
}

bool DependencyNode::AddDependency(const DependencyNode* dep, const DependencyType edgeType) {
	DependencyEdge newEdge(dep, edgeType);
	//
	// We do not allow parallel edges, so we check to make sure this edge doesn't already exist.
	// We COULD have used a map for the dependencies container.  But, most classes probably won't
	// have a ton of dependencies, so a vector makes sense for our more average case.
	//
	for (auto edge : this->dependencies) {
		if (edge == newEdge) {
			return false;
		}
	}
	this->dependencies.push_back(newEdge);
	return true;
}


std::vector<DependencyNode*> ClassDependencyGraph::GetLeafNodes() {
	std::vector<DependencyNode*> result;
	for (auto it = nodes.begin(); it != nodes.end(); it++) {
		if (it->second.GetInEdges()->size() == 0) {
			result.push_back(&it->second);
		}
	}
	return result;
}

std::vector<DependencyNode*> ClassDependencyGraph::GetInstanceNodes() {
	std::vector<DependencyNode*> result;
	for (auto it = nodes.begin(); it != nodes.end(); it++) {
		bool hasOutInstanceEdge = false;
		
		for (auto inEdge : *it->second.GetInEdges()) {
			if (inEdge.edgeType == DependencyType::INSTANCE) {
				hasOutInstanceEdge = true;
				break;
			}
		}
		if (hasOutInstanceEdge) {
			result.push_back(&it->second);
		}
	}
	return result;
}

int ClassDependencyGraph::ProcessDependencies(DependencyEdge* edge, std::map<const DependencyNode*, NodeGenerationStatus>& nodeStatus, std::set<const CNodeClass*>& forwardDeclarations, std::vector<const CNodeClass*> &classDefinitions) {
	int classesAdded = 0;
	NodeGenerationStatus nodeGenStatus;
	const DependencyNode* node = edge->dependency;
	if (nodeStatus.find(node) == nodeStatus.end()) {
		nodeStatus[node] = NodeGenerationStatus::UNPROCESSED;
	}
	nodeGenStatus = nodeStatus[node];
	// We have to treat instanced nodes differently, since they have 'hard' dependencies
	// on their parent classes.  If we come across an instanced node through a pointer, 
	// we will forward declare it because if we try to process it like normal we may end
	// up with a back edge to its parent that will break everything.
	//
	// If we come upon an instanced node througn an instance, we process it like normal.
	if (nodeGenStatus == NodeGenerationStatus::INSTANCED) {
		if (edge->edgeType == DependencyType::POINTER) {
			forwardDeclarations.insert(node->GetCClass());
			return 0;
		}
		else if (edge->edgeType == DependencyType::INSTANCE) {
			// Change nodeGenStatys to trigger the actual processing of the node
			nodeGenStatus = NodeGenerationStatus::UNPROCESSED;
		}
	}
	if (nodeGenStatus == NodeGenerationStatus::PROCESSED) {
		// The node is already written to the output, no further processing needed
		return classesAdded;
	}
	else if (nodeGenStatus == NodeGenerationStatus::PROCESSING) {
		// We've hit a back edge.  Since we start from leaf nodes, this dependency cannot
		// possibly be an instance dependency, it must be a pointer dependency.  Thus, a 
		// forward declaration is enough to handle this case.
		ASSERT(edge->edgeType == DependencyType::POINTER);
		forwardDeclarations.insert(node->GetCClass());
		// While it isn't actually finished processing, if we leave it as PROCESSING we'll
		// get a new forward declaration every time we hit this node.  If it's currently 
		// processing, somewhere up the call stack is the actual processing of this node
		// so on return it'll get finished up.
		nodeStatus[node] = NodeGenerationStatus::PROCESSED;
	}
	else {
		// Case where processing has yet to begin.  We need to recursively process all dependencies, 
		// then add this class to the ordered list of definitions.
		nodeStatus[node] = NodeGenerationStatus::PROCESSING;
		for (auto dep : *node->GetDependencies()) {
			if (dep.edgeType == DependencyType::INSTANCE)
				classesAdded += ProcessDependencies(&dep, nodeStatus, forwardDeclarations, classDefinitions);
		}
		for (auto dep : *node->GetDependencies()) {
			if (dep.edgeType == DependencyType::POINTER)
				classesAdded += ProcessDependencies(&dep, nodeStatus, forwardDeclarations, classDefinitions);
		}
		classDefinitions.push_back(node->GetCClass());
		nodeStatus[node] = NodeGenerationStatus::PROCESSED;
		classesAdded += 1;
	}
	return classesAdded;
}

std::string ClassDependencyGraph::ToDot(std::string graphLabel) {
	std::stringstream stream;
	stream << "digraph class_dependency {";
	for (auto node : this->nodes) {
		for (auto edge : *node.second.GetDependencies()) {
			stream << "\"";
			stream << CT2CA(node.first->GetName());
			stream << "\"";
			stream << " -> ";
			stream << "\"";
			stream << CT2CA(edge.dependency->GetCClass()->GetName());
			stream << "\"";
			if (edge.edgeType == DependencyType::POINTER) {
				stream << " [style=dotted]";
			}
			stream << ";";
			stream << "\n";
		}
	}
	stream << "}";
	return stream.str();
}

int ClassDependencyGraph::OrderClassesForGeneration(std::set<const CNodeClass*>& forwardDeclarations, std::vector<const CNodeClass*>& classDefinitions) {
	int classesAdded = 0;
	std::map<const DependencyNode*, NodeGenerationStatus> nodeStatus;
	std::vector<DependencyNode*> leafNodes			= this->GetLeafNodes();
	std::vector<DependencyNode*> instancedNodes		= this->GetInstanceNodes();

	for (auto node : instancedNodes) {
		nodeStatus[node] = NodeGenerationStatus::INSTANCED;
	}

	for (auto leaf : leafNodes) {
		for (auto dep : *leaf->GetDependencies()) {
			classesAdded += ProcessDependencies(&dep, nodeStatus, forwardDeclarations, classDefinitions);
		}
		classDefinitions.push_back(leaf->GetCClass());
		classesAdded += 1;
	 }

	 return classesAdded;
 }

