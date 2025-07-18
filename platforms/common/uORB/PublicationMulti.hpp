/****************************************************************************
 *
 *   Copyright (c) 2019 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file Publication.hpp
 *
 */

#pragma once

#include <px4_platform_common/defines.h>
#include <systemlib/err.h>
#include <uORB/uORB.h>
#include "uORBDeviceNode.hpp"

#include "Publication.hpp"

namespace uORB
{

/**
 * Base publication multi wrapper class
 */
template<typename T>
class PublicationMulti : public PublicationBase
{
public:

	/**
	 * Constructor
	 *
	 * @param meta The uORB metadata (usually from the ORB_ID() macro) for the topic.
	 */
	PublicationMulti(ORB_ID id) :
		PublicationBase(id)
	{}

	PublicationMulti(const orb_metadata *meta) :
		PublicationBase(static_cast<ORB_ID>(meta->o_id))
	{}

	// correction start
        PublicationMulti(ORB_ID id, uint _publisher_id) : PublicationBase(id, _publisher_id) {}
        PublicationMulti(const orb_metadata *meta, uint _publisher_id) : PublicationBase(static_cast<ORB_ID>(meta->o_id), _publisher_id) {}
        PublicationMulti(uint _publisher_id) : PublicationBase(_publisher_id) {}
         // corretion end

	bool advertise()
	{
		if (!advertised()) {
			int instance = 0;
			_handle = orb_advertise_multi(get_topic(), nullptr, &instance);
		}

		return advertised();
	}

	/**
	 * Publish the struct
	 * @param data The uORB message struct we are updating.
	 */
	bool publish(const T &data)
	{
		if (!advertised()) {
			advertise();
		}

                // correction start
                if (_module_id != 0){
                        T copy_for_pub = data;
                        copy_for_pub.publisher_id = _module_id;
                        return (orb_publish(get_topic(), _handle, &copy_for_pub) == PX4_OK);
                }
                else{
                        return (orb_publish(get_topic(), _handle, &data) == PX4_OK);
                }
                // correction end

	}

	int get_instance()
	{
		// advertise if not already advertised
		if (advertise()) {
			return Manager::orb_get_instance(_handle);
		}

		return -1;
	}
};

/**
 * The publication multi class with data embedded.
 */
template<typename T>
class PublicationMultiData : public PublicationMulti<T>
{
public:
	/**
	 * Constructor
	 *
	 * @param meta The uORB metadata (usually from the ORB_ID() macro) for the topic.
	 */
	PublicationMultiData(ORB_ID id) : PublicationMulti<T>(id) {}
	PublicationMultiData(const orb_metadata *meta) : PublicationMulti<T>(meta) {}

        // correction start
        PublicationMultiData(ORB_ID id, uint _publisher_id) : PublicationMulti<T>(id, _publisher_id) {}
        PublicationMultiData(const orb_metadata *meta, uint _publisher_id) : PublicationMulti<T>(meta, _publisher_id) {}
        PublicationMultiData(uint _publisher_id) : PublicationMulti<T>(_publisher_id) {}
        // corretion end

	T	&get() { return _data; }
	void	set(const T &data) { _data = data; }

	// Publishes the embedded struct.
	bool	update() { return PublicationMulti<T>::publish(_data); }
	bool	update(const T &data)
	{
		_data = data;
		return PublicationMulti<T>::publish(_data);
	}

private:
	T _data{};
};

} // namespace uORB
