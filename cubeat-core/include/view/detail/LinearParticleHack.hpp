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
		const core::vector3df& center,
		const core::vector3df& normal, f32 length,
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
		);

	//! Prepares an array with new particles to emitt into the system
	//! and returns how much new particles there are.
	virtual s32 emitt(u32 now, u32 timeSinceLastCall, SParticle*& outArray);

	//! Set the center of the radius for the cylinder, at one end of the cylinder
	virtual void setCenter( const core::vector3df& center ) { Center = center; }

	//! Set the normal of the cylinder
	virtual void setNormal( const core::vector3df& normal ) { Normal = normal; }

	//! Set the length of the cylinder
	virtual void setLength( f32 length ) { Length = length; }

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

	//! Get the center of the cylinder
	virtual const core::vector3df& getCenter() const { return Center; }

	//! Get the normal of the cylinder
	virtual const core::vector3df& getNormal() const { return Normal; }

	//! Get the center of the cylinder
	virtual f32 getLength() const { return Length; }

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
	virtual const core::dimension2df& getMaxStartSize() const { return MaxStartSize; };

	//! Gets the minimum starting size for particles
	virtual const core::dimension2df& getMinStartSize() const { return MinStartSize; };

private:

	core::array<SParticle> Particles;

	core::vector3df	Center;
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
};

//! constructor
LinearParticleEmitter::LinearParticleEmitter(
	const core::vector3df& center,
	const core::vector3df& normal, f32 length,
	const core::vector3df& direction,
	u32 minParticlesPerSecond, u32 maxParticlesPerSecond,
	const video::SColor& minStartColor, const video::SColor& maxStartColor,
	u32 lifeTimeMin, u32 lifeTimeMax, s32 maxAngleDegrees,
	const core::dimension2df& minStartSize,
	const core::dimension2df& maxStartSize )
	: Center(center), Normal(normal), Direction(direction),
	MaxStartSize(maxStartSize), MinStartSize(minStartSize),
	MinParticlesPerSecond(minParticlesPerSecond),
	MaxParticlesPerSecond(maxParticlesPerSecond),
	MinStartColor(minStartColor), MaxStartColor(maxStartColor),
	MinLifeTime(lifeTimeMin), MaxLifeTime(lifeTimeMax),
	Length(length), Time(0), Emitted(0),
	MaxAngleDegrees(maxAngleDegrees)
{
	#ifdef _DEBUG
	setDebugName("LinearParticleEmitter");
	#endif
}


//! Prepares an array with new particles to emitt into the system
//! and returns how much new particles there are.
s32 LinearParticleEmitter::emitt(u32 now, u32 timeSinceLastCall, SParticle*& outArray)
{
	Time += timeSinceLastCall;

	using namespace psc;

	const u32 pps = (MaxParticlesPerSecond - MinParticlesPerSecond);
	const f32 perSecond = pps ? (f32)MinParticlesPerSecond + (utils::random(pps)) : MinParticlesPerSecond;
	const f32 everyWhatMillisecond = 1000.0f / perSecond;

	if(Time > everyWhatMillisecond)
	{
		Particles.set_used(0);
		u32 amount = (u32)((Time / everyWhatMillisecond) + 0.5f);
		Time = 0;
		SParticle p;

		if(amount > MaxParticlesPerSecond*2)
			amount = MaxParticlesPerSecond * 2;

		for(u32 i=0; i<amount; ++i)
		{
			// Random length
			const f32 length = (i / (f32)amount) * Length; //fmodf( (f32)os::Randomizer::rand(), Length * 1000.0f ) * 0.001f;

			// Random point along the cylinders length
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

			if(MaxLifeTime - MinLifeTime == 0)
				p.endTime = now + MinLifeTime;
			else
				p.endTime = now + MinLifeTime + (utils::random(MaxLifeTime - MinLifeTime));

			p.color = MinStartColor.getInterpolated(
				MaxStartColor, (utils::random(100)) / 100.0f);

			p.startColor = p.color;
			p.startVector = p.vector;

			if (MinStartSize==MaxStartSize)
				p.startSize = MinStartSize;
			else
				p.startSize = MinStartSize.getInterpolated(
					MaxStartSize, (utils::random(100)) / 100.0f);
			p.size = p.startSize;

			Particles.push_back(p);
		}

		outArray = Particles.pointer();

		return Particles.size();
	}

	return 0;
}

} // end namespace scene
} // end namespace irr

#endif //_SHOOTING_CUBES_VIEW_DETAIL_PARTICLE_0_
