package gui_control.WirelessConnection;

import java.awt.Color;
import java.io.IOException;

import javafx.scene.control.Label;
import javafx.scene.control.TextArea;

public class MessageHandler {
	//DISPLAY:SIDE//DISPLAYCOLOR//REFERENCECOLOR//REFERENCE//STEPSIZE//URL//PATHXML//PATHJSON
	//ALLSIDES:SIDE
	//RESETSIDE:SIDE
	//SET:BRIGHTNESS
	private static TcpConnection tcpConnection;
	
	public void establishConnection(Label statusLabel){
		if(tcpConnection != null){
			tcpConnection.initTcpClient(statusLabel);
		}else{
			statusLabel.setText("Please enter a valid ip and port.");
			statusLabel.setTextFill(javafx.scene.paint.Color.RED);
		}
		
	}

	public void updateMovingBarSide(String side, Color displayColor, Color referenceColor, int referenceValue,
			int stepsize, String mode, String url, String pathXML, String pathJson){
		String sendData= "DISPLAY:"+ side + "//" + getHexValue(displayColor) + "//" + getHexValue(referenceColor) + "//" +
						referenceValue + "//" + stepsize + "//" + mode + "//" +  getUrlWithoutPrefix(url) + "//" +
						pathXML + "//" + pathJson;
		
		if(tcpConnection!=null){
			tcpConnection.sendMessage(sendData);
		}else{
			System.out.println("TCP connection not properly setup.");
		}
	}
	
	public void resetSide(String side){
		String sendData= "RESETSIDE:"+ side;
		System.out.println(sendData);
		if(tcpConnection!=null){
			tcpConnection.sendMessage(sendData);
		}else{
			System.out.println("TCP connection not properly setup.");
		}
	}
	
	public void updateOverallPreferences(int sampleTime, int brightness){
		String sendData = "SET:" + sampleTime + "//" + brightness;
		
		
		if(tcpConnection!=null){
			tcpConnection.sendMessage(sendData);
		}else{
			System.out.println("TCP connection not properly setup.");
		}
	}
	
	public String getHexValue(Color color){
		return String.format("%06x", color.getRGB() & 0x00FFFFFF);
	}
	
	public String getUrlWithoutPrefix(String urlWithPrefix){
		return urlWithPrefix.replaceAll("http://", "").replaceAll("https://", "");
	}
	
	public void requestStatus(TextArea statusTextField){
		String sendData = "STATUS:";
		
		if(tcpConnection!=null){
			tcpConnection.sendMessage(sendData);
			tcpConnection.readAndPrintStatus(statusTextField);
		}else{
			System.out.println("TCP connection not properly setup.");
		}
		
		
	}

	public static TcpConnection getWirelessConnection() {
		return tcpConnection;
	}

	public void setTcpConnection(String ip, int portnumber) throws IOException{
		closeConnection();
		tcpConnection= new TcpConnection(ip,portnumber);
	}
	
	public void closeConnection() throws IOException{
		if(tcpConnection!=null){
			tcpConnection.closeSocket();
		}
		
	}
	
	
}
