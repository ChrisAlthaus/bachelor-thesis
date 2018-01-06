package gui_control.WirelessConnection;

import java.awt.Color;

public class MessageHandler {
	//DISPLAY:SIDE//DISPLAYCOLOR//REFERENCECOLOR//REFERENCE//STEPSIZE//URL//PATHXML//PATHJSON
	//SET:BRIGHTNESS
	private static BluetoothConnection wirelessConnection = new BluetoothConnection();

	public void updateMovingBarSide(String side, Color displayColor, Color referenceColor, int referenceValue,
			int stepsize, String url, String pathXML, String pathJson){
		String sendData= "DISPLAY:"+ side + "//" + getHexValue(displayColor) + "//" + getHexValue(referenceColor) + "//" +
						referenceValue + "//" + stepsize + "//" +  getUrlWithoutPrefix(url) + "//" +
						pathXML + "//" + pathJson;
		
		if(wirelessConnection.isDeviceSet()){
			wirelessConnection.sendMessage(sendData);
		}
	}
	
	private void updateOverallPreferences(int brightness){
		String sendData = "SET:" + brightness;
		
		if(wirelessConnection.isDeviceSet()){
			wirelessConnection.sendMessage(sendData);
		}
	}
	
	public String getHexValue(Color color){
		return String.format("#%06x", color.getRGB() & 0x00FFFFFF);
	}
	
	public String getUrlWithoutPrefix(String urlWithPrefix){
		String url = urlWithPrefix.replaceAll("http://", "");
		url= urlWithPrefix.replaceAll("https://", "");
		
		return url;
	}

	public static BluetoothConnection getWirelessConnection() {
		return wirelessConnection;
	}

	public static void setWirelessConnection(BluetoothConnection wirelessConnection) {
		MessageHandler.wirelessConnection = wirelessConnection;
	}
	
	
}
