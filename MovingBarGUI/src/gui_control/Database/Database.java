package gui_control.Database;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Date;
import java.sql.Types;

import com.sun.javafx.scene.control.skin.FXVK.Type;

import gui_control.MovingBarOverallModel.MovingBarOverallModel;
import gui_control.MovingBarSideModel.MovingBarSideModel;
import gui_control.ScenarioModel.ScenarioModel;
import gui_control.SocketModel.SocketModel;
import javafx.scene.paint.Color;

public class Database {
	
	public Database(){
		
	}
	
	public void init() throws SQLException{
		createTables();
	}
	
	public static Connection connect(){
		Connection c = null;
		
		try {
			c = DriverManager.getConnection("jdbc:sqlite:database.db");
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("Connection to SQLite has been established.");
		return c;
	}
	
	public void createTables() throws SQLException{
		Connection c = connect();
		if(c==null){
			System.out.println("Connection has not been established.");
			return;
		}
		Statement stmt = null;

		
		stmt = c.createStatement();
        String sql = "CREATE TABLE IF NOT EXISTS MOVINGBAR " +
                       "(NAME 			TEXT  PRIMARY KEY     NOT NULL," +
                       " SCENARIO   TEXT    NOT NULL, " + 
                       " DISPLAYCOLOR   TEXT    NOT NULL, " + 
                       " REFERENCECOLOR TEXT     NOT NULL, " + 
                       " MODE TEXT     NOT NULL, " + 
                       " REFERENCEVALUE INT NOT NULL, " + 
                       " STEPSIZE INT NOT NULL)"; 
        stmt.executeUpdate(sql);
        
        stmt = c.createStatement();
        sql = "CREATE TABLE IF NOT EXISTS MODES " +
                       "(NAME 			TEXT  PRIMARY KEY     NOT NULL)"; 
        stmt.executeUpdate(sql);
        
        stmt = c.createStatement();
        sql = "CREATE TABLE IF NOT EXISTS SCENARIOS" +
                       "(NAME 			TEXT  PRIMARY KEY     NOT NULL,"+
                       " REQUESTURL   TEXT NOT NULL, " + 
                       " PATHXML   TEXT, " + 
                       " PATHJSON TEXT, " + 
                       " CREATIONDATE TEXT     NOT NULL, " + 
                       " UPDATEDURATION REAL NOT NULL)";
        stmt.executeUpdate(sql);
       
        stmt = c.createStatement();
        sql = "CREATE TABLE IF NOT EXISTS NETWORKSOCKET" +
        		       "( ID INTEGER PRIMARY KEY NOT NULL,"+
                       " IP TEXT  NOT NULL,"+
                       " PORTNUMBER   TEXT NOT NULL) ";
        stmt.executeUpdate(sql);
        
        
        stmt = c.createStatement();
        sql = "CREATE TABLE IF NOT EXISTS OVERALLMOVINGBARSETTINGS" +
                       "( ID INTEGER PRIMARY KEY NOT NULL,"+
                       	"SAMPLETIME NUMBER NOT NULL," +
                        "BRIGHTNESS NUMBER NOT NULL) ";
        stmt.executeUpdate(sql);
        
        stmt.close();
        c.close();
        
	}
	
	public void insertMovingBarSideModel(MovingBarSideModel m) throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		
		PreparedStatement stmt = null;
		
		c.setAutoCommit(false);
		
		String sql = "INSERT OR REPLACE INTO MOVINGBAR (NAME,SCENARIO,DISPLAYCOLOR,REFERENCECOLOR,MODE,REFERENCEVALUE,STEPSIZE) " +
                " VALUES (?,?,?,?,?,?,?);";
		stmt = c.prepareStatement(sql);
		
		System.out.println("insert into db:");
		System.out.println(m);
        
		if(m.getName()==null){
			return;
		}else{
			stmt.setString(1, m.getName());
		}
		
		if(m.getScenarioName()==null){
			return;
		}else{
			stmt.setString(2,m.getScenarioName());
		}
		
		if(m.getQuantityColor()==null){
			stmt.setString(3, null);
		}else{
			stmt.setString(3, String.valueOf(m.getQuantityColor().getRGB()));
		}
		
		if(m.getReferenceColor()==null){
			stmt.setString(4,null);
		}else{
			stmt.setString(4, String.valueOf(m.getReferenceColor().getRGB()));
		}
		
		if(m.getMode()==null){
			stmt.setString(5,null);
		}else{
			stmt.setString(5, m.getMode());
		}
		
		
		stmt.setInt(6, m.getReferenceValue());
		stmt.setInt(7, m.getStepSize());
		
		stmt.executeUpdate();
        
        stmt.close();
        c.commit();
        c.close();
        
        System.out.println("Successfully inserted data.");
        
    	/*String sql = "INSERT OR REPLACE INTO MOVINGBAR (NAME,DISPLAYCOLOR,REFERENCECOLOR,MODE,REFERENCEVALUE,STEPSIZE) " +
        "VALUES ('"+m.getName()+"', '"+String.valueOf(m.getQuantityColor().getRGB())+"', '" 
        		+String.valueOf(m.getReferenceColor().getRGB())+"', '"
        			+m.getMode()+"', "+m.getReferenceValue()+", "+m.getStepSize()+");"; */
	}
	
