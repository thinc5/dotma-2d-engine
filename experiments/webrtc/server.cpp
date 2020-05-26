#include <rtc/rtc.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

using namespace rtc;
using namespace std;

template <class T> weak_ptr<T> make_weak_ptr(shared_ptr<T> ptr) { return ptr; }

int main(int argc, char **argv) {
	InitLogger(LogLevel::Warning);

	Configuration config;
	// config.iceServers.emplace_back("stun.l.google.com:19302");

	auto pc = std::make_shared<PeerConnection>(config);

	pc->onLocalDescription([](const Description &description) {
		cout << "Local Description (Paste this to the other peer):" << endl;
		cout << string(description) << endl;
	});

	pc->onLocalCandidate([](const Candidate &candidate) {
		cout << "Local Candidate (Paste this to the other peer after the local description):"
		     << endl;
		cout << string(candidate) << endl << endl;
	});

	pc->onStateChange(
	    [](PeerConnection::State state) { cout << "[State: " << state << "]" << endl; });

	pc->onGatheringStateChange([](PeerConnection::GatheringState state) {
		cout << "[Gathering State: " << state << "]" << endl;
	});

	auto dc = pc->createDataChannel("test"); // this is the offerer, so create a data channel

	dc->onOpen([&]() { cout << "[DataChannel open: " << dc->label() << "]" << endl; });

	dc->onClosed([&]() { cout << "[DataChannel closed: " << dc->label() << "]" << endl; });

	dc->onMessage([](const variant<binary, string> &message) {
		if (holds_alternative<string>(message)) {
			cout << "[Received: " << get<string>(message) << "]" << endl;
		}
	});

	this_thread::sleep_for(1s);

	bool exit = false;
	while (!exit) {
		cout << endl
		     << "**********************************************************************************"
		        "*****"
		     << endl
		     << "* 0: Exit /"
		     << " 1: Enter remote description /"
		     << " 2: Enter remote candidate /"
		     << " 3: Send message /"
		     << " 4: Print Connection Info *" << endl
		     << "[Command]: ";

		int command = -1;
		cin >> command;
		cin.ignore();

		switch (command) {
		case 0: {
			exit = true;
			break;
		}
		case 1: {
			// Parse Description
			cout << "[Description]: ";
			string sdp, line;
			while (getline(cin, line) && !line.empty()) {
				sdp += line;
				sdp += "\r\n";
			}
			pc->setRemoteDescription(sdp);
			break;
		}
		case 2: {
			// Parse Candidate
			cout << "[Candidate]: ";
			string candidate;
			getline(cin, candidate);
			pc->addRemoteCandidate(candidate);
			break;
		}
		case 3: {
			// Send Message
			if (!dc->isOpen()) {
				cout << "** Channel is not Open ** ";
				break;
			}
			cout << "[Message]: ";
			string message;
			getline(cin, message);
			dc->send(message);
			break;
		}
		case 4: {
			// Connection Info
			if (!dc || !dc->isOpen()) {
				cout << "** Channel is not Open ** ";
				break;
			}
			CandidateInfo local, remote;
			std::optional<std::chrono::milliseconds> rtt = pc->rtt();
			if (pc->getSelectedCandidatePair(&local, &remote)) {
				cout << "Local: " << local.address << ":" << local.port << " " << local.type << " "
				     << local.transportType << endl;
				cout << "Remote: " << remote.address << ":" << remote.port << " " << remote.type
				     << " " << remote.transportType << endl;
				cout << "Bytes Sent:" << pc->bytesSent()
				     << " / Bytes Received:" << pc->bytesReceived() << " / Round-Trip Time:";
				if (rtt.has_value())
					cout << rtt.value().count();
				else
					cout << "null";
				cout << " ms";
			} else
				cout << "Could not get Candidate Pair Info" << endl;
			break;
		}
		default: {
			cout << "** Invalid Command ** ";
			break;
		}
		}
	}

	if (dc)
		dc->close();
	if (pc)
		pc->close();
}