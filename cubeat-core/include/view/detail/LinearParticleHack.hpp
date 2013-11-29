// As the name imply, this file is just a pure hack of a particle system

#ifndef _SHOOTING_CUBES_VIEW_DETAIL_PARTICLE_0_
#define _SHOOTING_CUBES_VIEW_DETAIL_PARTICLE_0_

#include "IParticleCylinderEmitter.h"
#include "irrArray.h"
#include "utils/Random.hpp"

namespace irr
{
namespace scene
{

//! A default box emitter
class LinearParticleEmitter : public IParticleEmitter
{
public:

    //! constructor
    LinearParticleEmitter(
        //const core::vector3df& center,
        core::vector3df const* vec3_to_be_followed,
        const core::vector3df& direction = core::vector3df(0.0f,0.0f,0.0f),
        u32 minParticlesPerSecond = 20,
        u32 maxParticlesPerSecond = 40,
        const video::SColor& minStartColor = video::SColor(255,0,0,0),
        const video::SColor& maxStartColor = video::SColor(255,255,255,255),
        u32 lifeTimeMin=2000,
        u32 lifeTimeMax=4000,
        s32 maxAngleDegrees=0,
        const core::dimension2df& minStartSize = core::dimension2df(5.0f,5.0f),
        const core::dimension2df& maxStartSize = core::dimension2df(5.0f,5.0f)
        ):
        Direction(direction),
        MaxStartSize(maxStartSize), MinStartSize(minStartSize),
        MinParticlesPerSecond(minParticlesPerSecond),
        MaxParticlesPerSecond(maxParticlesPerSecond),
        MinStartColor(minStartColor), MaxStartColor(maxStartColor),
        MinLifeTime(lifeTimeMin), MaxLifeTime(lifeTimeMax),
        Time(0), Emitted(0),
        MaxAngleDegrees(maxAngleDegrees),
        LinkedPositionPtr(vec3_to_be_followed),
        LastPosition(*vec3_to_be_followed)
    {
        #ifdef _DEBUG
        setDebugName("LinearParticleEmitter");
        #endif
    }

    //! Prepares an array with new particles to emitt into the system
    //! and returns how much new particles there are.
    virtual s32 emitt(u32 now, u32 timeSinceLastCall, SParticle*& outArray)
    {
        Time += timeSinceLastCall;

        using namespace psc;

        const u32 pps = (MaxParticlesPerSecond - MinParticlesPerSecond);
        const f32 perSecond = pps ? (f32)MinParticlesPerSecond + (utils::random(pps)) : MinParticlesPerSecond;
        const f32 everyWhatMillisecond = 1000.0f / perSecond;

        if(Time > everyWhatMillisecond)
        {
            Particles.set_used(0);
            Time = 0;
            SParticle p;

            Length = (*LinkedPositionPtr - LastPosition).getLength();
            Normal = - (*LinkedPositionPtr - LastPosition).normalize();

            /// Make amount directly related to Length, divided by the texture unit length (may be taken from outside)
            u32 amount = Length / 8;
            amount += 1; // make it at least an additional one

            for(u32 i=1; i<=amount; ++i)
            {
                p.pos = core::vector3df(0,0,0);//Center;

                // Unit length
                const f32 length = (i / (f32)amount) * Length;

                // point distributed equally according to unit length
                p.pos += Normal * length;

                p.startTime = now;
                p.vector = Direction;

                if( MaxAngleDegrees )
                {
                    core::vector3df tgt = Direction;
                    tgt.rotateXYBy( (utils::random(MaxAngleDegrees*2)) - MaxAngleDegrees, core::vector3df() );
                    tgt.rotateYZBy( (utils::random(MaxAngleDegrees*2)) - MaxAngleDegrees, core::vector3df() );
                    tgt.rotateXZBy( (utils::random(MaxAngleDegrees*2)) - MaxAngleDegrees, core::vector3df() );
                    p.vector = tgt;
                }

                p.endTime = now + MinLifeTime;
                if (MaxLifeTime != MinLifeTime)
                    p.endTime += utils::random(MaxLifeTime - MinLifeTime);

                if (MinStartColor==MaxStartColor)
                    p.color = MinStartColor;
                else
                    p.color = MinStartColor.getInterpolated(MaxStartColor, (utils::random(100)) / 100.0f);

                p.startColor = p.color;
                p.startVector = p.vector;

                if (MinStartSize==MaxStartSize)
                    p.startSize = MinStartSize;
                else
                    p.startSize = MinStartSize.getInterpolated(MaxStartSize, (utils::random(100)) / 100.0f);
                p.size = p.startSize;

                Particles.push_back(p);
            }

            outArray = Particles.pointer();

            LastPosition = *LinkedPositionPtr;

            return Particles.size();
        }

        return 0;
    }

