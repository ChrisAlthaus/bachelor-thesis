package gui_control.view;

import java.io.IOException;

import gui_control.Main;
import javafx.fxml.FXML;

public class MainController {

	//private static Main main;
	@FXML
	public void goSettingView() throws IOException{
		Main.showSettingView();
	}
	
	@FXML
	public void goLedView() throws IOException{
		Main.showLedView();
	}
	
	public void showHelpStage() throws IOException{
		Main.showHelpStage();
	}
	
	public void goEditorView() throws IOException{
		Main.showEditorView();
	}
	

}
