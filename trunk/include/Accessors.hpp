#ifndef _SHOOTING_CUBES_ACCESSORS_
#define _SHOOTING_CUBES_ACCESSORS_

#include "Accessor_proto.hpp"
#include "data/BasicViewTypes.hpp"
#include "IrrDevice.hpp"

namespace psc {
namespace accessor {

    struct Pos3D : Accessor<vec3>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            node->setPosition(val);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            out = node->getPosition();
        }
    };

    struct Pos2D : Accessor<vec2>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            vec3 pos(val.X, -val.Y, node->getPosition().Z);
            node->setPosition( pos );
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            vec3 pos;
            Pos3D::get(node, pos);
            out = value_type(pos.X, -pos.Y);
        }
    };

    struct Rotation : Accessor<vec3>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            node->setRotation(val);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            out = node->getRotation();
        }
    };

    struct Scale : Accessor<vec3>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            node->setScale(val);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            out = node->getScale();
        }
    };

    struct ColorDiffuse : Accessor<int>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.set( val );
        }
        static void get(irr::scene::ISceneNode* node, value_type& out ) {
            return; //currently not used, and is not quite useful, hence no impl.
        }
    };

    struct GradientDiffuse : Accessor<int>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() ) {
                node->getMaterial(0).DiffuseColor.setRed(val%256);
                node->getMaterial(0).DiffuseColor.setGreen(val%256);
                node->getMaterial(0).DiffuseColor.setBlue(val%256);
            }
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getAverage();
        }
    };

    struct GradientEmissive : Accessor<int>{
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

    struct Red : Accessor<int>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setRed(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getRed();
        }
    };

    struct Green : Accessor<int>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setGreen(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getGreen();
        }
    };

    struct Blue : Accessor<int>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setBlue(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getBlue();
        }
    };

    struct Alpha : Accessor<int>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setAlpha(val);
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getAlpha();
        }
    };

    struct Frame : Accessor<float>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getType() == irr::scene::ESNT_ANIMATED_MESH )
                static_cast<irr::scene::IAnimatedMeshSceneNode*>(node)->setCurrentFrame( val );
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            if( node->getType() == irr::scene::ESNT_ANIMATED_MESH )
                out = static_cast<irr::scene::IAnimatedMeshSceneNode const*>(node)->getFrameNr();
        }
    };

    struct Visible : Accessor<bool>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            node->setVisible(val);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            out = node->isVisible();
        }
    };

    struct GroupID : Accessor<int>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            node->setID(val);
        }
        static void get(irr::scene::ISceneNode const* node, value_type& out) {
            out = node->getID();
        }
    };

    struct Size2D : Accessor<vec2>{
        static void set(irr::scene::ISceneNode* node, value_type const& val ) {
            if( node->getType() == irr::scene::ESNT_BILLBOARD )
                static_cast<irr::scene::IBillboardSceneNode*>(node)->setSize(
                    irr::core::dimension2df(val.X, val.Y) );
            else {
                vec2 ori;
                get(node, ori);
                irr::scene::IMeshManipulator* mani = IrrDevice::i().d()->getSceneManager()->getMeshManipulator();
                mani->scaleMesh(
                    static_cast<irr::scene::IMeshSceneNode*>(node)->getMesh(), vec3(val.X / ori.X, val.Y / ori.Y, 1) );
            }
        }
        static void get(irr::scene::ISceneNode* node, value_type& out) {
            if( node->getType() == irr::scene::ESNT_BILLBOARD ) {
                irr::core::dimension2df size = static_cast<irr::scene::IBillboardSceneNode*>(node)->getSize();
                out.X = size.Width; out.Y = size.Height;
            }
            else {
                irr::core::aabbox3df box =
                    static_cast<irr::scene::IMeshSceneNode*>(node)->getMesh()->getBoundingBox();
                out.X = (box.MaxEdge.X - box.MinEdge.X);
                out.Y = (box.MaxEdge.Y - box.MinEdge.Y);
            }
        }
    };

}   //accessor
}   //psc

#endif
