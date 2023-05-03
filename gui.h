#include "raylib.h"
#include "raymath.h"




void initCamera(Camera* camera);
Model createModel();
void updateModel(Model* model, const float pitch, const float roll, const float yaw);
void handleInput(float* pitch, float* roll, float* yaw, Vector3* position);
void draw(Model* model, Camera* camera);