    //! Set direction the emitter emits particles
    virtual void setDirection( const core::vector3df& newDirection ) { Direction = newDirection; }

    //! Set direction the emitter emits particles
    virtual void setMinParticlesPerSecond( u32 minPPS ) { MinParticlesPerSecond = minPPS; }

    //! Set direction the emitter emits particles
    virtual void setMaxParticlesPerSecond( u32 maxPPS ) { MaxParticlesPerSecond = maxPPS; }

    //! Set direction the emitter emits particles
    virtual void setMinStartColor( const video::SColor& color ) { MinStartColor = color; }

    //! Set direction the emitter emits particles
    virtual void setMaxStartColor( const video::SColor& color ) { MaxStartColor = color; }

    //! Set the maximum starting size for particles
    virtual void setMaxStartSize( const core::dimension2df& size ) { MaxStartSize = size; };

    //! Set the minimum starting size for particles
    virtual void setMinStartSize( const core::dimension2df& size ) { MinStartSize = size; };

    //! Gets direction the emitter emits particles
    virtual const core::vector3df& getDirection() const { return Direction; }

    //! Gets direction the emitter emits particles
    virtual u32 getMinParticlesPerSecond() const { return MinParticlesPerSecond; }

    //! Gets direction the emitter emits particles
    virtual u32 getMaxParticlesPerSecond() const { return MaxParticlesPerSecond; }

    //! Gets direction the emitter emits particles
    virtual const video::SColor& getMinStartColor() const { return MinStartColor; }

    //! Gets direction the emitter emits particles
    virtual const video::SColor& getMaxStartColor() const { return MaxStartColor; }

    //! Gets the maximum starting size for particles
    virtual const core::dimension2df& getMaxStartSize() const { return MaxStartSize; }

    //! Gets the minimum starting size for particles
    virtual const core::dimension2df& getMinStartSize() const { return MinStartSize; }

    virtual void setMinLifeTime(u32 min) { MinLifeTime = min; }

    virtual void setMaxLifeTime(u32 max) { MaxLifeTime = max; }

    virtual void setMaxAngleDegrees(s32 deg) { MaxAngleDegrees = deg; }

    virtual u32 getMinLifeTime() const { return MinLifeTime; }

    virtual u32 getMaxLifeTime() const { return MaxLifeTime; }

    virtual s32 getMaxAngleDegrees() const { return MaxAngleDegrees; }

private:

    core::array<SParticle> Particles;

    core::vector3df	Normal;
    core::vector3df Direction;
    core::dimension2df MaxStartSize, MinStartSize;
    u32 MinParticlesPerSecond, MaxParticlesPerSecond;
    video::SColor MinStartColor, MaxStartColor;
    u32 MinLifeTime, MaxLifeTime;

    f32 Length;

    u32 Time;
    u32 Emitted;
    s32 MaxAngleDegrees;

    core::vector3df const* LinkedPositionPtr;
    core::vector3df LastPosition;
};

} // end namespace scene
} // end namespace irr

#endif //_SHOOTING_CUBES_VIEW_DETAIL_PARTICLE_0_
