#pragma once

struct CreatureLoadsRoomsTag {
  struct {
    float sizeX = 10;
    float sizeY = 10;
  } loadBorder;
  struct {
    float sizeX = 30;
    float sizeY = 30;
  } unloadBorder;
  struct {
    float sizeX = 100;
    float sizeY = 100;
  } destroyBorder;
};