package gui_control.sequence_editor;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

import gui_control.led_model.LEDModel;

public class Sequence {
	ArrayList<LEDModel> sequence;
	String name;
	String creation_date;
	
	public Sequence(String name){
		sequence = new ArrayList<LEDModel>();
		this.name = name;
		
		DateFormat date_format = new SimpleDateFormat("dd/MM/YYYY HH:MM");
		creation_date = date_format.format(new Date());
	}
	
	public Sequence() {
		DateFormat date_format = new SimpleDateFormat("dd/MM/YYYY HH:MM");
		creation_date = date_format.format(new Date());
	}

	public void addSegment(LEDModel segment){
		sequence.add(segment);
	}

	public ArrayList<LEDModel> getSequence() {
		return sequence;
	}

	public void setSequence(ArrayList<LEDModel> sequence) {
		this.sequence = sequence;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getCreation_date() {
		return creation_date;
	}

	public void setCreation_date(String creation_date) {
		this.creation_date = creation_date;
	}
	
	
	public String toString(){
		String message = "";
		int sequence_length = sequence.size();
		for(int i = 0; i< sequence_length; i++){
			message+=sequence.get(i).toString();
			if(i!=(sequence_length-1))message+="//";
		}
		return message;
	}
	
	
	
}
