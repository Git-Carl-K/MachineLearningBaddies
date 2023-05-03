#include "gui.h"

#define screenWidth 1600
#define screenHeight 900

const float MOVEMENT_SPEED = 1.0f;

void initCamera(Camera* camera) {
	camera->position = (Vector3){ 0.0f, 50.0f, -60.0f };
	camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };
	camera->up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera->fovy = 100.0f;
	camera->projection = CAMERA_FREE;
}

Model createModel() {
	Model model = LoadModelFromMesh(GenMeshCube(6.0, 2.0, 2.0));
	return model;
}

void updateModel(Model* model, const float pitch, const float roll, const float yaw) {
	model->transform = MatrixRotateXYZ((Vector3){ DEG2RAD * pitch, DEG2RAD * yaw, DEG2RAD * roll });
}

void handleInput(float* pitch, float* roll, float* yaw, Vector3* position) {
	if (IsKeyDown(KEY_DOWN)) *pitch += 0.6f;
	else if (IsKeyDown(KEY_UP)) *pitch -= 0.6f;
	if (IsKeyDown(KEY_L)) *yaw -= 1.0f;
	else if (IsKeyDown(KEY_K)) *yaw += 1.0f;
	if (IsKeyDown(KEY_LEFT)) *roll -= 1.0f;
	else if (IsKeyDown(KEY_RIGHT)) *roll += 1.0f;
	if (IsKeyDown(KEY_W)) position->y -= MOVEMENT_SPEED;
	else if (IsKeyDown(KEY_S)) position->y += MOVEMENT_SPEED;
	if (IsKeyDown(KEY_R)) position->z -= MOVEMENT_SPEED;
	else if (IsKeyDown(KEY_F)) position->z += MOVEMENT_SPEED;
	if (IsKeyDown(KEY_D)) position->x += MOVEMENT_SPEED;
	else if (IsKeyDown(KEY_A)) position->x -= MOVEMENT_SPEED;
}

void draw(Model* model, Camera* camera) {
	BeginDrawing();
	ClearBackground(BLACK);
	BeginMode3D(*camera);
	DrawModelWires(*model, (Vector3){ 0.0f, 0.0f, 0.0f }, 3.02f, WHITE);
	DrawModel(*model, (Vector3){ 0.0f, 0.0f, 0.0f }, 3.0f, BLUE);
	// DrawGrid(100.0f, 10.0f);
	EndMode3D();
	EndDrawing();
}
