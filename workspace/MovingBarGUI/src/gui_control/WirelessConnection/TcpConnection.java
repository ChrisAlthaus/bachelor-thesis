package gui_control.WirelessConnection;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

import javafx.application.Platform;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.paint.Color;

public class TcpConnection {
	String ip;
	int portNumber;
	Socket socket;
	boolean isReady =false;
	boolean isRunning = false;
	Thread connectionThread;
	
	public TcpConnection(String ip, int portNumber) {
		this.ip = ip;
		this.portNumber = portNumber;
	}

	public void initTcpClient(Label statusLabel){
		if(ip==null){
			statusLabel.setText("IP Address is empty.");
			statusLabel.setTextFill(Color.RED);
			
		}
		
		connectionThread= new Thread(new Runnable(){
			String status = "";
			@Override
			public void run() {
				// TODO Auto-generated method stub
				try {
					socket = new Socket(ip,portNumber);
					isReady=true;
				} catch (UnknownHostException e) {
					//e.printStackTrace();
					status="Unknown IP Address";
					//statusLabel.setText("Unknown IP Adress");
					//statusLabel.setTextFill(Color.RED);
			
				} catch (IOException e) {
					status=e.getMessage();
					//statusLabel.setText(e.getMessage());
					//statusLabel.setTextFill(Color.RED);
					
				}
				
				Platform.runLater(()->{
					if(!isReady){
						statusLabel.setText(status);
						statusLabel.setTextFill(Color.RED);	
					}else{
						statusLabel.setText("Connection established successfully.");
						statusLabel.setTextFill(Color.GREEN);	
					}	
				});
				
				
			}
			
		});
		
		if(!isRunning){
			connectionThread.start();
		}else{

			Platform.runLater(()->{
				statusLabel.setText("Already trying to connect...");
				statusLabel.setTextFill(Color.RED);	
			});
		}
		
		
	}
	

	
	
	public void sendMessage(String message){
		if(isReady()){
			PrintWriter printWriter;
			try {
				printWriter = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
				printWriter.print(message);
			 	printWriter.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}else{
			System.out.println("TCP connection not available for read and write operations.");
		}
		 	
	}
	
	String readMessage() throws IOException {
		BufferedReader input =new BufferedReader(new InputStreamReader(socket.getInputStream()));
		
		String line;
		StringBuilder sb = new StringBuilder();
		System.out.println("read message");
		while ((line = input.readLine()) != null) {
			System.out.println("received message:"+ line);
			if(line.equals("EOF"))break;
			else{
				sb.append(line + "\n");
			}
		}
		
		return sb.toString();
		
		
	}
	
	void readAndPrintStatus(TextArea textArea){
		Thread receiveStatusThread = new Thread(new Runnable(){

			@Override
			public void run() {
				long t= System.currentTimeMillis();
				long end = t+60000;
				 
				try {
					final String receivedMessage = readMessage();
					
					Platform.runLater(()->{
						textArea.appendText(receivedMessage);
					});
					
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				};
				
				
					
				
				
				
			}
			
		});
		
		receiveStatusThread.start();
	}
	
	public void closeSocket() throws IOException{
		
		if(socket!=null){
			if(socket.isClosed()){
				socket.close();
			}
		}
		
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
	}
	
	public boolean isReady(){
		return isReady;
	}
}
