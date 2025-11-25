#include "fallbackVisitor.hpp"

void FallbackVisitor::visit([[maybe_unused]] RegistryComposite *node) {
  // TODO: maybe log error????
}

void FallbackVisitor::visit([[maybe_unused]] SceneNode *node) {
  visit(static_cast<RegistryComposite *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] Shape *node) {
  visit(static_cast<RegistryComposite *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] Circle *node) { visit(static_cast<Shape *>(node)); }

void FallbackVisitor::visit([[maybe_unused]] Segment *node) {
  visit(static_cast<Shape *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] Capsule *node) {
  visit(static_cast<Shape *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] Polygon *node) {
  visit(static_cast<Shape *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] Joint *node) {
  visit(static_cast<RegistryComposite *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] DistanceJoint *node) {
  visit(static_cast<Joint *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] MouseJoint *node) {
  visit(static_cast<Joint *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] PrismaticJoint *node) {
  visit(static_cast<Joint *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] RevoluteJoint *node) {
  visit(static_cast<Joint *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] WeldJoint *node) {
  visit(static_cast<Joint *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] Body *node) {
  visit(static_cast<RegistryComposite *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] CircleBody *node) {
  visit(static_cast<Body *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] SegmentBody *node) {
  visit(static_cast<Body *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] CapsuleBody *node) {
  visit(static_cast<Body *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] PolygonBody *node) {
  visit(static_cast<Body *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] LimbBody *node) {
  visit(static_cast<Body *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] Connection *node) {
  visit(static_cast<RegistryComposite *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] DistanceConnection *node) {
  visit(static_cast<Connection *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] GirdleConnection *node) {
  visit(static_cast<Connection *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] RevoluteConnection *node) {
  visit(static_cast<Connection *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] WeldConnection *node) {
  visit(static_cast<Connection *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] Terrain *node) {
  visit(static_cast<SceneNode *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] CapsuleTerrain *node) {
  visit(static_cast<Terrain *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] CircleTerrain *node) {
  visit(static_cast<Terrain *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] PolygonTerrain *node) {
  visit(static_cast<Terrain *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] SegmentTerrain *node) {
  visit(static_cast<Terrain *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] Creature *node) {
  visit(static_cast<SceneNode *>(node));
}

void FallbackVisitor::visit([[maybe_unused]] DemoCreature *node) {
  visit(static_cast<Creature *>(node));
}
