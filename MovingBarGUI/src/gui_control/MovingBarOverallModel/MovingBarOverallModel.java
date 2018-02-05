package gui_control.MovingBarOverallModel;

public class MovingBarOverallModel {
	int brightness;
	int sampleTime;

	public MovingBarOverallModel(int brightness,int sampleTime) {
		this.brightness = brightness;
		this.sampleTime = sampleTime;
	}
	
	

	public MovingBarOverallModel() {		//default values
		brightness=75;
		sampleTime=10;
	}

	

	public int getSampleTime() {
		return sampleTime;
	}



	public void setSampleTime(int sampleTime) {
		this.sampleTime = sampleTime;
	}



	public int getBrightness() {
		return brightness;
	}

	public void setBrightness(int brightness) {
		this.brightness = brightness;
	}
	
	
	
}
