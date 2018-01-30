package gui_control.SettingRequestView;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.Date;

import javax.swing.text.Document;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import gui_control.DataHandler.DataHandler;
import gui_control.ScenarioModel.ScenarioModel;
import gui_control.XMLParser.JSONParser;
import gui_control.XMLParser.XMLBuilder;
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.ContextMenu;
import javafx.scene.control.Label;
import javafx.scene.control.MenuItem;
import javafx.scene.control.TextField;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;

public class SettingRequestController {
    
	@FXML
    private TextField URLTextfield;
	   
	@FXML
	private ChoiceBox<String> fileTypeChoicebox;

    @FXML
    private Button DisplayButton;
    
    @FXML
    private TreeView<String> structuredTreeview;
    
    @FXML
    private ContextMenu contextMenuTreeview;
    
    @FXML
    private MenuItem menuItemChoose;
    
    @FXML
    private TextField scenarioNameTextField;

    @FXML
    private Button saveScenarioButton;
    
    @FXML
    private Label statusLabel;
    
    
    ObservableList<ScenarioModel> scenarioList = FXCollections.observableArrayList();
    
    
	@FXML
	private void initialize(){
		structuredTreeview.addEventHandler(MouseEvent.MOUSE_CLICKED, new EventHandler<MouseEvent>(){

			@Override
			public void handle(MouseEvent event) {
				if(event.getButton() == MouseButton.SECONDARY){
					contextMenuTreeview.show(structuredTreeview, event.getScreenX(), event.getScreenY());
				}
				
			}
			
		});
		
		ObservableList<String> dataTypes = FXCollections.observableArrayList("XML","JSON");
		
		fileTypeChoicebox.setItems(dataTypes);
		fileTypeChoicebox.setValue("XML");
		
	}
	
	
	
	public SettingRequestController(ObservableList<ScenarioModel> scenarioList) {
		this.scenarioList = scenarioList;
	}
	
	public SettingRequestController() {
	
	}


	File xmlFile= new File("data.xml");
	File jsonFile= new File("data.json");
	
	String pathToValue;
	DataHandler dataHandler;
	String url ="";
	
	boolean validUrl=false;
	
	public void displayData() throws SAXException, ParserConfigurationException, IOException{
		//deleteFileContent(xmlFile);
		//deleteFileContent(jsonFile);
		//structuredTreeview = builder.getTreeView(new File("data.xml"));
		String URLPath = URLTextfield.textProperty().getValue();
		if(URLPath.length() >= 0){
			String fileType = fileTypeChoicebox.getSelectionModel().getSelectedItem();
			url=URLPath;
			validUrl=false;
			//download file
			if(fileType == "XML"){
				downloadFile(URLPath,xmlFile,fileType);
				setTreeViewXML(xmlFile);
			}else if(fileType == "JSON"){
				downloadFile(URLPath,jsonFile,fileType); 
				
				setTreeViewJSON(jsonFile);
			}
			
			
		}
		
		
		
	}
	
	void deleteFileContent(File file) throws FileNotFoundException{
		PrintWriter writer = new PrintWriter(file);
		writer.print("");
		writer.close();
	}
	
	public void setTreeViewXML(File file) throws SAXException, ParserConfigurationException, IOException{
		
		XMLBuilder builder = new XMLBuilder();
		structuredTreeview.setRoot(builder.getTreeView(file));
		
	}
	
	public void setTreeViewJSON(File file){
		statusLabel.setText("");
		XMLBuilder builder = new XMLBuilder();
		
		JSONParser parser = new JSONParser();
		String xml;
		try {
			xml = parser.getXMLString(file);
			//check if valid xml returned
			if(xml!=null){
				structuredTreeview.setRoot(builder.getTreeView(xml));
			}
		} catch (FileNotFoundException e) {
			statusLabel.setStyle("-fx-text-box-border: red ;");
			statusLabel.setText("Couldn't load file.");
		} catch (SAXException | ParserConfigurationException | IOException e) {
			statusLabel.setStyle("-fx-text-box-border: red ;");
			statusLabel.setText("Couldn't parse file.");
		}
		
		
	}
	
