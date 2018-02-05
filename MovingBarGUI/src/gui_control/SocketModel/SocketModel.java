package gui_control.SocketModel;

public class SocketModel {
	String ip;
	int portNumber;
	
	public SocketModel(String ip, int portNumber) {
		this.ip = ip;
		this.portNumber = portNumber;
	}

	public String getIp() {
		return ip;
	}

	public void setIp(String ip) {
		this.ip = ip;
	}

	public int getPortNumber() {
		return portNumber;
	}

	public void setPortNumber(int portNumber) {
		this.portNumber = portNumber;
	};
	
	
}
