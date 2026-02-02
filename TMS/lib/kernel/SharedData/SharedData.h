#ifndef __SHARED_DATA__
#define __SHARED_DATA__

#include <PubSubClient.h>

// This class is used for inter-FSM communication
class SharedData {
	public:
	  	SharedData();
		bool isMqttError();
		bool isWifiError();
		void setWifiError(bool value);
		void setMqttError(bool value);
		PubSubClient *getMqttClient();
		void setMqttClient(PubSubClient *client);

	private:
		bool mqttError = true;
		bool wifiError = true;

		PubSubClient *mqttClient;
};

#endif
