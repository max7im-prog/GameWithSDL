#pragma once

#include <box2d/box2d.h>
#include <vector>

class KinematicUtils {
public:
  // Solves an inverse kinematic problem with a FABRIK method
  // points - old locations of joints
  static std::vector<b2Vec2> solveFABRIK(b2Vec2 fixture, b2Vec2 target,
                                         const std::vector<b2Vec2> &points,
                                         float marginMtr = 0.01,
                                         size_t maxIter = 10);

  // Solves an inverse kinematic problem with a FABRIK method
  // This version of a function theoretically works better as it does not
  // compute the length of limbs on each step points - old locations of joints
  static std::vector<b2Vec2> solveFABRIK(b2Vec2 fixture, b2Vec2 target,
                                         const std::vector<b2Vec2> &points,
                                         const std::vector<float> &lengths,
                                         float marginMtr = 0.01,
                                         size_t maxIter = 10);

  // Returns angles in each joint of a sequence. ret[0] equals to the angle
  // between x+ axis and a first segment. After that, values are angles between
  // 2 consecutive segments
  static std::vector<float> getAngles(const std::vector<b2Vec2> &pos);

private:
};

class IKTask {
public:
  enum class IKTaskParams : uint32_t {
    CONSTRAIN_ANGLE = 1,
  };
  struct AngleConstraint {
    float minRot = -B2_PI;
    float maxRot = B2_PI;
  };

  std::vector<float> lengths = {};
  std::vector<b2Vec2> previousJoints = {};
  std::vector<AngleConstraint> angleConstraints = {};
  b2Vec2 targetPoint = {0, 0};
  b2Vec2 fixturePoint = {0, 0};
  b2Rot rootRot = b2MakeRot(0);
  size_t maxIterations = 10;
  float margin = 0.01;
  std::vector<b2Vec2> solveFABRIK(uint32_t params);

private:
  std::vector<b2Vec2> joints = {};
  float totalLength = 0;
  size_t numLengths = 0;
  size_t numJoints = 0;
  void forwardPass(uint32_t params);
  void backwardPass(uint32_t params);
  void solveFar(uint32_t params);
};

inline b2Rot rotToPoint(b2Vec2 from, b2Vec2 to) {
  b2Vec2 vecToPoint = b2Normalize(b2Sub(to, from));
  return {.c = vecToPoint.x,.s = vecToPoint.y};
}

inline b2Rot clampRot(b2Rot rot,float minAngle, float maxAngle){
  float angle = b2Rot_GetAngle(rot);
  angle = b2ClampFloat(angle,minAngle,maxAngle);
  return b2MakeRot(angle);
}