package gui_control.sequence_editor;

import java.io.IOException;
import java.util.Date;

import gui_control.Main;
import gui_control.led_model.LEDModel;
import gui_control.led_view.ArduinoLink;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.ContextMenu;
import javafx.scene.control.ListView;
import javafx.scene.control.MenuItem;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;

public class EditorController {
	
	private ArduinoLink arduino_link;
	
	@FXML
	TableView<Sequence> table_view;
	
	@FXML
	TableColumn<Sequence,String> name_column;
	
	@FXML
	TableColumn<Sequence, Date> date_column;
	
	@FXML
	Button add_sequence;
	
	@FXML
	Button delete_sequence;
	
	@FXML
	ContextMenu context_menu;
	
	@FXML
	MenuItem run_sequence;
	
	ObservableList<Sequence> sequence_list = FXCollections.observableArrayList();
	
	
	public void initialize(){
		name_column.setCellValueFactory(new PropertyValueFactory<>("name"));
		date_column.setCellValueFactory(new PropertyValueFactory<>("creation_date"));
		
		table_view.addEventHandler(MouseEvent.MOUSE_CLICKED, new EventHandler<MouseEvent>(){

			@Override
			public void handle(MouseEvent event) {
				if(event.getButton() == MouseButton.SECONDARY){
					context_menu.show(table_view, event.getScreenX(), event.getScreenY());
				}
				
			}
			
		});
		
		
	
		table_view.setItems(sequence_list);
		//table_view.getColumns().addAll(name_column,date_column);
		
	}
	
	
	
	public ObservableList<Sequence> getSequenceList(){
		ObservableList<Sequence> sequence_list = FXCollections.observableArrayList();
		
		Sequence seq1 = new Sequence("Sequence 1");
		seq1.addSegment(new LEDModel("Flash" , 1000, 1000 ,true));
		
		Sequence seq2 = new Sequence("Sequence 2");
		seq1.addSegment(new LEDModel("Dimming" , 500, 1000 ,true));
		
		sequence_list.addAll(seq1, seq2);
		
		return sequence_list;
	}
	
	public void addSequence(Sequence sequence){
		this.sequence_list.add(sequence);
	}
	
	public void deleteSequence(){
		Sequence sequence = table_view.getSelectionModel().getSelectedItem();
		if(sequence!=null)sequence_list.remove(sequence);
	}
	
	public void showBuilder() throws IOException{
		Main.showSequenceBuilder();
	}
	
	public boolean contains_name(String name){
		for(Sequence s: sequence_list){
			if(s.getName().equals(name))return true;
		}
		
		return false;
	}
	
	public void runSequence() throws IOException, InterruptedException{
		int row_index = table_view.getSelectionModel().getSelectedIndex();
		if(!arduino_link.isConnected())arduino_link.startLink();
		
		arduino_link.controllLED(sequence_list.get(row_index));
	}
	
	public void setArduinoLink(ArduinoLink arduino_link){
		this.arduino_link = arduino_link;
	}
	
	
}
