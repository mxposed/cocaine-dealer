/*
    Copyright (c) 2011-2012 Rim Zaidullin <creator@bash.org.ru>
    Copyright (c) 2011-2012 Other contributors as noted in the AUTHORS file.

    This file is part of Cocaine.

    Cocaine is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Cocaine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef _COCAINE_DEALER_CLIENT_HPP_INCLUDED_
#define _COCAINE_DEALER_CLIENT_HPP_INCLUDED_

#include <string>

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

#include <cocaine/dealer/message.hpp>
#include <cocaine/dealer/response.hpp>
#include <cocaine/dealer/utils/data_container.hpp>
#include <cocaine/dealer/message_path.hpp>
#include <cocaine/dealer/message_policy.hpp>

namespace cocaine {
namespace dealer {

class dealer_t : private boost::noncopyable {
public:
	typedef boost::shared_ptr<response_t> response_ptr_t;
	typedef std::vector<response_ptr_t> responses_list_t;

public:
	explicit dealer_t(const std::string& config_path = "");
	virtual ~dealer_t();

	response_ptr_t
	send_message(const message_t& message);

	response_ptr_t
	send_message(const void* data,
				 size_t size,
				 const message_path_t& path,
				 const message_policy_t& policy);

	response_ptr_t
	send_message(const void* data,
				 size_t size,
				 const message_path_t& path);

	responses_list_t
	send_messages(const void* data,
				  size_t size,
				  const message_path_t& path,
				  const message_policy_t& policy);

	responses_list_t
	send_messages(const void* data,
				  size_t size,
				  const message_path_t& path);

	template <typename T> response_ptr_t
	send_message(const T& object,
				 const message_path_t& path,
				 const message_policy_t& policy)
	{
		msgpack::sbuffer buffer;
		msgpack::pack(buffer, object);
		return send_message(reinterpret_cast<const void*>(buffer.data()), buffer.size(), path, policy);
	}

	template <typename T> response_ptr_t
	send_message(const T& object,
				 const message_path_t& path)
	{
		msgpack::sbuffer buffer;
		msgpack::pack(buffer, object);
		return send_message(reinterpret_cast<const void*>(buffer.data()), buffer.size(), path);
	}

	size_t stored_messages_count(const std::string& service_alias);
	void remove_stored_message(const message_t& message);
	void remove_stored_message_for(const response_ptr_t& response);
	void get_stored_messages(const std::string& service_alias,
							 std::vector<message_t>& messages);

	message_policy_t policy_for_service(const std::string& service_alias);
	
private:
	boost::shared_ptr<dealer_impl_t> m_impl;
};

} // namespace dealer
} // namespace cocaine

#endif // _COCAINE_DEALER_CLIENT_HPP_INCLUDED_
