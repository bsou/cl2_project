
public class QuestionData implements Comparable<QuestionData>{
	private final int uid;
	private final String userid;
	private final String tstamp;
	private final int[] ques;
	private final int score;
	private final String sign;
	
	public QuestionData(int Uid, String Userid, String Tstamp, int[] Ques, 
			int Score, String Sign) {
		
		uid = Uid;
		userid = Userid;
		tstamp = Tstamp;
		
		ques = new int[20];
		for(int i=0; i<20; ++i){
			ques[i] = Ques[i];
		}
		
		score = Score;
		sign = Sign;
	}
	
	public int getUid() {
		return uid;
	}
	
	public String getUserid() {
		return userid;
	}
	
	public String getTstamp() {
		return tstamp;
	}
	
	public int[] getQues() {
		return ques;
	}

	public int getQuesNum(int num) {
		return ques[num];
	}
	
	public int getScore() {
		return score;
	}	
	
	public String getSign() {
		return sign;
	}
	
	public int compareTo(QuestionData other) {
		 
		int compareQuantity = ((QuestionData) other).getScore(); 
 
		//ascending order
		//return this.score - compareQuantity;
 
		//descending order
		return compareQuantity - this.score;
 
	}
}
