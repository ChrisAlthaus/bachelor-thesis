package gui_control.XMLParser;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.XML;
import org.xml.sax.SAXException;

import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;


public class JSONParser {

	public String getXMLString(File jsonFile) throws FileNotFoundException{
		String fileString= getString(jsonFile);
		System.out.println(fileString);
		
		JSONObject json=null;
		try{
			json = new JSONObject(fileString);
		}catch(JSONException e){
			System.out.println("warning:JSON file is not displayed, because text string has wrong syntax.");
			return null;
		}
		System.out.println("JSON=");
		System.out.println(json);
		String xmlText = XML.toString(json,"object");
		System.out.println("XML=");
		System.out.println(xmlText);

		
		return xmlText;
	}
	
	public String getString(File file) throws FileNotFoundException{
		Scanner scanner = new Scanner(file);
		String content =scanner.useDelimiter("\\Z").next();
		scanner.close();
		return content;
	}
}
