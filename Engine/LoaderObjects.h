#pragma once
#include "./engine/object/Object.h"




//Примитивы 

Object* ObjSphere(GLuint mode, glm::vec3 position, float radius, Texture* texture, Texture* specularTexture) {
	Model* model = new Model();
	model->sizeBrush = 5.0;


	model->getMesh()->loadModelFromFile("./model/Sphere.obj");
	model->getMaterial()->loadColor({ 0,0,1 });
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	//model->getMaterial()->loadMaterialSettings(matset);
	model->getMaterial()->loadTextures(texture, specularTexture);
	model->setMode(mode);

	Transform* transform = new Transform(position, 0, { 0,1,0 }, { radius,radius,radius });

	Object* obj = new Object(model, transform, "Lol it is object sphere");


	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/sphere.obj");
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);
	obj->getRigidbody()->info.isStatic = false;
	obj->getRigidbody()->info.dynamicFriction = 10;
	obj->getRigidbody()->info.staticFriction = 60;
	obj->getRigidbody()->info.restitution = 4;
	obj->getRigidbody()->info.mass = 0.1;
	return obj;
}

Object* ObjCube(GLuint mode, glm::vec3 position, float radius, Texture* texture, Texture* specularTexture) {
	Model* model = new Model();
	model->sizeBrush = 5.0;


	model->getMesh()->loadModelFromFile("./model/Cube.obj");
	model->getMaterial()->loadColor({ 0,0,1 });
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	//model->getMaterial()->loadMaterialSettings(matset);
	model->getMaterial()->loadTextures(texture, specularTexture);
	model->setMode(mode);

	Transform* transform = new Transform(position, 0, { 0,1,0 }, { radius,radius,radius });

	Object* obj = new Object(model, transform, "Lol it is object cube");

	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/Cube.obj");
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);
	obj->getRigidbody()->info.isStatic = false;
	obj->getRigidbody()->info.mass = radius;
	return obj;
}

Object* ObjPlane(GLuint mode, glm::vec3 position, float size, Texture* texture, Texture* specularTexture) {
	Model* model = new Model();
	model->sizeBrush = 5.0;


	model->getMesh()->loadModelFromFile("./model/Plane.obj");
	model->getMaterial()->loadColor({ 0,0,1 });
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	//model->getMaterial()->loadMaterialSettings(matset);
	model->getMaterial()->loadTextures(texture, specularTexture);
	model->setMode(mode);

	Transform* transform = new Transform(position, 0, { 0,1,0 }, { size,1,size });
	Object* obj = new Object(model, transform, "Lol it is object plane");

	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/Plane.obj");
	obj->getCollision()->getCollider()->settings.scale = { 1, 1, 1 };
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);
	obj->getRigidbody()->info.isStatic = false;
	return obj;
}

//Боты и игрок
Object* ObjCapy(GLuint mode, glm::vec3 position, glm::vec3 rotation = {0,1,0}, float angle =0) {
	Model* model = new Model();

	Texture* texture = new Texture("./texture/animals/capy.png");

	model->getMesh()->loadModelFromFile("./model/game/capy.obj");
	model->getMaterial()->loadColor({ 0,0,1 });
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	//model->getMaterial()->loadMaterialSettings(matset);
	model->getMaterial()->loadTextures(texture, nullptr);
	model->setMode(mode);

	Transform* transform = new Transform(position, -90 + angle, rotation, { 1,1,1 });

	Object* obj = new Object(model, transform, "Lol it is Player");

	obj->getCollision()->getCollider()->settings.scale = { 1.1f,1.f,0.5f };
	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/Cube.obj");
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);
	obj->getRigidbody()->info.isStatic = false;

	delete texture;
	return obj;
}
//Земля
Object* ObjTerrain(glm::vec3 position, float size) {

	Texture* texture = new Texture("./texture/Terrain.png");

	Model* model = new Model();
	model->getMesh()->loadModelFromFile("./model/game/terrain.obj");
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	model->getMaterial()->loadTextures(texture, nullptr);

	Transform* transform = new Transform(position, 0, { 0,1,0 }, { size,size,size});
	Object* obj = new Object(model, transform, "Lol it is object Terrain");

	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/game/terrain.obj");
	obj->getCollision()->getCollider()->settings.scale = { 1, 1, 1 };
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);

	delete texture;
	return obj;
}

//Вода
Object* ObjWater(glm::vec3 position, float size) {

	Texture* texture = new Texture("./texture/clear/water.jpg");

	Model* model = new Model();
	model->getMesh()->loadModelFromFile("./model/game/water.obj");
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	model->getMaterial()->loadTextures(texture, nullptr);

	Transform* transform = new Transform(position, 0, { 0,1,0 }, { size,size,size });
	Object* obj = new Object(model, transform, "Lol it is object Water");

	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/Cube.obj");
	obj->getCollision()->getCollider()->settings.offset = { 1, -10, 1 };
	obj->getCollision()->getCollider()->settings.scale = { 200, 10, 200 };
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);
	obj->getRigidbody()->info.isSupportReaction = false;
	delete texture;
	return obj;
}

