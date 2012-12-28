// Copyright (C) 2000 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_POOL_GUARD_HPP
#define BOOST_POOL_GUARD_HPP

/*!
  \file
  \brief Extremely Light-Weight guard class.
  \details Auto-lock/unlock-er
  detail/guard.hpp provides a type guard<Lockable>
  that allows scoped access to the Lockable's locking and unlocking operations.
  It is used to ensure that a Lockable is unlocked, even if an exception is thrown.
*/

/* 2012.12 arch.jslin:

    This totally abstract "guard" doesn't know anything about Mutex. I am tired of fixing the comments though.

    It is just able to guard anything that provide it with lock() and unlock() concept.
    of course provided that lock() and unlock() of that Lockable is actually implemented.

*/

namespace boost {

namespace details {
namespace pool {

template <typename Lockable> //!< \tparam Lockable (platform-specific) mutex class.
class guard
{ //! Locks the Lockable, binding guard<Lockable> to Lockable.
	/*! Example:
	Given a (platform-specific) mutex class, we can wrap code as follows:

	extern mutex global_lock;

	static void f()
	{
		boost::details::pool::guard<mutex> g(global_lock);
		// g's constructor locks "global_lock"

		... // do anything:
				//   throw exceptions
				//   return
				//   or just fall through
	} // g's destructor unlocks "global_lock"
	*/
  private:
    Lockable & l_;

    guard(const guard &); //!< Guards the mutex, ensuring unlocked on destruction, even if exception is thrown.
    void operator=(const guard &);

  public:
    explicit guard(Lockable & l) : l_(l)
    { //! Locks the mutex of the guard class.
        l_.lock();
    }

    ~guard()
    { //! destructor unlocks the mutex of the guard class.
        l_.unlock();
    }
}; // class guard

} // namespace pool
} // namespace details

} // namespace boost

#endif
