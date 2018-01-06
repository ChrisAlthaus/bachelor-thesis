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
	
	public void setTreeViewJSON(File file) throws SAXException, ParserConfigurationException, IOException{
		
		XMLBuilder builder = new XMLBuilder();
		
		JSONParser parser = new JSONParser();
		String xml = parser.getXMLString(file);
		
		//check if valid xml returned
		if(xml!=null){
			structuredTreeview.setRoot(builder.getTreeView(xml));
		}
		
	}
	
	public void saveScenarioAndExit(){
		ScenarioModel scenario = new ScenarioModel();
		//String url = URLTextfield.textProperty().getValue();
		if(!url.isEmpty() && validUrl){
			scenario.setRequestURL(url);
		}else{
			URLTextfield.setStyle("-fx-text-box-border: red ;");
			return;
		}
		
		
		String fileType = fileTypeChoicebox.getSelectionModel().getSelectedItem();
		if(fileType=="JSON"){
			scenario.setPathToJson(pathToValue);
		}else if(fileType=="XML"){
			scenario.setPathToXML(pathToValue);
		}else{
			fileTypeChoicebox.setStyle("-fx-text-box-border: red ;");
			return;
		}
		
		String name = scenarioNameTextField.textProperty().getValue();
		
		if(!name.isEmpty()){
			if(!scenarioListContainsName(name)){
				scenario.setName(name);
			}else{
				System.out.println("Scenario name already used.");
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
		
		dataHandler.addScenario(scenario);
		this.scenarioList.add(scenario);
		
	}
	
	public void setPath(){
		StringBuilder pathBuilder = new StringBuilder();
		for (TreeItem<String> item = structuredTreeview.getSelectionModel().getSelectedItem();
		    item != null ; item = item.getParent()) {

		    pathBuilder.insert(0, item.getValue());
		    pathBuilder.insert(0, "/");
		}
		System.out.println(pathBuilder.toString());
		pathToValue=pathBuilder.toString();
	}
	
	public void downloadFile(String URL, File target, String fileType) throws IOException{
		try{
			URL url = new URL(URL);
		    Path targetPath = target.toPath();
		    
		    Files.copy(url.openStream(), targetPath, StandardCopyOption.REPLACE_EXISTING);
		    validUrl=true;
	    }catch(IOException e){
	    	System.out.println("error: couldn't reach url.");
	    	URLTextfield.setStyle("-fx-text-box-border: red ;");
	    	return;
	    }
	    
		URLTextfield.setStyle("");

	}
	
	public boolean scenarioListContainsName(String name){
		for(ScenarioModel s: this.scenarioList){
			if(s.getName() == name){
				return true;
			}
		}
		return false;
	}



	public void setDataHandler(DataHandler dataHandler) {
		this.dataHandler = dataHandler;
	}
	
	
}
