package gui_control.led_view;

import java.io.IOException;
import java.text.NumberFormat;
import java.util.prefs.Preferences;

import gui_control.led_model.LEDModel;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.ColorPicker;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.scene.control.Tooltip;
import javafx.scene.paint.Color;

public class LEDViewController {
	
	private boolean error_occured = false;
	private ArduinoLink arduino_link;
	
	@FXML
	private Slider slider_on;
	
	@FXML
	private TextField duration_textfield_on;
	
	@FXML
	private Slider slider_off;
	
	@FXML
	private TextField duration_textfield_off;
	
	@FXML
	private Button start_button;
	
	@FXML
	private Label error_label;
	
	@FXML
	private CheckBox loop_check;
	
	@FXML
	private ColorPicker color_picker;
	
	@FXML 
	private Tooltip color_tooltip;
	
	@FXML
	private ChoiceBox<String> select_mode;
	
	
	private static LEDModel led_model;
	private static ObservableList<String> modes;
	
	
	
	public LEDViewController(){
		led_model = new LEDModel("Flash",500,500,false);
		modes = FXCollections.observableArrayList("Flash" , "Dimming" , "Beacon" , "Staircase" , "Pulse Slow");
	}
	
	@FXML
	private void initialize(){
		Long duration_on = led_model.getDuration_on();
		Long duration_off = led_model.getDuration_off();
		
		duration_textfield_on.setText(String.valueOf(duration_on));
		slider_on.setValue(duration_on);
		duration_textfield_off.setText(String.valueOf(duration_off));
		slider_off.setValue(duration_off);
		
		slider_on.valueProperty().addListener(new ChangeListener<Number>(){

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
				slider_on.setValue(newValue.intValue());
				duration_textfield_on.textProperty().bindBidirectional(slider_on.valueProperty(), NumberFormat.getNumberInstance());
				
				led_model.setDuration_on(newValue.longValue());
				
			}

		});
		
		slider_off.valueProperty().addListener(new ChangeListener<Number>(){

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
				slider_off.setValue(newValue.intValue());
				duration_textfield_off.textProperty().bindBidirectional(slider_off.valueProperty(), NumberFormat.getNumberInstance());
				
				led_model.setDuration_off(newValue.longValue());
				
			}

		});
		
		setColorText(color_picker.getValue());
		
		color_picker.valueProperty().addListener(new ChangeListener<Color>(){

			@Override
			public void changed(ObservableValue<? extends Color> arg0, Color arg1, Color arg2) {
				setColorText(arg2);
				led_model.setRGB( (int) (arg2.getRed() *255), (int) (arg2.getGreen() *255),(int) (arg2.getBlue() *255));
			}
			
		});
		
		select_mode.setItems(modes);
		select_mode.setValue(led_model.getMode());
		
		select_mode.valueProperty().addListener(new ChangeListener<String>(){

			@Override
			public void changed(ObservableValue<? extends String> arg0, String arg1, String arg2) {
				// TODO Auto-generated method stub
				led_model.setMode(arg2);
			}
			
		});
		
		loop_check.selectedProperty().addListener(new ChangeListener<Boolean>(){

			@Override
			public void changed(ObservableValue<? extends Boolean> arg0, Boolean arg1, Boolean arg2) {
				led_model.setRepeat(loop_check.isSelected());
			}
			
		});
		
		loop_check.setSelected(led_model.getRepeat());
		
		
	}
	
	@FXML
	private void startLEDsOnArduino() throws IOException, InterruptedException{
		if(arduino_link.getArduino_port()!=null){
			if(!arduino_link.isConnected() )showMessage(arduino_link.startLink());
			System.out.println(led_model);
			if(!error_occured)arduino_link.controllLED(led_model);
		}else{
			showMessage("Error: No port selected!");
		}
		
		
	}
	

	private void setColorText(Color color){
		color_tooltip.setText("R:" + Math.round(color.getRed() *255) + " G:" + Math.round(color.getGreen() *255) + 
								" B:" + Math.round(color.getBlue() *255));
	}
	
	
	public void showMessage(String message){
		System.out.println(message.substring(0,5));
		if(message.substring(0,5).equals("Error")){
			error_label.setTextFill(Color.RED);
			error_label.setText(message);
			error_occured = true;
		}else{
			error_label.setTextFill(Color.GREEN);
			error_label.setText(message);
			error_occured = false;
		}
		
	}
	
	public void setArduinoLink(ArduinoLink arduino_link){
		this.arduino_link = arduino_link;
	}
	
}
