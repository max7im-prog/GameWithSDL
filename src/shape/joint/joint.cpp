#include "joint.hpp"
#include "box2d/box2d.h"

b2JointId Joint::getJointId() { return jointId; }

b2JointType Joint::getType() { return b2Joint_GetType(jointId); }

entt::entity Joint::getEntity() { return entity; }

Joint::~Joint() {
  if (b2Joint_IsValid(jointId)) {
    b2DestroyJoint(jointId);
  }

  if(entity != entt::null){
    
  }
}

Joint::Joint(){
  
}