	public void insertModes(ArrayList<String> modes) throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		Statement stmt = null;
		
		c.setAutoCommit(false);
		stmt = c.createStatement();
		
		for(String mode: modes){
			String sql = "INSERT OR REPLACE INTO MODES (NAME) " +
                    "VALUES ('"+ mode + "');"; 
			
			stmt.executeUpdate(sql);
		}
		
		 stmt.close();
	     c.commit();
	     c.close();
	     
	     System.out.println("Successfully inserted data.");
	}
	
	public void insertScenarios(ArrayList<ScenarioModel> scenarios) throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		PreparedStatement stmt = null;
		
		c.setAutoCommit(false);
		
		String sql = "INSERT OR REPLACE INTO SCENARIOS (NAME,REQUESTURL,PATHXML,PATHJSON,CREATIONDATE,UPDATEDURATION) " +
                " VALUES (?,?,?,?,?,?);";
		stmt = c.prepareStatement(sql);
		
		
		for(ScenarioModel s: scenarios){
			if(s.getName()==null){
				continue;
			}else{
				stmt.setString(1, s.getName());
			}
			
			if(s.getRequestURL()==null){
				continue;
			}else{
				stmt.setString(2, s.getRequestURL());
			}
			
			if(s.getPathToXML()==null){
				stmt.setString(3,null);
			}else{
				stmt.setString(3, s.getPathToXML());
			}
			
			if(s.getPathToJson()==null){
				stmt.setString(4,null);
			}else{
				stmt.setString(4, s.getPathToJson());
			}
			
			if(s.getCreationDate() == null){
				stmt.setString(5,null);
			}else{
				stmt.setString(5, s.getCreationDate());
			}
			
			stmt.setDouble(6, s.getUpdateDuration());
			
			stmt.executeUpdate();
		}
		
		 stmt.close();
	     c.commit();
	     c.close();
	     /*String sql = "INSERT OR REPLACE INTO SCENARIOS (NAME,REQUESTURL,PATHXML,PATHJSON,CREATIONDATE,UPDATEDURATION) " +
                 "VALUES ('"+s.getName()+"', '"+s.getRequestURL()+"'," 
     			+s.getPathToXML()+", "+
                 s.getPathToJson()+", "+
     			s.getDateText()+","
                 +s.getUpdateDuration()+" );"; */
	     System.out.println("Successfully inserted data.");
	}
	
	public void insertNetworkSocket(SocketModel socket) throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		Statement stmt = null;
		
		c.setAutoCommit(false);
		stmt = c.createStatement();
		
	
		String sql = "INSERT OR REPLACE INTO NETWORKSOCKET (ID,IP,PORTNUMBER) " +
                    "VALUES ( 1 ,'"+ socket.getIp() + "' ,"+ socket.getPortNumber() + ");"; 
			
		stmt.executeUpdate(sql);
		
		
		 stmt.close();
	     c.commit();
	     c.close();
	     
	     System.out.println("Successfully inserted data.");
	}
	
	public void insertOverallSettings(MovingBarOverallModel overallSettings) throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		Statement stmt = null;
		
		c.setAutoCommit(false);
		stmt = c.createStatement();
		
	
		String sql = "INSERT OR REPLACE INTO OVERALLMOVINGBARSETTINGS (ID, SAMPLETIME, BRIGHTNESS) " +
                    "VALUES ( 1 ," + overallSettings.getSampleTime() + "," + overallSettings.getBrightness() +");"; 
			
		stmt.executeUpdate(sql);
		
		
		 stmt.close();
	     c.commit();
	     c.close();
	     
	     System.out.println("Successfully inserted data.");
	}
	
	public void resetScenarioTable() throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		Statement stmt = null;
		
		c.setAutoCommit(false);
		stmt = c.createStatement();
		
		String sql = "DELETE FROM SCENARIOS"; 
			
		stmt.executeUpdate(sql);
		
		
		 stmt.close();
	     c.commit();
	     c.close();
	     
	     System.out.println("Successfully cleaned table for scenarios.");
	}
	
	
	public MovingBarSideModel getMovingBarSide(String name) throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		Statement stmt = null;
		
		c.setAutoCommit(false);
		
		stmt = c.createStatement();
	    ResultSet rs = stmt.executeQuery( "SELECT * FROM MOVINGBAR "
	    	                              + "WHERE NAME IS '" + name + "';" );
	    
	   
	    if(rs.next()){
		    String nameIdentifier = rs.getString("name");
		    String scenarioName = rs.getString("scenario");
			String quantityColor= rs.getString("displaycolor");
			String referenceColor= rs.getString("referencecolor");
			String mode= rs.getString("mode");;
			int referenceValue = rs.getInt("referencevalue");
			int stepSize = rs.getInt("stepsize");	
			
			MovingBarSideModel result = new MovingBarSideModel(nameIdentifier,scenarioName,quantityColor,referenceColor,mode,
					referenceValue,stepSize);
			
			rs.close();
		    stmt.close();
		    c.close();
			return result;
	    }else{
	    	rs.close();
	        stmt.close();
	        c.close();
	        
	    	System.out.println("No entry in database.");
	    	return null;
	    }
	}
	
	
	public ArrayList<String> getModes() throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		Statement stmt = null;
		
		c.setAutoCommit(false);
		
		stmt = c.createStatement();
	    ResultSet rs = stmt.executeQuery( "SELECT * FROM MODES;" );
	    
	    ArrayList<String> result = new ArrayList<String>();
	    
	    if(!rs.isBeforeFirst())result = null;
	    //rs.beforeFirst();
	    
	    while(rs.next()){
		    String mode = rs.getString("name");
		    result.add(mode);
	    }
	    
	    rs.close();
	    stmt.close();
	    c.close();
	    return result;
	}
	
	
	public ArrayList<ScenarioModel> getScenarios() throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		Statement stmt = null;
		
		c.setAutoCommit(false);
		
		stmt = c.createStatement();
	    ResultSet rs = stmt.executeQuery( "SELECT * FROM SCENARIOS;" );
	    
	    ArrayList<ScenarioModel> result = new ArrayList<ScenarioModel>();

	    if(!rs.isBeforeFirst())result = null;
	    //rs.beforeFirst();
	    
	    while(rs.next()){
			String nameIdentifier = rs.getString("name");
			String requestURL = rs.getString("requesturl");
			String pathToXML= rs.getString("pathxml");
			String pathToJson= rs.getString("pathjson");
			String creationDate= rs.getString("creationdate");
			double updateDuration = rs.getDouble("updateduration");
			
			ScenarioModel s = new ScenarioModel(nameIdentifier, requestURL, pathToXML, pathToJson, creationDate,updateDuration);
			result.add(s);
				
	    }
	    
	    rs.close();
		stmt.close();
		c.close();
		return result;
	    
	}
	
	
	public SocketModel getSocket() throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		Statement stmt = null;
		
		c.setAutoCommit(false);
		
		stmt = c.createStatement();
	    ResultSet rs = stmt.executeQuery( "SELECT * FROM NETWORKSOCKET;" );
	    
	    SocketModel result = null;
	    
	    if(rs.next()){
		    String ip = rs.getString("ip");
		    int portNumber=rs.getInt("portnumber");
		    result = new SocketModel(ip,portNumber);
	    }
	    
	    rs.close();
	    stmt.close();
	    c.close();
	    return result;
	}
	
	public MovingBarOverallModel getOverallSettings() throws SQLException{
		Connection c = connect();
		//if(c==null)return;
		Statement stmt = null;
		
		c.setAutoCommit(false);
		
		stmt = c.createStatement();
	    ResultSet rs = stmt.executeQuery( "SELECT * FROM OVERALLMOVINGBARSETTINGS;" );
	    
	    MovingBarOverallModel result = null;
	    
	    if(rs.next()){
	    	int sampleTime=rs.getInt("sampletime");
		    int brightness=rs.getInt("brightness");
		    result = new MovingBarOverallModel(sampleTime,brightness);
	    }
	    
	    rs.close();
	    stmt.close();
	    c.close();
	    return result;
	}
	
}

	

