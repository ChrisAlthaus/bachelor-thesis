package gui_control.EditorView;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;

import gui_control.Main;
import gui_control.DataHandler.DataHandler;
import gui_control.ScenarioModel.ScenarioModel;
import gui_control.SettingRequestView.SettingRequestController;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.image.Image;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.BorderPane;
import javafx.stage.Modality;
import javafx.stage.Stage;

public class EditorController {
	  	@FXML
	    private TableView<ScenarioModel> scenarioTableView;
	  	
	  	 @FXML
	     private TableColumn<ScenarioModel, String> scenarioNameColumn;

	     @FXML
	     private TableColumn<ScenarioModel, String> scenarioDateColumn;

	    @FXML
	    private Button addButton;

	    @FXML
	    private Button deleteButton;
	    
	    private DataHandler dataHandler;
	    ObservableList<ScenarioModel> scenarios = FXCollections.observableArrayList();
		
		
		public EditorController(ArrayList<ScenarioModel> scenarios) {
			this.scenarios = FXCollections.observableArrayList(scenarios);
		}
		
		public EditorController() {
			
		}
		
		
		public void initialize(){
			
			
			
			scenarioNameColumn.setCellValueFactory(new PropertyValueFactory<>("name"));
			scenarioDateColumn.setCellValueFactory(new PropertyValueFactory<>("creationDate"));
			
			//Remove all "None" scenarios
			ArrayList<ScenarioModel> savedScenarios = dataHandler.getScenarios();
			ArrayList<ScenarioModel> displayScenarios = new ArrayList<ScenarioModel>();
			
			for(int i=0; i<savedScenarios.size(); i++){
				ScenarioModel s= savedScenarios.get(i);
				if(s.getName()!="(None)"){
					displayScenarios.add(s);
				}
			}
			
			
			setScenarioList(displayScenarios);
			
			for(ScenarioModel s: this.scenarios){
				System.out.println(s);
			}
			//this.scenarios.add(new ScenarioModel(null,null,null,"djkfsfkl","01/04/2018 15:19:00",0));
			
			scenarioTableView.setItems(scenarios);
		}
		
		public void setScenarioList(ArrayList<ScenarioModel> scenarios){
			this.scenarios = FXCollections.observableArrayList(scenarios);
		}
		
		

		
		public void addScenario() throws IOException{
			SettingRequestController controller = new SettingRequestController(this.scenarios);
			controller.setDataHandler(dataHandler);
			
			FXMLLoader loader = new FXMLLoader();
			loader.setLocation(Main.class.getResource("SettingRequestView/SettingRequestView.fxml"));
			loader.setController(controller);
			
			BorderPane builderView = loader.load();
			
			
			
			
			
			Stage builderStage = new Stage();
			builderStage.setTitle("Scenario Builder");
			builderStage.initModality(Modality.NONE);
			builderStage.getIcons().add(new Image("file:icons/guiLogo.png"));
			
			Scene scene = new Scene(builderView);
			builderStage.setScene(scene);
			builderStage.showAndWait();
		}

		public void setDataHandler(DataHandler dataHandler) {
			this.dataHandler = dataHandler;
		}
		
		public void delete(){
			ScenarioModel deleteScenario = scenarioTableView.getSelectionModel().getSelectedItem();
			
			if(deleteScenario!=null){
				String scenarioName = deleteScenario.getName();
				dataHandler.deleteScenario(scenarioName);
				this.scenarios.remove(deleteScenario);
			}
		}
		
		
}
