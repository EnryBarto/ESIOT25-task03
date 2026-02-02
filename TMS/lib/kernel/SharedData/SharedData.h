#ifndef __SHARED_DATA__
#define __SHARED_DATA__

// This class is used for inter-FSM communication
class SharedData {
	public:
	  	SharedData();
		bool isMqttError();
		bool isWifiError();
		void setWifiError(bool value);

	private:
		bool mqttError = false;
		bool wifiError;
};

#endif
