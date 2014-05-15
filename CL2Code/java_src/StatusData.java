import java.util.*;

public class StatusData {
	/*
	 * contains information about status messages and other information on a per user basis
	 */
	
	private final String userid;
	private final int uid;
	private final int score;
	private final String sign;
	private final int gender;
	private int statusCount;
	private List<String> statusList;
	private List<String> dateOfstatuslist;
	
	public StatusData(String Userid, int Uid, int Score, String Sign, int Gender) {
		userid = Userid;
		uid = Uid;
		score = Score;
		sign = Sign;
		gender = Gender;
		statusCount = 0;
		statusList = new ArrayList<String>();
		dateOfstatuslist = new ArrayList<String>();
	}
	
	public int getUid() {
		return uid;
	}
	
	public String getUserid() {
		return userid;
	}
	
	public int getScore() {
		return score;
	}
	
	public String getSign() {
		return sign;
	}
	
	public int getGender() {
		return gender;
	}
	
	public int getStatusCount() {
		return statusCount;
	}
	
	public void addStatus(String status, String date) {
		statusList.add(status.toLowerCase());
		dateOfstatuslist.add(date);
		++statusCount;
	}
	
	public String getStatus(int idx) {
		if(idx+1 <= statusCount) {
			return statusList.get(idx);
		}
		else {
			return "Index " + idx + " out of bound (" + statusCount + ")";
		}
	}
	
	public String getTimeStamp(int idx) {
		if(idx+1 <= statusCount) {
			return dateOfstatuslist.get(idx);
		}
		else {
			return "Index " + idx + " out of bound (" + statusCount + ")";
		}
	}
}