//Деревья ++++++++++++++++++++++++++++++++++++++++++++++++++++

Object* ObjDiedTree(glm::vec3 position, float size, glm::vec3 rotation = { 0,1,0 }, float angle = 0) {

	Texture* texture = new Texture("./texture/Tree.png");

	Model* model = new Model();
	model->getMesh()->loadModelFromFile("./model/game/died_tree.obj");
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	model->getMaterial()->loadTextures(texture, nullptr);

	Transform* transform = new Transform(position, angle, rotation, { size/2,size/ 2,size/ 2 });
	Object* obj = new Object(model, transform, "Lol it is object Died Tree");

	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/Cube.obj");
	obj->getCollision()->getCollider()->settings.scale = { 2, 4, 2 };
	obj->getCollision()->getCollider()->settings.offset = { 0,4,0 };
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);

	delete texture;
	return obj;
}


Object* ObjTree1(glm::vec3 position, float size, glm::vec3 rotation = { 0,1,0 }, float angle  = 0) {

	Texture* texture = new Texture("./texture/Tree.png");

	Model* model = new Model();
	model->getMesh()->loadModelFromFile("./model/game/tree1.obj");
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	model->getMaterial()->loadTextures(texture, nullptr);

	Transform* transform = new Transform(position, angle, rotation, { size / 2,size / 2,size / 2 });
	Object* obj = new Object(model, transform, "Lol it is object Tree1");

	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/Cube.obj");
	obj->getCollision()->getCollider()->settings.scale = { 2, 4, 2 };
	obj->getCollision()->getCollider()->settings.offset = { 0,4,0 };
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);

	delete texture;
	return obj;
}



Object* ObjTree2(glm::vec3 position, float size, glm::vec3 rotation = { 0,1,0 }, float angle  = 0) {

	Texture* texture = new Texture("./texture/Tree.png");

	Model* model = new Model();
	model->getMesh()->loadModelFromFile("./model/game/tree2.obj");
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	model->getMaterial()->loadTextures(texture, nullptr);

	Transform* transform = new Transform(position, angle, rotation, { size / 2,size / 2,size / 2 });
	Object* obj = new Object(model, transform, "Lol it is object Tree2");

	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/Cube.obj");
	obj->getCollision()->getCollider()->settings.scale = { 2, 4, 2 };
	obj->getCollision()->getCollider()->settings.offset = { 0,4,0 };
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);

	delete texture;
	return obj;
}


//Дома
Object* ObjHouse1(glm::vec3 position, float size, glm::vec3 rotation = { 0,1,0 }, float angle = 0) {

	Texture* texture = new Texture("./texture/house/house1.png");

	Model* model = new Model();
	model->getMesh()->loadModelFromFile("./model/game/house1.obj");
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	model->getMaterial()->loadTextures(texture, nullptr);

	Transform* transform = new Transform(position, angle, rotation, { size / 2,size / 2,size / 2 });
	Object* obj = new Object(model, transform, "Lol it is object House1");

	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/Cube.obj");
	obj->getCollision()->getCollider()->settings.scale = { 11, 10, 8 };
	obj->getCollision()->getCollider()->settings.offset = { 1,5,0 };
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);

	delete texture;
	return obj;
}


Object* ObjHouse2(glm::vec3 position, float size, glm::vec3 rotation = { 0,1,0 }, float angle = 0) {

	Texture* texture = new Texture("./texture/house/house2.png");

	Model* model = new Model();
	model->getMesh()->loadModelFromFile("./model/game/house2.obj");
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	model->getMaterial()->loadTextures(texture, nullptr);

	Transform* transform = new Transform(position, angle, rotation, { size/2,size/2,size/2});
	Object* obj = new Object(model, transform, "Lol it is object House2");

	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/Cube.obj");
	obj->getCollision()->getCollider()->settings.scale = { 8, 10, 11 };
	obj->getCollision()->getCollider()->settings.offset = { 0,5,-1 };
	obj->getCollision()->setWork(true);
	//obj->getCollision()->setDraw(true);

	delete texture;
	return obj;
}

//Объекты
Object* ObjUmbrela(glm::vec3 position, float size, glm::vec3 rotation = { 0,1,0 }, float angle = 0) {

	Texture* texture = new Texture("./texture/objects/umbrela.png");

	Model* model = new Model();
	model->getMesh()->loadModelFromFile("./model/game/umbrela.obj");
	model->getMaterial()->loadShaders("./shader/vs.vert", "./shader/fs.frag");
	model->getMaterial()->loadTextures(texture, nullptr);

	Transform* transform = new Transform(position, angle, rotation, { size,size,size });
	Object* obj = new Object(model, transform, "Lol it is object Umbrela");
	/*
	obj->getCollision()->getCollider()->loadColliderModelFromFile("./model/Cube.obj");
	obj->getCollision()->getCollider()->settings.scale = { 1, 1, 1 };
	obj->getCollision()->getCollider()->settings.offset = { 0,0.5,0 };
	obj->getCollision()->setWork(true);
	obj->getCollision()->setDraw(true);
	*/
	delete texture;
	return obj;
}