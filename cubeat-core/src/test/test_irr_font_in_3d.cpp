
/** Example 001 HelloWorld From Official Irrlicht Example **/
#include <iostream>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main()
{
	/*
	The most important function of the engine is the createDevice()
	function. The IrrlichtDevice is created by it, which is the root
	object for doing anything with the engine. createDevice() has 7
	parameters:

	- deviceType: Type of the device. This can currently be the Null-device,
	   one of the two software renderers, D3D8, D3D9, or OpenGL. In this
	   example we use EDT_SOFTWARE, but to try out, you might want to
	   change it to EDT_BURNINGSVIDEO, EDT_NULL, EDT_DIRECT3D8,
	   EDT_DIRECT3D9, or EDT_OPENGL.

	- windowSize: Size of the Window or screen in FullScreenMode to be
	   created. In this example we use 640x480.

	- bits: Amount of color bits per pixel. This should be 16 or 32. The
	   parameter is often ignored when running in windowed mode.

	- fullscreen: Specifies if we want the device to run in fullscreen mode
	   or not.

	- stencilbuffer: Specifies if we want to use the stencil buffer (for
	   drawing shadows).

	- vsync: Specifies if we want to have vsync enabled, this is only useful
	   in fullscreen mode.

	- eventReceiver: An object to receive events. We do not want to use this
	   parameter here, and set it to 0.

	Always check the return value to cope with unsupported drivers,
	dimensions, etc.
	*/
	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<u32>(800, 600), 32,
			false, false, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);
    smgr->addLightSceneNode( smgr->getRootSceneNode(), vector3df(0,0,-40), SColor(255,255,255,255));

	IAnimatedMesh* mesh = smgr->getMesh("rc/model/test_cube.x");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	if (node) {
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("rc/texture/circle.png") );
	}
	smgr->addCameraSceneNode(0, vector3df(0,-30,-80), vector3df(0,5,0));
    ISceneNode* textparent = smgr->addEmptySceneNode();
    textparent->setPosition(vector3df(0,40,80));
    IGUITTFont* font = guienv->getFont("rc/fonts/Star Jedi.ttf", 32, true);
    font->setBatchLoadSize(64);
    font->setMaxPageTextureSize(dimension2du(512,512));
    font->addTextSceneNode(L"\"star wars\"\nthe adventure\nof\ncharlie!!!!", smgr, textparent, SColor(255,255,255,0), true);

    IGUITTFont* font2= guienv->getFont("rc/fonts/Star Jedi.ttf", 24, true);
    font2->setBatchLoadSize(1);
    font2->setMaxPageTextureSize(dimension2du(512,512));
    font2->addTextSceneNode(L"123467890oiuytre\nwqasdfghjkmnbvcxz", smgr);

/* THIS PART IS USED TO DEBUG THE FONT PAGE TEXTURE */
    SMaterial m;
    m.Lighting = false;
    m.MaterialType = EMT_TRANSPARENT_MODULATE;
    m.MaterialTypeParam = 0.01f;
    m.BackfaceCulling = false;
    m.ColorMaterial = ECM_NONE;

    IBillboardSceneNode* bbb=
        smgr->addBillboardSceneNode(smgr->getRootSceneNode(), dimension2df(514, 514), vector3df(-257, 257, 385));
    bbb->getMaterial(0) = m;
    bbb->setColor(SColor(128,128,128,128));

    IBillboardSceneNode* bb =
        smgr->addBillboardSceneNode(smgr->getRootSceneNode(), dimension2df(512, 512), vector3df(-256, 256, 384));
    bb->getMaterial(0) = m;
    bb->setMaterialTexture(0, font->createTextureFromChar(0));
/* ABOVE PART IS USED TO DEBUG THE FONT PAGE TEXTURE */

	while(device->run()) {
		driver->beginScene(true, true, SColor(255,100,101,140));
		smgr->drawAll();
		guienv->drawAll();
		//font->draw(stringw(L"Hello, world!"), recti(5, 5, 200, 30), SColor(128,255,255,255), false, false);
		driver->endScene();
	}
	device->drop();
	return 0;
}
