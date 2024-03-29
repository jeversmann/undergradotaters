#pragma once
#include <llvm/Pass.h>
#include <llvm/IR/InstVisitor.h>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SetVector.h>

namespace dataflow {
using namespace llvm;

/* --------
 * flow_set
 * --------
 * a wrapper for the flow sets, it includes sets for each incoming edge in a
 * phi function so that they can be handled differently
 */
template <class T> class flow_set : public std::unordered_set<T> {
public:
  std::unordered_map<const BasicBlock *, std::unordered_set<T>> phiSets;
  bool universal;
  flow_set(bool universal = false)
      : std::unordered_set<T>(), universal(universal) {}
};

/* ----------------
 * Broadway Lattice
 * ----------------
 */
template <class T> class BroadwayLattice {
private:
  std::unordered_map<std::string, std::string> parents;
  std::unordered_map<std::string, flow_set<T>> sets;

public:
  std::set<std::string> properties; // To iterate through
  std::string initial;
  std::string name;

  BroadwayLattice() {}

  BroadwayLattice(const BroadwayLattice &other) {
    parents = std::unordered_map<std::string, std::string>(other.parents);
    sets = std::unordered_map<std::string, flow_set<T>>(other.sets);
    properties = std::set<std::string>(other.properties);
    initial = std::string(other.initial);
    name = std::string(other.name);
  }

  // Create a new set for a state
  BroadwayLattice<T> &addProperty(const std::string name,
                                  const std::string parent = "bottom") {
    parents.emplace(name, parent);
    sets[name] = flow_set<T>();
    properties.insert(name);
    return *this;
  }

  void addToProperty(const std::string stateName, const T var) {
    removeFromAll(var);
    addToParents(stateName, var);
  }

  // Add a var to a state's and its parent's sets
  void addToParents(const std::string stateName, const T var) {
    sets.at(stateName).insert(var);
    auto parent = parents[stateName];
    if (parent != "bottom") {
      addToParents(parent, var);
    }
  }

  bool isInProperty(const std::string stateName, const T var) {
    flow_set<T> property = sets.at(stateName);
    return (property.find(var) != property.end());
  }

  void removeFromProperty(const std::string stateName, const T var) {
    if (stateName != "bottom")
      sets.at(stateName).erase(var);
    std::stack<std::string> parentsToCheck;
    parentsToCheck.push(stateName);
    while (!parentsToCheck.empty()) {
      std::string parent = parentsToCheck.top();
      parentsToCheck.pop(); // I hate c++

      // Find all the children that list this as a parent
      for (auto &prop : properties) {
        if (parents.at(prop) == parent) {
          sets.at(prop).erase(var);
          parentsToCheck.push(prop); // Do its children next
        }
      }
    }
  }

  void removeFromAll(const T var) {
    for (const auto &prop : properties)
      sets.at(prop).erase(var);
  }

  // Access flow_set by property name
  flow_set<T> get(const std::string name) const { return sets.at(name); }

  void put(const std::string name, flow_set<T> set) { sets[name] = set; }

  // Returns a new object with the same parents and properties
  BroadwayLattice<T> clone() const {
    BroadwayLattice<T> resp;
    resp.parents = std::unordered_map<std::string, std::string>(parents);
    resp.properties = std::set<std::string>(properties);
    resp.name = std::string(name);
    for (auto &prop : properties)
      resp.sets[prop] = flow_set<T>();
    return resp;
  }

  bool operator==(const BroadwayLattice<T> &other) {
    if (properties != other.properties)
      return false;
    for (auto &prop : properties) {
      if (parents.at(prop) != other.parents.at(prop))
        return false;
      if (sets.at(prop) != other.sets.at(prop))
        return false;
    }
    return true;
  }

  bool operator!=(const BroadwayLattice<T> &other) {
    if (properties != other.properties)
      return true;
    for (auto &prop : properties) {
      if (parents.at(prop) != other.parents.at(prop))
        return true;
      if (sets.at(prop) != other.sets.at(prop))
        return true;
    }
    return false;
  }
};

template <class FlowValue>
BroadwayLattice<FlowValue> MeetUnion(BroadwayLattice<FlowValue> &first,
                                     BroadwayLattice<FlowValue> &second,
                                     const BasicBlock *from) {
  BroadwayLattice<FlowValue> resp = first.clone();
  // Iterate through all of the properties in the lattice
  for (auto &prop : resp.properties) {
    // take the union of the sets
    flow_set<FlowValue> result(first.get(prop));
    for (auto &e : second.get(prop))
      result.insert(e);

    // push the phi set for this node into the result set
    for (auto e : second.get(prop).phiSets[from])
      result.insert(e);

    result.universal = first.get(prop).universal || second.get(prop).universal;

    // Store the result in the response lattice
    resp.put(prop, result);
  }

  return resp;
}

template <class FlowValue>
BroadwayLattice<FlowValue> MeetIntersect(BroadwayLattice<FlowValue> &first,
                                         BroadwayLattice<FlowValue> &second,
                                         const BasicBlock *from) {
  BroadwayLattice<FlowValue> resp = first.clone();
  // Iterate through all of the properties in the lattice
  for (auto &prop : resp.properties) {
    flow_set<FlowValue> firstSet = first.get(prop);
    flow_set<FlowValue> secondSet = second.get(prop);

    flow_set<FlowValue> result(firstSet);
    result.clear();
    result.phiSets[from].clear();

    // if either is universal, just take the other one
    if (firstSet.universal) {
      result.insert(secondSet.begin(), secondSet.end());
      result.phiSets[from].insert(secondSet.phiSets[from].begin(),
                                  secondSet.phiSets[from].end());
    } else if (secondSet.universal) {
      result.insert(firstSet.begin(), firstSet.end());
      result.phiSets[from].insert(firstSet.phiSets[from].begin(),
                                  firstSet.phiSets[from].end());
    } else {
      // if neither is universal, intersect of the sets
      for (auto &e : firstSet)
        if (secondSet.find(e) != secondSet.end())
          result.insert(e);

      // intersect the phi set for this node
      for (auto e : firstSet.phiSets[from])
        if (secondSet.phiSets[from].find(e) != secondSet.phiSets[from].end())
          result.insert(e);
    }

    result.universal = firstSet.universal && secondSet.universal;

    // Store the result in the response lattice
    resp.put(prop, result);
  }

  return resp;
}

template <class FlowValue>
void printLattice(const BroadwayLattice<FlowValue> &lattice,
                  bool printPhi = false) {
  errs() << "{\n";
  for (auto &prop : lattice.properties) {
    errs() << "  " << prop << ": ";
    const auto &set = lattice.get(prop);
    printSet(set, printPhi);
  }
  errs() << "\n}\n";
}

template <class FlowValue>
void printSet(const flow_set<FlowValue> &set, bool printPhi = false) {
  errs() << "{\n    ";
  for (auto &e : set) {
    errs() << " " << e->getName();
  }
  errs() << "\n  }";

  if (printPhi) {
    errs() << " phiSets {\n";
    for (auto &p : set.phiSets) {
      errs() << "    " << p.first->getName() << ":";
      for (auto &e : p.second)
        errs() << " " << e->getName();
    }
    errs() << "  }";
  }
  errs() << "\n";
}
}
