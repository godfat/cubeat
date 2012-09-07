#ifndef _SHOOTING_CUBES_ACCESSORS_
#define _SHOOTING_CUBES_ACCESSORS_

#include <cmath>
#include "Accessor_proto.hpp"
#include "data/BasicViewTypes.hpp"
#include "IrrDevice.hpp"

namespace psc {
namespace accessor {

    struct Pos3D : Accessor<vec3, AT::POSITION>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            node->setPosition(val);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            out = node->getPosition();
        }
    };

    struct Pos2D : Accessor<vec2, AT::POSITION>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            vec3 pos(val.X, -val.Y, node->getPosition().Z);
            node->setPosition( pos );
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            vec3 pos(node->getPosition());
            out = value_type(pos.X, -pos.Y);
        }
    };

    struct PosX : Accessor<float, AT::X>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            vec3 pos(node->getPosition());
            pos.X = val;
            node->setPosition(pos);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out ) {
            out = node->getPosition().X;
        }
    };

    //I had to assume when you want to access Position Y individually, it will most likely be a 2D use case.
    //So it's NEGATIVE val instead of val;
    struct PosY : Accessor<float, AT::Y>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            vec3 pos(node->getPosition());
            pos.Y = -val;
            node->setPosition(pos);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out ) {
            out = node->getPosition().Y;
        }
    };

    struct Rotation : Accessor<vec3, AT::ROTATION>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            node->setRotation(val);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            out = node->getRotation();
        }
    };

    struct Scale : Accessor<vec3, AT::SCALE>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            node->setScale(val);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            out = node->getScale();
        }
    };

    struct ColorDiffuseVec3 : Accessor<vec3, AT::DIFFUSE>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            for( unsigned int i=0; i < node->getMaterialCount(); ++i ) {    //IMPORTANT NOTE!
                node->getMaterial(i).DiffuseColor.setRed(static_cast<int>(val.X)%256);
                node->getMaterial(i).DiffuseColor.setGreen(static_cast<int>(val.Y)%256);
                node->getMaterial(i).DiffuseColor.setBlue(static_cast<int>(val.Z)%256);
            }
        }
        static void get(irr::scene::ISceneNode* node, value_type& out ) {
            if( node->getMaterialCount() ) {
                out.X = static_cast<float>(node->getMaterial(0).DiffuseColor.getRed());
                out.Y = static_cast<float>(node->getMaterial(0).DiffuseColor.getGreen());
                out.Z = static_cast<float>(node->getMaterial(0).DiffuseColor.getBlue());
            }
        }
    };

    struct ColorDiffuse : Accessor<int, AT::DIFFUSE>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            for( unsigned int i=0; i < node->getMaterialCount(); ++i )      //IMPORTANT NOTE!
                node->getMaterial(i).DiffuseColor.set( val );
        }
        static void get(irr::scene::ISceneNode* node, value_type& out ) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.color;
        }
    };

    struct GradientDiffuse : Accessor<int, AT::DIFFUSE>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            for( unsigned int i=0; i < node->getMaterialCount(); ++i ) {    //IMPORTANT NOTE!
                node->getMaterial(i).DiffuseColor.setRed(val%256);
                node->getMaterial(i).DiffuseColor.setGreen(val%256);
                node->getMaterial(i).DiffuseColor.setBlue(val%256);
            }
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getAverage();
        }
    };

    struct GradientEmissive : Accessor<int, AT::EMISSIVE>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() ) {
                node->getMaterial(0).EmissiveColor.setRed(val%256);
                node->getMaterial(0).EmissiveColor.setGreen(val%256);
                node->getMaterial(0).EmissiveColor.setBlue(val%256);
            }
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).EmissiveColor.getAverage();
        }
    };

    struct Red : Accessor<int, AT::R>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setRed(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getRed();
        }
    };

    struct Green : Accessor<int, AT::G>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setGreen(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getGreen();
        }
    };

    struct Blue : Accessor<int, AT::B>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setBlue(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getBlue();
        }
    };

    struct RedE : Accessor<int, AT::RE>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).EmissiveColor.setRed(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).EmissiveColor.getRed();
        }
    };

    struct GreenE : Accessor<int, AT::GE>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).EmissiveColor.setGreen(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).EmissiveColor.getGreen();
        }
    };

    struct BlueE : Accessor<int, AT::BE>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).EmissiveColor.setBlue(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).EmissiveColor.getBlue();
        }
    };

    struct Alpha : Accessor<int, AT::A>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setAlpha(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getAlpha();
        }
    };

    struct Frame : Accessor<float, AT::FRAME>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getType() == irr::scene::ESNT_ANIMATED_MESH )
                static_cast<irr::scene::IAnimatedMeshSceneNode*>(node)->setCurrentFrame( val );
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            if( node->getType() == irr::scene::ESNT_ANIMATED_MESH )
                out = static_cast<irr::scene::IAnimatedMeshSceneNode const*>(node)->getFrameNr();
        }
    };

    struct Visible : Accessor<bool, AT::VISIBLE>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            node->setVisible(val);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            out = node->isVisible();
        }
    };

    struct GroupID : Accessor<int, AT::ID>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            node->setID(val);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            out = node->getID();
        }
    };

    struct Size2D : Accessor<vec2, AT::SIZE2D>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            vec2 ori;
            get(node, ori);
            irr::scene::IMeshManipulator* mani =
                IrrDevice::i().d()->getSceneManager()->getMeshManipulator();
            mani->scaleMesh( static_cast<irr::scene::IMeshSceneNode*>(node)->getMesh(),
                vec3(val.X / ori.X, val.Y / ori.Y, 1) );
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            irr::core::aabbox3df box =
                static_cast<irr::scene::IMeshSceneNode*>(node)->getMesh()->getBoundingBox();
            out.X = (box.MaxEdge.X - box.MinEdge.X);
            out.Y = (box.MaxEdge.Y - box.MinEdge.Y);
        }
    };

    //This only work with 2D objects like view::Sprite, and it won't have hierarchical effects.
    //it will go wrong with objects using hand-made textures
    //so don't use this with SpriteText and SpriteMovie
    struct ScaleWithUV : Accessor<vec2, AT::SCALE>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            vec2 ori;
            get(node, ori);
            irr::scene::IMeshManipulator* mani =
                IrrDevice::i().d()->getSceneManager()->getMeshManipulator();
            irr::scene::IMesh* mesh = static_cast<irr::scene::IMeshSceneNode*>(node)->getMesh();
            mani->scaleMesh( mesh, vec3(val.X / ori.X, val.Y / ori.Y, 1) );

            irr::video::S3DVertex* ptr = static_cast<irr::video::S3DVertex*>(
                mesh->getMeshBuffer(0)->getVertices());

            /*
                2___3
                |  /|
                | / |    <-- plane mesh is like this, point 2 is (0,0), point 0 is (0, -1)
                |/  |    <-- the texture coords of point 2 is (0,0, point 0 is (0, 1)
                0---1
            */

            if( ptr[2].Pos.X < 0 ) { //means this mesh is center-aligned
                if( ptr[2].TCoords.X > ptr[3].TCoords.X ) { // means this mesh's texture was flipped horizontally once.
                    ptr[2].TCoords.X = ptr[0].TCoords.X = 0.5 + (val.X/2);
                    ptr[3].TCoords.X = ptr[1].TCoords.X = 0.5 - (val.X/2);
                }
                else {
                    ptr[2].TCoords.X = ptr[0].TCoords.X = 0.5 - (val.X/2);
                    ptr[3].TCoords.X = ptr[1].TCoords.X = 0.5 + (val.X/2);
                }
                if( ptr[2].TCoords.Y > ptr[0].TCoords.Y ) { // means this mesh's texture was flipped vertically once.
                    ptr[2].TCoords.Y = ptr[3].TCoords.Y = 0.5 + (val.Y/2);
                    ptr[0].TCoords.Y = ptr[1].TCoords.Y = 0.5 - (val.Y/2);
                }
                else {
                    ptr[2].TCoords.Y = ptr[3].TCoords.Y = 0.5 - (val.Y/2);
                    ptr[0].TCoords.Y = ptr[1].TCoords.Y = 0.5 + (val.Y/2);
                }
            }
            else { //means this mesh is NOT center-aligned
                if( ptr[2].TCoords.X > 0 ) // means this mesh's texture was flipped horizontally once.
                    ptr[2].TCoords.X = ptr[0].TCoords.X = val.X;
                else
                    ptr[3].TCoords.X = ptr[1].TCoords.X = val.X;

                if( ptr[2].TCoords.Y > 0 ) // means this mesh's texture was flipped vertically once.
                    ptr[2].TCoords.Y = ptr[3].TCoords.Y = val.Y;
                else
                    ptr[0].TCoords.Y = ptr[1].TCoords.Y = val.Y;
            }
        }
        static void get(irr::scene::ISceneNode* node, value_type& out ) {
            irr::scene::IMesh* mesh = static_cast<irr::scene::IMeshSceneNode*>(node)->getMesh();
            irr::video::S3DVertex* ptr = static_cast<irr::video::S3DVertex*>(
                mesh->getMeshBuffer(0)->getVertices());

            out.X = std::abs(ptr[2].TCoords.X - ptr[3].TCoords.X);
            out.Y = std::abs(ptr[0].TCoords.Y - ptr[2].TCoords.Y);
        }
    };

}   //accessor
}   //psc

#endif
