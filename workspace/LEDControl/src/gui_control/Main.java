package gui_control;

import java.io.IOException;

import gui_control.led_view.ArduinoLink;
import gui_control.led_view.LEDViewController;
import gui_control.sequence_editor.EditorController;
import gui_control.sequence_editor.SequenceBuilderController;
import gui_control.settingView.SettingsController;
import gui_control.view.MainController;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.FlowPane;
import javafx.stage.Modality;
import javafx.stage.Stage;

public class Main extends Application {
	
	private static Stage primaryStage;
	private static BorderPane main_layout;
	
	private static ArduinoLink arduino_link=new ArduinoLink();
	private static EditorController editor_controller = new EditorController();
	private static LEDViewController led_controller = new LEDViewController();
	private static SettingsController setting_controller = new SettingsController();

	@Override
	public void start(Stage primaryStage) throws IOException {
		this.primaryStage = primaryStage;
		this.primaryStage.setTitle("LED Controller");
		
		
		showMainView();
	}
	
	private static void showMainView() throws IOException{
		FXMLLoader loader = new FXMLLoader();
		loader.setLocation(Main.class.getResource("view/MainView.fxml"));
		
		main_layout = loader.load();
		Scene scene = new Scene(main_layout);
		primaryStage.setScene(scene);
		primaryStage.show();
		
		
	
	}
	
	public static void showLedView() throws IOException{
		FXMLLoader loader = new FXMLLoader();
		loader.setLocation(Main.class.getResource("led_view/LEDView.fxml"));
		loader.setController(led_controller);
		FlowPane led_view = loader.load(); 
		main_layout.setCenter(led_view);
		
		led_controller.setArduinoLink(arduino_link);
		
		if(arduino_link.getArduino_port()==null){
			arduino_link.setDefaultPort();
		}
	}
	
	public static void showSettingView() throws IOException{
		FXMLLoader loader = new FXMLLoader();
		loader.setLocation(Main.class.getResource("settingView/SettingView.fxml"));
		loader.setController(setting_controller);
		
		setting_controller.setArduinoLink(arduino_link);
		
		if(arduino_link.getArduino_port()==null){
			arduino_link.setDefaultPort();
		}
		
		BorderPane setting = loader.load(); 
		main_layout.setCenter(setting);
		
		
	}
	
	public static void showEditorView() throws IOException{
		FXMLLoader loader = new FXMLLoader();
		loader.setLocation(Main.class.getResource("sequence_editor/SequenceEditorView.fxml"));
		loader.setController(editor_controller);
		
		BorderPane editor = loader.load(); 
		main_layout.setCenter(editor);
		
		editor_controller.setArduinoLink(arduino_link);
		
		if(arduino_link.getArduino_port()==null){
			arduino_link.setDefaultPort();
		}
	
	}
	
	public static void showSequenceBuilder() throws IOException{
		FXMLLoader loader = new FXMLLoader();
		loader.setLocation(Main.class.getResource("sequence_editor/SequenceBuilder.fxml"));
		BorderPane builder_view = loader.load();
		
		SequenceBuilderController controller = (SequenceBuilderController)loader.getController();
		System.out.println(controller);
		controller.setMainEditor(editor_controller);
		
		Stage builder_stage = new Stage();
		builder_stage.setTitle("Sequence Builder");
		builder_stage.initModality(Modality.NONE);
		builder_stage.initOwner(primaryStage);
		
		Scene scene = new Scene(builder_view);
		builder_stage.setScene(scene);
		builder_stage.showAndWait();
		
		
	}
	
	public static void showHelpStage() throws IOException{
		FXMLLoader loader = new FXMLLoader();
		loader.setLocation(Main.class.getResource("view/HelpView.fxml"));
		BorderPane helpView = loader.load();
		
		Stage addHelpStage = new Stage();
		addHelpStage.setTitle("Help");
		addHelpStage.initModality(Modality.WINDOW_MODAL);
		addHelpStage.initOwner(primaryStage);
		
		Scene scene = new Scene(helpView);
		addHelpStage.setScene(scene);
		addHelpStage.showAndWait();
	}

	public static void main(String[] args) {
		launch(args);
	}
}
