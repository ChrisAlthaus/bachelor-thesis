package gui_control.sequence_editor;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.stream.Collectors;

import gui_control.led_model.LEDModel;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.TextField;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.paint.Color;

public class SequenceBuilderController {
	@FXML
	TableView<LEDModel> table_view;
	
	@FXML
	TableColumn<LEDModel,String> mode_column;
	
	@FXML
	TableColumn<LEDModel, Long> timeOn_column;
	
	@FXML
	TableColumn<LEDModel, Long> timeOff_column;
	
	@FXML
	TableColumn<LEDModel, Boolean> repeat_column;
	
	@FXML
	Button add_Button;
	
	@FXML
	Button save_Button;
	
	@FXML
	ChoiceBox<String> select_mode;
	
	@FXML
	TextField timeOn;
	
	@FXML
	TextField timeOff;
	
	//@FXML
	//CheckBox select_repeat;
	
	@FXML
	TextField enter_name;
	
	@FXML
	Label sequence_error;
	
	@FXML
	Label segment_error;
	
	ObservableList<LEDModel> sequence_list;
	
	EditorController editor_controller;
	
	
	
	public void initialize(){
		mode_column.setCellValueFactory(new PropertyValueFactory<LEDModel,String>("mode"));
		timeOn_column.setCellValueFactory(new PropertyValueFactory<LEDModel,Long>("duration_on"));
		timeOff_column.setCellValueFactory(new PropertyValueFactory<LEDModel,Long>("duration_off"));
		repeat_column.setCellValueFactory(new PropertyValueFactory<LEDModel,Boolean>("repeat"));
		
		sequence_list = FXCollections.observableArrayList();
		table_view.setItems(sequence_list);
		
		
		select_mode.setItems(FXCollections.observableArrayList("Flash" , "Dimming" , "Beacon" , "Staircase" , "Pulse Slow"));
		select_mode.setValue("Flash");
		
		
		
		timeOn.textProperty().addListener(new ChangeListener<String>(){
			
			@Override
			public void changed(ObservableValue<? extends String> arg0, String arg1, String arg2) {
				if(!arg2.matches("[0-9]*") || arg2.length()>4){
					timeOn.setStyle("-fx-text-box-border: red ;");
				}else{
					timeOn.setStyle("");
				}
			}
			
		});
		
		timeOff.textProperty().addListener(new ChangeListener<String>(){
			
			@Override
			public void changed(ObservableValue<? extends String> arg0, String arg1, String arg2) {
				if(!arg2.matches("[0-9]*") || arg2.length()>4){
					timeOff.setStyle("-fx-text-box-border: red ;");
				}else{
					timeOff.setStyle("");
				}
			}
			
		});
		
		sequence_error.setTextFill(Color.RED);
		segment_error.setTextFill(Color.RED);
		
	}
	
	
	public void addLEDModel(){
		String mode = select_mode.getValue();
		long on_time = Long.parseLong(timeOn.textProperty().getValue());
		long off_time = Long.parseLong(timeOff.textProperty().getValue());
		//boolean repeat = select_repeat.isSelected();
		
		if(entries_correct()){
			LEDModel led_model = new LEDModel(mode,on_time,off_time,false);
			sequence_list.add(led_model);
			System.out.println(led_model);	
		}
		
	}
	
	public void save_sequence(){
		List<LEDModel> list = (List<LEDModel>) table_view.getItems();
		ArrayList<LEDModel> save_list = new ArrayList(list);
		
		Sequence sequence = new Sequence();
		sequence.setSequence(save_list);
		
		String entered_text = enter_name.getText();
		if(entered_text.trim().isEmpty()){
			sequence_error.setText("Please enter a sequence name.");
			return;
		}
		if(editor_controller.contains_name(entered_text.trim())){
			sequence_error.setText("Name of the sequence already in use!");
			return;
			
		}
			
		sequence.setName(entered_text.trim());
		editor_controller.addSequence(sequence);
		
		
	}
	
	public boolean entries_correct(){
		String mode = select_mode.valueProperty().getName();
		String textfield_on = timeOn.textProperty().getValue();
		String textfield_off = timeOff.textProperty().getValue();
		if(!textfield_on.matches("[0-9]*") ||textfield_on.length() >4){
			segment_error.setText("Please enter a digit number, with maximum 4 places.");
			return false;
		}
		if(!textfield_off.matches("[0-9]*") ||textfield_off.length() >4){
			segment_error.setText("Please enter a digit number, with maximum 4 places.");
			return false;
		}
		if(mode.isEmpty()){
			segment_error.setText("Please select a mode.");
			return false;
		}
		
		return true;
	}
	
	public void setMainEditor(EditorController editor_controller){
		this.editor_controller = editor_controller;
	}
	

}
