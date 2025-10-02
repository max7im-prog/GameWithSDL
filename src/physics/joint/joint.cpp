#include "joint.hpp"
#include "box2d/box2d.h"
#include "registryComposite.hpp"

b2JointId Joint::getJointId() { return jointId; }

b2JointType Joint::getType() { return b2Joint_GetType(jointId); }


Joint::~Joint() {
  if (b2Joint_IsValid(jointId)) {
    b2DestroyJoint(jointId);
  }
}

Joint::Joint(entt::registry &registry) : RegistryComposite(registry){}


void Joint::accept(Visitor &v) { v.visit(this); }