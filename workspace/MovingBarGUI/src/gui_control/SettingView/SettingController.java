package gui_control.SettingView;

import java.io.IOException;

import gui_control.ScenarioModel.ScenarioModel;
import gui_control.WirelessConnection.BluetoothConnection;
import gui_control.WirelessConnection.MessageHandler;
import gui_control.WirelessModel.WirelessConnectionModel;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;

public class SettingController {
	 @FXML
	 private TableView<WirelessConnectionModel> devicesTableview;

	 @FXML
	 private TableColumn<WirelessConnectionModel, String> devicesNameTableColumn;

	 @FXML
	 private TableColumn<WirelessConnectionModel, String> devicesAddressTableColumn;

	 @FXML
	 private Button searchDevicesButton;

	 @FXML
	 private Button SelectDeviceButton;
	 
	
	 ObservableList<WirelessConnectionModel> wirelessConnections = FXCollections.observableArrayList();
	 BluetoothConnection conn;
	 MessageHandler messageHandler;
	 
	 boolean foundValidDevice = false;
	 
	 public void initialize() throws InterruptedException{
			
			devicesNameTableColumn.setCellValueFactory(new PropertyValueFactory<>("deviceName"));
		 	devicesAddressTableColumn.setCellValueFactory(new PropertyValueFactory<>("deviceAddress"));
			
			
			devicesTableview.setItems(wirelessConnections);
			conn.setObservableList(wirelessConnections);
		}
	 
	 public void searchForDevices() throws IOException, InterruptedException{
		 //wirelessConnections = null;
		 conn.searchForDevices();
		 //wirelessConnections = FXCollections.observableArrayList(conn.getWirelessConnectionModels());
	 }
	 
	 public void selectDevice(){
		 WirelessConnectionModel selectedEntry = devicesTableview.getSelectionModel().getSelectedItem();
		 if(selectedEntry!=null){
			 if(!conn.setSelectedDevice(selectedEntry)){
				 System.out.println("Please search for devices once again");
				 foundValidDevice = false;
			 }else{
				 //device found in list
				 foundValidDevice = true;
			 }
		 }
		 //conn.sendMessage("Hallo Huawei");
	 }
	 
	 public void setWirelessConnectionHandler( BluetoothConnection wirelessConnection){
		 this.conn = wirelessConnection;
	 }
	 
	 public void setMessageHandler(MessageHandler messageHandler){
	    	this.messageHandler = messageHandler;
	    	 this.conn = messageHandler.getWirelessConnection();
	    }

	public boolean isDeviceSet() {
		return foundValidDevice;
	}

	 
	 
}
