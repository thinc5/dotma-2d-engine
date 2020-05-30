/**
 * Copyright (c) 2019 Paul-Louis Ageneau
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef RTC_INCLUDE_H
#define RTC_INCLUDE_H

#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0602
#endif
#endif

#include "log.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace rtc {

using std::byte;
using std::string;
using binary = std::vector<byte>;

using std::nullopt;

using std::size_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;

const size_t MAX_NUMERICNODE_LEN = 48; // Max IPv6 string representation length
const size_t MAX_NUMERICSERV_LEN = 6;  // Max port string representation length

const uint16_t DEFAULT_SCTP_PORT = 5000; // SCTP port to use by default
const size_t DEFAULT_MAX_MESSAGE_SIZE = 65536;    // Remote max message size if not specified in SDP
const size_t LOCAL_MAX_MESSAGE_SIZE = 256 * 1024; // Local max message size


template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...)->overloaded<Ts...>;

template <typename... P> class synchronized_callback {
public:
	synchronized_callback() = default;
	synchronized_callback(std::function<void(P...)> func) { *this = std::move(func); };
	~synchronized_callback() { *this = nullptr; }

	synchronized_callback &operator=(std::function<void(P...)> func) {
		std::lock_guard lock(mutex);
		callback = std::move(func);
		return *this;
	}

	void operator()(P... args) const {
		std::lock_guard lock(mutex);
		if (callback)
			callback(args...);
	}

	operator bool() const {
		std::lock_guard lock(mutex);
		return callback ? true : false;
	}

private:
	std::function<void(P...)> callback;
	mutable std::recursive_mutex mutex;
};
}

#endif
