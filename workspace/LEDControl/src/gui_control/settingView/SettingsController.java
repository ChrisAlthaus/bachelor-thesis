package gui_control.settingView;

import java.util.ArrayList;
import java.util.List;

import com.fazecast.jSerialComm.SerialPort;

import gui_control.led_view.ArduinoLink;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.Label;
import javafx.scene.control.Tooltip;
import javafx.scene.paint.Color;

public class SettingsController {
	
	ObservableList<Integer> pins = FXCollections.observableArrayList(0,1,2,3,4,5,6,7,8,9,10,11,12,13);
	
	ArduinoLink arduino_link;
	
	@FXML
	private ChoiceBox led_pins;
	
	@FXML
	private ChoiceBox<String> select_port;
	
	@FXML
	private Label error_label;
	
	private String port;
	
	public SettingsController(){
		
	}
	
	@FXML
	private void initialize(){
		if(arduino_link!=null){
			ArrayList<String> available_ports = arduino_link.getArduinoPorts();
			select_port.setItems(FXCollections.observableArrayList(available_ports));
			
			port = arduino_link.getCurrentPortName();
			select_port.setValue(port);
		}
		
		led_pins.setItems(pins);
		
		select_port.valueProperty().addListener(new ChangeListener<String>(){

			@Override
			public void changed(ObservableValue<? extends String> arg0, String arg1, String arg2) {
				arduino_link.setArduino_port(arg2);
				port = arg2;
			}
			
		});
	}
	
	
	public void showMessage(String message){
		error_label.setTextFill(Color.RED);
		error_label.setText(message);
	}
	
	public void setArduinoLink( ArduinoLink arduino_link ){
		this.arduino_link = arduino_link; 
	}
	
}
