#ifndef _SHOOTING_CUBES_ACCESSORS_
#define _SHOOTING_CUBES_ACCESSORS_

#include "Accessor_proto.hpp"
#include "ISceneNode.h"

template<class T>
void Accessor<T>::scaleWrap(irr::scene::ISceneNode const* node, irr::core::vector3df& out){
    irr::scene::ISceneNode* parent = node->getParent();
    while( parent ) {
        out *= parent->getScale();
        parent = parent->getParent();
    }
}

template<class T>
void Accessor<T>::scaleUnwrap(irr::scene::ISceneNode* node, irr::core::vector3df& out){
    irr::scene::ISceneNode* parent = node->getParent();
    while( parent ) {
        out /= parent->getScale();
        parent = parent->getParent();
    }
}

namespace irr {
namespace scene {

namespace accessor {

    struct Pos3D : Accessor<core::vector3df>{
        static void set(ISceneNode* node, value_type const& val ) {
            node->setPosition(val);
        }
        static void get(ISceneNode const* node, value_type& out) {
            out = node->getPosition();
        }
    };

    struct Pos2D : Accessor<core::vector2df>{
        static void set(ISceneNode* node, value_type const& val )
        {
            core::vector3df pos(val.X, val.Y, node->getPosition().Z);
            scaleUnwrap( node, pos );
            node->setPosition( pos );
        }
        static void get(ISceneNode const* node, value_type& out) {
            core::vector3df pos;
            Pos3D::get(node, pos);
            scaleWrap( node, pos);
            out = value_type(pos.X, pos.Y);
        }
    };

    struct Rotation : Accessor<core::vector3df>{
        static void set(ISceneNode* node, value_type const& val ) {
            node->setRotation(val);
        }
        static void get(ISceneNode const* node, value_type& out) {
            out = node->getRotation();
        }
    };

    struct Scale : Accessor<core::vector3df>{
        static void set(ISceneNode* node, value_type const& val ) {
            node->setScale(val);
        }
        static void get(ISceneNode const* node, value_type& out) {
            out = node->getScale();
        }
    };

    struct ColorDiffuse : Accessor<s32>{
        static void set(ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.set( val );
            else if( node->getType() == ESNT_TEXT )
                static_cast<ITextSceneNode*>(node)->setTextColor( val );
        }
        static void get(ISceneNode* node, value_type& out ) {
            return; //currently not used, and is not quite useful, hence no impl.
        }
    };

    struct GradientDiffuse : Accessor<s32>{
        static void set(ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() ) {
                node->getMaterial(0).DiffuseColor.setRed(val%256);
                node->getMaterial(0).DiffuseColor.setGreen(val%256);
                node->getMaterial(0).DiffuseColor.setBlue(val%256);
            }
            else if( node->getType() == ESNT_TEXT ) {
                static_cast<ITextSceneNode*>(node)->
                    setTextColor(video::SColor(255,val%256,val%256,val%256));
            }
        }
        static void get(ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getAverage();
            else if( node->getType() == ESNT_TEXT )
                out = static_cast<ITextSceneNode*>(node)->getTextColor().getAverage();
        }
    };

    struct GradientEmissive : Accessor<s32>{
        static void set(ISceneNode* node, value_type const& val ) {
            if( !node->getMaterialCount() ) return;
            node->getMaterial(0).EmissiveColor.setRed(val%256);
            node->getMaterial(0).EmissiveColor.setGreen(val%256);
            node->getMaterial(0).EmissiveColor.setBlue(val%256);
        }
        static void get(ISceneNode* node, value_type& out) {
            if( !node->getMaterialCount() ) return;
            out = node->getMaterial(0).EmissiveColor.getAverage();
        }
    };

    struct Red : Accessor<s32>{
        static void set(ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setRed(val);
            else if( node->getType() == ESNT_TEXT )
                static_cast<ITextSceneNode*>(node)->getTextColor().setRed(val);
        }
        static void get(ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getRed();
            else if( node->getType() == ESNT_TEXT )
                out = static_cast<ITextSceneNode*>(node)->getTextColor().getRed();
        }
    };

    struct Green : Accessor<s32>{
        static void set(ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setGreen(val);
            else if( node->getType() == ESNT_TEXT )
                static_cast<ITextSceneNode*>(node)->getTextColor().setGreen(val);
        }
        static void get(ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getGreen();
            else if( node->getType() == ESNT_TEXT )
                out = static_cast<ITextSceneNode*>(node)->getTextColor().getGreen();
        }
    };

    struct Blue : Accessor<s32>{
        static void set(ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setBlue(val);
            else if( node->getType() == ESNT_TEXT )
                static_cast<ITextSceneNode*>(node)->getTextColor().setBlue(val);
        }
        static void get(ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getBlue();
            else if( node->getType() == ESNT_TEXT )
                out = static_cast<ITextSceneNode*>(node)->getTextColor().getBlue();
        }
    };

    struct Alpha : Accessor<s32>{
        static void set(ISceneNode* node, value_type const& val ) {
            if( node->getMaterialCount() )
                node->getMaterial(0).DiffuseColor.setAlpha(val);
            else if( node->getType() == ESNT_TEXT )
                static_cast<ITextSceneNode*>(node)->getTextColor().setAlpha(val);
        }
        static void get(ISceneNode* node, value_type& out) {
            if( node->getMaterialCount() )
                out = node->getMaterial(0).DiffuseColor.getAlpha();
            else if( node->getType() == ESNT_TEXT )
                out = static_cast<ITextSceneNode*>(node)->getTextColor().getAlpha();
        }
    };

    struct Frame : Accessor<f32>{
        static void set(ISceneNode* node, value_type const& val ) {
            if( node->getType() != ESNT_ANIMATED_MESH ) return;
            static_cast<IAnimatedMeshSceneNode*>(node)->setCurrentFrame( val );
        }
        static void get(ISceneNode const* node, value_type& out) {
            if( node->getType() != ESNT_ANIMATED_MESH ) return;
            out = static_cast<IAnimatedMeshSceneNode const*>(node)->getFrameNr();
        }
    };

    struct Visible : Accessor<bool>{
        static void set(ISceneNode* node, value_type const& val ) {
            node->setVisible(val);
        }
        static void get(ISceneNode const* node, value_type& out) {
            out = node->isVisible();
        }
    };

    struct GroupID : Accessor<s32>{
        static void set(ISceneNode* node, value_type const& val ) {
            node->setID(val);
        }
        static void get(ISceneNode const* node, value_type& out) {
            out = node->getID();
        }
    };

    struct Size2D : Accessor<core::dimension2df>{
        static void set(ISceneNode* node, value_type const& val ) {
            if( node->getType() == ESNT_BILLBOARD )
                static_cast<IBillboardSceneNode*>(node)->setSize( val );
            else {
                core::vector3df scale(val.Width/100.0f, val.Height/100.0f, 1);
                scaleUnwrap( node, scale );
                node->setScale( scale );
            }
        }
        static void get(ISceneNode* node, value_type& out) {
            if( node->getType() == ESNT_BILLBOARD )
                out = static_cast<IBillboardSceneNode*>(node)->getSize();
            else {
                core::vector3df scale = node->getScale();
                scaleWrap( node, scale );
                core::dimension2df size2d(scale.X*100.0f, scale.Y*100.0f);
                out = size2d;
            }
        }
    };

}   //accessor
}   //scene
}   //irr

#endif
