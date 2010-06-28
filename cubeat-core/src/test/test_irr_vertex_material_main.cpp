#include <irrlicht.h>
#include <iostream>

using namespace irr;

void createMyMesh(scene::SMesh& meshorig) {
    using namespace core;
    using namespace video;
    using namespace scene;
    S3DVertex vertices[4];
    u16 indices[6] = {0,2,3,3,1,0};

    vertices[0].Color = video::SColor(255,255,255,255);
    vertices[1].Color = video::SColor(255,255,255,255);
    vertices[2].Color = video::SColor(255,255,255,255);
    vertices[3].Color = video::SColor(255,255,255,255);

    vertices[0].Pos = core::vector3df( 0,-10, 0);
    vertices[1].Pos = core::vector3df(10,-10, 0);
    vertices[2].Pos = core::vector3df( 0,  0, 0);
    vertices[3].Pos = core::vector3df(10,  0, 0);

    vertices[0].Normal = core::vector3df(0,0,-1);
    vertices[1].Normal = core::vector3df(0,0,-1);
    vertices[2].Normal = core::vector3df(0,0,-1);
    vertices[3].Normal = core::vector3df(0,0,-1);

    vertices[0].TCoords = core::vector2df(0,1);
    vertices[1].TCoords = core::vector2df(1,1);
    vertices[2].TCoords = core::vector2df(0,0);
    vertices[3].TCoords = core::vector2df(1,0);

    SMeshBuffer* buf = new SMeshBuffer();
	buf->append(vertices, 4, indices, 6);
	meshorig.addMeshBuffer( buf );
	buf->drop(); //the addMeshBuffer method will grab it, so we can drop this ptr.
}

int main()
{
	// create device
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
			core::dimension2d<u32>(640, 480), 16, false);

	if (device == 0) {
	    std::cout << "Could not create OpenGL driver." << std::endl;
	    return 1;
	}

	// create engine and camera
	device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	smgr->addCameraSceneNode(0, core::vector3df(0,0,-50), core::vector3df(0,0,0));
	smgr->addLightSceneNode(0, core::vector3df(0,0,-50), video::SColor(255, 255, 255, 255), 1000);

	scene::SMesh meshorig;
	createMyMesh(meshorig);
    scene::IMesh* mesh = &meshorig;
    scene::ISceneNode* myNode = smgr->addMeshSceneNode(mesh, smgr->getRootSceneNode(), -1, core::vector3df(0,0,0));
    myNode->setMaterialTexture( 0, driver->getTexture("rc/texture/circle.png") );

    //Now we start to manipulate the material of this scene node
    video::SMaterial& mat = myNode->getMaterial(0);
    mat.setFlag(video::EMF_LIGHTING, true);
    mat.setFlag(video::EMF_ZWRITE_ENABLE, false);
    mat.setFlag(video::EMF_NORMALIZE_NORMALS, true);
    mat.MaterialType = video::EMT_TRANSPARENT_MODULATE;
    mat.MaterialTypeParam = 0.01f;
    mat.DiffuseColor.set(128, 255, 0, 0);

	scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.2f, 0, 0));

	if(anim) {
		myNode->addAnimator(anim);
		anim->drop();
		anim = 0;
	}

	u32 frames=0;
	while(device->run()) {
		driver->beginScene(true, true, video::SColor(0,100,100,100));
		smgr->drawAll();
		driver->endScene();

		if (++frames==100) {
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames=0;
		}
	}
	device->drop();
	return 0;
}
