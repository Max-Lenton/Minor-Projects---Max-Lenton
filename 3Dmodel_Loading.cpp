#include <raylib.h>
#include <rlgl.h>

int main() {
    InitWindow(1399, 1200, "Utah Teapot");
    SetTargetFPS(100);
    // Define the camera
    Camera3D camera = { 0 };
    camera.position = { 9.0f, 3.0f, 5.0f };  // Camera position
    camera.target = { 0.0f, 1.5f, 0.0f };    // Looking at teapot center (raised)
    camera.up = { 0.0f, 1.0f, 0.0f };        // Y is up
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Load the 3Dmodel
    Model model = LoadModel("3Dmodels/UTAH_BLEND.obj"); 
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = WHITE;
    
    // Ground
    Texture2D grassTexture = LoadTexture("Textures/images.jpg");
    SetTextureWrap(grassTexture, TEXTURE_WRAP_REPEAT);
    Mesh groundMesh = GenMeshPlane(1000.0f, 1000.0f, 1, 1);
    float textureRepeat = 100.0f;  //tile the texture
    for (int i = 0; i < groundMesh.vertexCount * 2; i++) {
        groundMesh.texcoords[i] *= textureRepeat;
    }
    UpdateMeshBuffer(groundMesh, 1, groundMesh.texcoords, groundMesh.vertexCount * 2 * sizeof(float), 0);
    
    Model groundModel = LoadModelFromMesh(groundMesh);
    groundModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grassTexture;
    
    // Load sky texture and create skybox sphere
    Texture2D skyTexture = LoadTexture("Textures/images.steamusercontent.jpg");
    Mesh skyMesh = GenMeshSphere(500.0f, 32, 32);
    Model skyModel = LoadModelFromMesh(skyMesh);
    skyModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = skyTexture;
    
    Vector3 position = { 0.0f, 0.0f, 0.0f };
    Vector3 rotationAxis = { 0.0f, 1.0f, 0.0f };  // Rotate around Y axis
    float rotationAngle = 0.0f;
    
    BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);
    while (!WindowShouldClose()) {
        // Update rotation
        rotationAngle += 1.0f;  // Degrees per frame
        if (rotationAngle >= 360.0f) rotationAngle -= 360.0f;
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(camera);
            // Draw sky sphere (disable backface culling to see inside)
            rlDisableBackfaceCulling();
            rlDisableDepthMask();
            DrawModel(skyModel, camera.position, 1.0f, WHITE);
            rlEnableDepthMask();
            rlEnableBackfaceCulling();
            
            // Draw outline: scaled-up black model, cull front faces to only show back faces as outline
            rlSetCullFace(RL_CULL_FACE_FRONT);
            DrawModelEx(model, position, rotationAxis, rotationAngle, {1.03f, 1.03f, 1.03f}, BLACK);
            rlSetCullFace(RL_CULL_FACE_BACK);
            
            // Draw the actual model on top
            DrawModelEx(model, position, rotationAxis, rotationAngle, {1.0f, 1.0f, 1.0f}, LIGHTGRAY);
            
            // Draw textured ground
            DrawModel(groundModel, {0.0f, 0.0f, 0.0f}, 1.0f, WHITE);
        EndMode3D();
        
        DrawFPS(10, 10);
        EndDrawing();
    }
    
    UnloadTexture(grassTexture);
    UnloadTexture(skyTexture);
    UnloadModel(skyModel);
    UnloadModel(groundModel);
    UnloadModel(model);  // Unload model data
    CloseWindow();
    
    return 0;
}