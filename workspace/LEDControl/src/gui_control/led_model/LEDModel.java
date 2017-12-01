package gui_control.led_model;

import java.util.ArrayList;

import javafx.scene.paint.Color;

public class LEDModel {
	long duration_on;
	long duration_off;
	boolean repeat =false;
	int red;
	int green;
	int blue;
	String mode ="Flash";
	
	public LEDModel(String mode, long duration_on, long duration_off, boolean loop){
		this.mode = mode;
		this.duration_on = duration_on;
		this.duration_off = duration_off;
		this.repeat = loop;
	}
	
	public LEDModel(){
		
	}
	
	
	public long getDuration_on() {
		return duration_on;
	}

	public void setDuration_on(long duration_on) {
		this.duration_on = duration_on;
	}

	public long getDuration_off() {
		return duration_off;
	}

	public void setDuration_off(long duration_off) {
		this.duration_off = duration_off;
	}

	public boolean getRepeat() {
		return repeat;
	}

	public void setRepeat(boolean repeat) {
		this.repeat = repeat;
	}

	public String getMode() {
		return mode;
	}

	public void setMode(String mode) {
		this.mode = mode;
	}

	public void setRGB(int red, int green, int blue){
		this.red = red;
		this.green = green;
		this.blue = blue;
	}

	
	public String toString(){
		String message_mode = mode.replaceAll("\\s", "").toUpperCase();
		System.out.println("mode=" + message_mode);
		if(message_mode.equals("FLASH") || message_mode.equals("DIMMING")){
			if(repeat){
				return ( message_mode + "/" + "LOOP" + "/" + duration_on + "/" + duration_off );
			}else{
				return ( message_mode + "/" + duration_on + "/" + duration_off );
			}	
		}else{
			long total_duration = duration_on + duration_off;
			if(repeat){
				return ( message_mode + "/" + "LOOP" + "/" + total_duration);
			}else{
				return ( message_mode + "/" + total_duration );
			}	
		}
		
		
		
	}
	
}
