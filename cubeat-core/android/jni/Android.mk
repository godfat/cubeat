LOCAL_PATH := $(call my-dir)/../..
PROJECT_PATH := $(LOCAL_PATH)/android

include $(CLEAR_VARS)

# copy rc data to assets
$(shell mkdir assets)
$(shell mkdir assets/rc)
$(shell cp    $(LOCAL_PATH)/config_script.zip $(PROJECT_PATH)/assets/)
$(shell cp -r $(LOCAL_PATH)/rc/fonts $(PROJECT_PATH)/assets/rc/)
$(shell cp -r $(LOCAL_PATH)/rc/shader $(PROJECT_PATH)/assets/rc/)
$(shell cp -r $(LOCAL_PATH)/rc/sound $(PROJECT_PATH)/assets/rc/)
$(shell cp -r $(LOCAL_PATH)/rc/texture $(PROJECT_PATH)/assets/rc/)

LOCAL_MODULE := CuBeat

LOCAL_CFLAGS := -D_SHOOTING_CUBES_ANDROID_ \
                -DUSE_FILE32API \
                -DBOOST_DISABLE_ASSERTS \
                -DALMIXER_COMPILE_WITHOUT_SDL \
                -D_SHOOTING_CUBES_ENABLE_SOUND_ \
                -pipe -fstrict-aliasing -fno-fast-math

# -fno-exceptions -fno-rtti can't be used with boost
# USE_FILE32API is important for minizip on Android

ifndef NDEBUG
LOCAL_CFLAGS += -g -D_DEBUG -DDEBUG
else
LOCAL_CFLAGS += -O1 -Os -s -DNDEBUG
endif

ifeq ($(TARGET_ARCH_ABI),x86)
LOCAL_CFLAGS += -fno-stack-protector
endif

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/lib
LOCAL_C_INCLUDES += c:/local_gitrepo/irrlicht-svn/include
LOCAL_C_INCLUDES += $(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/include
LOCAL_C_INCLUDES += $(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a/include

LOCAL_SRC_FILES := \
  lib/Singleton.cpp \
  src/audio/detail/ALmixer.cpp \
  src/audio/Sound.cpp \
  src/audio/SoundObject.cpp \
  src/ctrl/AIPlayer.cpp \
  src/ctrl/TimerDispatcher.cpp \
  src/data/Color.cpp \
  src/data/MapSetting.cpp \
  src/data/ViewSetting.cpp \
  src/model/AICommand.cpp \
  src/model/Cube.cpp \
  src/model/Map.cpp \
  src/model/SimpleCube.cpp \
  src/model/SimpleMap.cpp \
  src/model/detail/MapUtils.cpp \
  src/model/detail/AIUtils.cpp \
  src/model/state/Dropping.cpp \
  src/model/state/Dying.cpp \
  src/model/state/State.cpp \
  src/model/state/Sinking.cpp \
  src/model/state/Waiting.cpp \
  src/presenter/cube/ViewSprite.cpp \
  src/presenter/cube/ViewSpriteMaster.cpp \
  src/presenter/game/Demo.cpp \
  src/presenter/game/Multi.cpp \
  src/presenter/game/Puzzle.cpp \
  src/presenter/Cube.cpp \
  src/presenter/MainMenu.cpp \
  src/presenter/Map.cpp \
  src/presenter/Object.cpp \
  src/presenter/Opening2.cpp \
  src/presenter/OpeningSequence.cpp \
  src/presenter/PlayerAbility.cpp \
  src/presenter/PlayerView.cpp \
  src/presenter/Stage.cpp \
  src/presenter/Stage2.cpp \
  src/presenter/Transitioner.cpp \
  src/private/MastEventReceiver.cpp \
  src/script/ai/bindings.cpp \
  src/script/demo/bindings.cpp \
  src/script/event/bindings.cpp \
  src/script/ui/puzzle/bindings.cpp \
  src/script/ui/bindings.cpp \
  src/script/ui/tween_call.cpp \
  src/script/lua_utility.cpp \
  src/utils/GaussLegendre.cpp \
  src/utils/Logger.cpp \
  src/utils/Timer.cpp \
  src/utils/dictionary.cpp \
  src/view/AnimatedSceneObject.cpp \
  src/view/AnimatedSprite.cpp \
  src/view/Cube.cpp \
  src/view/Menu.cpp \
  src/view/Object.cpp \
  src/view/SFX.cpp \
  src/view/Scene.cpp \
  src/view/SceneObject.cpp \
  src/view/Sprite.cpp \
  src/view/SpriteText.cpp \
  src/App.cpp \
  src/Button_vc.cpp \
  src/CallbackDelegate.cpp \
  src/Conf.cpp \
  src/Crosshair.cpp \
  src/EventDispatcher.cpp \
  src/Input.cpp \
  src/IrrDevice.cpp \
  src/Player.cpp \
  src/Weapon.cpp \
  android/ioapi.c \
  android/unzip.c \
  android/unpack_data.cpp \
  main.cpp

LOCAL_LDLIBS := -Lc:/local_gitrepo/irrlicht-svn/lib/Android -lEGL -llog -lGLESv1_CM -lz -lGLESv2 -landroid -lIrrlicht
LOCAL_LDLIBS += $(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a/libgnustl_static.a

LOCAL_STATIC_LIBRARIES := android_native_app_glue boost-thread openal
LOCAL_SHARED_LIBRARIES := luajit2 almixer
LOCAL_WHOLE_STATIC_LIBRARIES := freetype

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,boost-thread)
$(call import-module,luajit2)
$(call import-module,openal)
$(call import-module,almixer)
$(call import-module,freetype)