	public void saveScenarioAndExit(){
		ScenarioModel scenario = new ScenarioModel();
		
		if(pathToValue==null){
			statusLabel.setStyle("-fx-text-box-border: red ;");
			statusLabel.setText("Please set a path.");
			return;
		}
		
		if(!url.isEmpty() && validUrl){
			scenario.setRequestURL(url);
		}else{
			statusLabel.setStyle("-fx-text-inner-color: red;");
			statusLabel.setText("Please set a valid URL.");
			URLTextfield.setStyle("-fx-text-box-border: red ;");
			return;
		}
		
		
		String fileType = fileTypeChoicebox.getSelectionModel().getSelectedItem();
		if(fileType=="JSON"){
			scenario.setPathToJson(pathToValue);
		}else if(fileType=="XML"){
			scenario.setPathToXML(pathToValue);
		}else{
			statusLabel.setStyle("-fx-text-box-border: red ;");
			statusLabel.setText("Please choose a file type.");
			fileTypeChoicebox.setStyle("-fx-text-box-border: red ;");
			return;
		}
		
		String name = scenarioNameTextField.textProperty().getValue();
		
		if(!name.isEmpty()){
			if(!scenarioListContainsName(name)){
				scenario.setName(name);
			}else{
				statusLabel.setStyle("-fx-text-box-border: red ;");
				statusLabel.setText("Scenario name already in use.");
				
				scenarioNameTextField.setStyle("-fx-text-box-border: red ;");
				return;
			}
			
		}else{
			scenarioNameTextField.setStyle("-fx-text-box-border: red ;");
			return;
		}
		
		scenario.setCreationDate(new Date());
		
		System.out.println("saved scenario:");
		System.out.println(scenario);
		
		statusLabel.setText("Scenario successfully saved.");
		scenarioNameTextField.setStyle("");
		
		dataHandler.addScenario(scenario);
		this.scenarioList.add(scenario);
		
	}
	
	public void setPath(){
		StringBuilder pathBuilder = new StringBuilder();
		for (TreeItem<String> item = structuredTreeview.getSelectionModel().getSelectedItem();
		    item != null ; item = item.getParent()) {
			
		    pathBuilder.insert(0, item.getValue());
		    pathBuilder.insert(0, ",");
		}
		System.out.println("old="+pathBuilder.toString());
		pathBuilder.deleteCharAt(0);
		
		if(fileTypeChoicebox.getSelectionModel().getSelectedItem()=="JSON"){
			pathBuilder.delete(0,"object,".length());
		}else{
			pathBuilder.delete(0,pathBuilder.indexOf(",")+1);
		}
		
		System.out.println("new="+pathBuilder.toString());
		pathToValue=pathBuilder.toString();
		statusLabel.setStyle("-fx-text-box-border: green ;");
		statusLabel.setText("Path set:"+pathToValue);
	}
	
	public void downloadFile(String URL, File target, String fileType) throws IOException{
		try{
			URL url = new URL(URL);
		    Path targetPath = target.toPath();
		    
		    Files.copy(url.openStream(), targetPath, StandardCopyOption.REPLACE_EXISTING);
		    validUrl=true;
	    }catch(IOException e){
	    	
			statusLabel.setText("Couldn't reach URL.");
			statusLabel.setStyle("-fx-text-inner-color: red;");
	    	URLTextfield.setStyle("-fx-text-box-border: red ;");
	    	System.out.println(e);
	    	return;
	    }
		statusLabel.setText("");
		URLTextfield.setStyle("");

	}
	
	public boolean scenarioListContainsName(String name){
		for(ScenarioModel s: this.scenarioList){
			System.out.println(s.getName() +","+ name);
			if(s.getName().equals(name)){
				System.out.println("Name already in use.");
				return true;
			}
		}
		return false;
	}



	public void setDataHandler(DataHandler dataHandler) {
		this.dataHandler = dataHandler;
	}
	
	
}
