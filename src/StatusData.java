import java.util.*;

public class StatusData {
	private final String userid;
	private final int uid;
	private final int score;
	private final String sign;
	private int statusCount;
	private List<String> statusList;
	private List<String> dateOfstatuslist;
	
	public StatusData(String Userid, int Uid, int Score, String Sign) {
		userid = Userid;
		uid = Uid;
		score = Score;
		sign = Sign;
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
}
