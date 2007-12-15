#ifndef _SHOOTING_CUBES_SETTERS_
#define _SHOOTING_CUBES_SETTERS_

#include "ISceneNode.h"

namespace irr {
namespace scene {

namespace setter {
    template <class T>
    struct Setter {
        typedef T value_type; 
    };
    struct Position : Setter<core::vector3df>{
        static void set(ISceneNode* node, value_type val ) {
            node->setPosition(val);
        }
    };
    struct Rotation : Setter<core::vector3df>{
        static void set(ISceneNode* node, value_type val ) {
            node->setRotation(val);
        }
    };
    struct Scale : Setter<core::vector3df>{
        static void set(ISceneNode* node, value_type val ) {
            node->setScale(val);
        }
    };
    struct RGB : Setter<s32>{
        static void set(ISceneNode* node, value_type val ) {
            if( !node->getMaterialCount() ) return;
            node->getMaterial(0).DiffuseColor.setRed(val%256);
            node->getMaterial(0).DiffuseColor.setGreen(val%256);
            node->getMaterial(0).DiffuseColor.setBlue(val%256);
        }
    };
    struct Red : Setter<s32>{
        static void set(ISceneNode* node, value_type val ) {
            if( !node->getMaterialCount() ) return;
            node->getMaterial(0).DiffuseColor.setRed(val);
        }
    };
    struct Green : Setter<s32>{
        static void set(ISceneNode* node, value_type val ) {
            if( !node->getMaterialCount() ) return;
            node->getMaterial(0).DiffuseColor.setGreen(val);
        }
    };
    struct Blue : Setter<s32>{
        static void set(ISceneNode* node, value_type val ) {
            if( !node->getMaterialCount() ) return;
            node->getMaterial(0).DiffuseColor.setBlue(val);
        }
    };
    struct Alpha : Setter<s32>{
        static void set(ISceneNode* node, value_type val ) {
            if( !node->getMaterialCount() ) return;
            node->getMaterial(0).DiffuseColor.setAlpha(val);
        }
    };
    struct Frame : Setter<f32>{
        static void set(ISceneNode* node, value_type val ) {
            if( node->getType() != ESNT_ANIMATED_MESH ) return;
            static_cast<IAnimatedMeshSceneNode*>(node)->setCurrentFrame( val );
        }
    };
}   //setter
}   //scene
}   //irr

#endif 