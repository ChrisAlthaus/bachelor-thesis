package gui_control.led_view;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import com.fazecast.jSerialComm.*;

import gui_control.led_model.LEDModel;
import gui_control.sequence_editor.Sequence;
import javafx.collections.FXCollections;

public class ArduinoLink {
	
	public static OutputStream output_stream;
	public static InputStream input_stream;
	private static SerialPort arduino_port;
	
	
	public boolean openLink() throws InterruptedException{
		if(arduino_port.openPort()){
			System.out.println("Opened port:" + arduino_port.getSystemPortName());
			Thread.sleep(2000);
			return true;
		}else{
			System.out.println("Couldn't open port:" + arduino_port.getSystemPortName());
			return false;
		}
	}
	


	public String startLink() throws IOException, InterruptedException{
		
		if(!openLink())return "Error: Failed to open link.";
		
		arduino_port.setComPortParameters(9600,8,1,SerialPort.EVEN_PARITY);
		arduino_port.setComPortTimeouts(SerialPort.TIMEOUT_SCANNER, 1000, 1000);
		System.out.println("read timeout=" + arduino_port.getReadTimeout() + ",write timeout=" + arduino_port.getWriteTimeout());
		
		
		
		output_stream = arduino_port.getOutputStream();
		input_stream = arduino_port.getInputStream();
		Scanner arduino_read = new Scanner(input_stream);
		
		int wait_cycle = 0;
		while(input_stream.available() <= 0){
			Thread.sleep(1000);
			wait_cycle++;
			if(wait_cycle == 4){
				return "Error: Device not reachable.";
			}
		}
		
		String message = null;
		Thread.sleep(4000);
		if(arduino_read.hasNextLine())message = arduino_read.nextLine();
		
		System.out.println(message);
		if(message == null){
			return "Error: System not intialized or linked correctly.";
		}
		
		arduino_read.close();
		
		
		
		
		return "Linked with arduino.";
		
	}
	
	public void controllLED(LEDModel led_model) throws IOException, InterruptedException{
		String mode = led_model.getMode();
		long duration_on = led_model.getDuration_on();
		long duration_off = led_model.getDuration_off();
		String loop_checked = led_model.getRepeat()==true?"true":"false";
		
		System.out.println("transferring data");
		System.out.println(led_model.toString());
		
		String message = "NORMAL:" + led_model.toString();
		
		output_stream.write(message.getBytes());
		
		output_stream.flush();
	}
	
	public void controllLED(Sequence sequence) throws IOException{
		System.out.println("transferring data");
		System.out.println("sequence="+sequence.toString());
		
		String message = "SEQUENCE:" + sequence.toString();
		output_stream.write(message.getBytes());
		
		output_stream.flush();
	}
	
	
	public ArrayList<String> getArduinoPorts(){
		SerialPort[] ports = SerialPort.getCommPorts();
		
		if(ports.length > 0){
			ArrayList<String> ports_list = new ArrayList(ports.length);
			
			for(SerialPort port: ports){
				ports_list.add(port.getSystemPortName());
			}
			return ports_list;
		}else{
			System.out.println("No ports detected");
			return null;
		}
		
	}
	
	public boolean isConnected(){
		return arduino_port.isOpen();
	}
	

	public SerialPort getArduino_port() {
		return arduino_port;
	}
	
	public String getCurrentPortName(){
		return arduino_port.getSystemPortName();
	}
	
	public void setArduino_port(String port_identifier) {
		arduino_port = SerialPort.getCommPort(port_identifier);
	}
	
	public boolean setDefaultPort(){
		ArrayList<String> ports_list = getArduinoPorts();
		if(ports_list!=null){
			arduino_port = SerialPort.getCommPort(ports_list.get(0));
			return false;
		}
		return true;
	}
}
