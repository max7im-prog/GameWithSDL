#include "kinematicUtils.hpp"
#include "box2d/math_functions.h"
#include <cmath>
#include <numeric>
#include <stdexcept>

std::vector<b2Vec2>
KinematicUtils::solveFABRIK(b2Vec2 fixture, b2Vec2 target,
                            const std::vector<b2Vec2> &points, float marginMtr,
                            size_t maxIter) {
  std::vector<b2Vec2> ret = points;
  if (points.size() < 2)
    return ret;

  int numPoints = points.size();
  std::vector<float> lengths(numPoints - 1);
  for (int i = 0; i < numPoints - 1; i++) {
    lengths[i] = b2Distance(points[i], points[i + 1]);
  }

  return KinematicUtils::solveFABRIK(fixture, target, points, lengths,
                                     marginMtr, maxIter);
}

std::vector<b2Vec2> KinematicUtils::solveFABRIK(
    b2Vec2 fixture, b2Vec2 target, const std::vector<b2Vec2> &points,
    const std::vector<float> &lengths, float marginMtr, size_t maxIter) {
  std::vector<b2Vec2> ret = points;
  size_t numPoints = points.size();
  if (numPoints - 1 != lengths.size()) {
    return ret;
  }

  float totalDist = 0;
  for (auto &elem : lengths) {
    totalDist += elem;
  }

  if (totalDist < b2Distance(target, fixture)) {
    // Target unreachable - point straight towards target
    b2Vec2 dir = b2Normalize(b2Sub(target, fixture));
    ret[0] = fixture;
    for (size_t i = 1; i < numPoints; i++) {
      ret[i] = b2Add(ret[i - 1], b2MulSV(lengths[i - 1], dir));
    }
  } else {
    float curMargin = b2Distance(ret.back(), target);
    for (size_t iter = 0; iter < maxIter && curMargin > marginMtr; iter++) {
      // BACKWARD
      ret.back() = target;
      for (int i = numPoints - 2; i >= 0; i--) {
        b2Vec2 dir = b2Normalize(b2Sub(ret[i], ret[i + 1]));
        ret[i] = b2Add(ret[i + 1], b2MulSV(lengths[i], dir));
      }

      // FORWARD
      ret[0] = fixture;
      for (size_t i = 1; i < numPoints; i++) {
        b2Vec2 dir = b2Normalize(b2Sub(ret[i], ret[i - 1]));
        ret[i] = b2Add(ret[i - 1], b2MulSV(lengths[i - 1], dir));
      }

      curMargin = b2Distance(ret.back(), target);
    }
  }

  return ret;
}

std::vector<float> KinematicUtils::getAngles(const std::vector<b2Vec2> &pos) {
  if (pos.size() < 2)
    return {};

  std::vector<float> ret;
  b2Vec2 lastVec = {1, 0}; // Reference direction (e.g., pointing along X axis)

  for (size_t i = 0; i < pos.size() - 1; ++i) {
    b2Vec2 curVec = b2Sub(pos[i + 1], pos[i]);

    float lastAngle = b2Atan2(lastVec.y, lastVec.x);
    float curAngle = b2Atan2(curVec.y, curVec.x);
    float angleDiff = curAngle - lastAngle;

    // Normalize angle to [-π, π]
    angleDiff = std::fmod(angleDiff + M_PI, 2.0f * M_PI);
    if (angleDiff < 0)
      angleDiff += 2.0f * M_PI;
    angleDiff -= M_PI;

    ret.push_back(angleDiff);

    lastVec = curVec;
  }

  return ret;
}

std::vector<b2Vec2> IKTask::solveFABRIK() {
  if (previousJoints.size() != lengths.size() + 1) {
    throw std::runtime_error("Size of lengths vector should be 1 less than "
                             "size of previous joints vector");
  }
  if ((lengths.size() != angleConstraints.size())) {
    throw std::runtime_error("AngleConstraint vector is of different size than "
                             "the PreviousJoints vector");
  }
  if (lengths.size() == 0) {
    throw std::runtime_error("Lengths of size 0");
  }

  joints = previousJoints;
  totalLength = std::accumulate(lengths.begin(), lengths.end(), 0.0f);
  numJoints = joints.size();
  numLengths = lengths.size();
  /*if (b2Distance(fixturePoint, targetPoint) > totalLength) {
    solveFar(params);
  } else */ // TODO optimize for far away
  {
    for (size_t i = 0;
         i < maxIterations && b2Distance(joints.back(), targetPoint) > margin;
         i++) {
      backwardPass();
      forwardPass();
    }
  }
  return joints;
}

void IKTask::backwardPass() {
  joints[numJoints - 1] = targetPoint;
  for (size_t i = numJoints - 2; i > 0; i--) {
    b2Vec2 initPoint = joints[i + 1];
    b2Vec2 l = b2MulSV(lengths[i], {1, 0});

    // Calculate root rotation
    b2Rot curRootRot;
    if (i != 0) {
      curRootRot = rotToPoint(joints[i - 1], joints[i]);
    } else {
      curRootRot = rootRot;
    }

    // Calculate clamped rotation
    b2Rot nonClampedRot =
        b2InvMulRot(curRootRot, rotToPoint(joints[i], joints[i + 1]));
    b2Rot clampedRot = clampRot(nonClampedRot, angleConstraints[i].minRot,
                                angleConstraints[i].maxRot);

    // Calculate total rotation
    b2Rot totalRot =
        b2MulRot(b2MulRot(curRootRot, clampedRot), b2MakeRot(B2_PI));

    // Increment
    b2Vec2 incr = b2RotateVector(totalRot, l);

    joints[i] = b2Add(initPoint, incr);
  }
}

void IKTask::forwardPass() {
  joints[0] = fixturePoint;
  for (size_t i = 1; i < numJoints - 1; i++) {
    b2Vec2 initPoint = joints[i - 1];
    b2Vec2 l = b2MulSV(lengths[i - 1], {1, 0});

    // Calculate root rotation
    b2Rot curRootRot;
    if (i > 1) {
      curRootRot = rotToPoint(joints[i - 2], joints[i - 1]);
    } else {
      curRootRot = rootRot;
    }

    // Calculate clamped rotation
    b2Rot nonClampedRot =
        b2InvMulRot(curRootRot, rotToPoint(joints[i - 1], joints[i]));
    b2Rot clampedRot = clampRot(nonClampedRot, angleConstraints[i - 1].minRot,
                                angleConstraints[i - 1].maxRot);

    // Calculate total rotation
    b2Rot totalRot = b2MulRot(curRootRot, clampedRot);

    // Increment
    b2Vec2 incr = b2RotateVector(totalRot, l);

    joints[i] = b2Add(initPoint, incr);
  }
}

void IKTask::solveFar() {}
