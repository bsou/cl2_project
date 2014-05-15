import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.*;
import java.util.Map.Entry;


public class TestDataSet {
	/*
	 * class for processing the test data set
	 */
	
	private final int NTUSERS = 239;
	private final String fileMeta;
	private final String fileStatus;
	private Map<String, Integer> user2gender;
	private Map<String, List<String>> statusList;
	
	public TestDataSet (String meta, String status) {
		this.fileMeta = meta;
		this.fileStatus = status;
		this.user2gender = new HashMap<String, Integer>();
		this.statusList = new HashMap<String, List<String>>();
	}
	
	public void readMetaFile() {
		try {
			System.out.println("Reading file: " + this.fileMeta);
			BufferedReader br = new BufferedReader(new FileReader(this.fileMeta));
			String line;
			int linecount = 0;
			while ((line = br.readLine()) != null) {
				++linecount;
				System.out.println("line: " + linecount);
				if(linecount > 1) {
					String[] token = line.split(",");
					String userid = token[0];
					String timestamp = token[1];
					int gender = Integer.parseInt(token[2]);
				
					if (!user2gender.containsKey(userid)) {
						user2gender.put(userid, gender);
					}
					else {
						System.out.println("Duplicate userid: " + userid);
					}
				}	   
			}
			
			br.close();
		} catch(Exception e) {
			System.err.println("*** Read Error: " + e.getMessage());
		}
	}
	
	public void readStatusFile() {
		System.out.println("Reading file: " + this.fileStatus);
		int linecount = -1;
		try {
			BufferedReader br = new BufferedReader(new FileReader(this.fileStatus));
			String line, lineprocess = "";
			linecount = 0;
			
			while ((line = br.readLine()) != null) {
				++linecount;
				
				if(linecount > 1) {
	
					String[] token = line.split(",");
					
					if((token[token.length-1].length() == 19) && 
							(token[token.length-1].matches("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}"))) {

						lineprocess += line;
						
						String[] ptoken = lineprocess.split(",");
						
						String userid = "";
						String status = "";
						String date = "";
						int count = 0;
						
						for(int i=0; i<ptoken.length; ++i) {
							if(i == 0) {
								userid= ptoken[i];
							}
							else if(i == ptoken.length-1) {
								date = ptoken[i];
							}
							else {
								++count;
								if(count > 1) {
									status += ",";
								}
								status += ptoken[i];
							}
						}
					
						if(!statusList.containsKey(userid)) {
							
							String finalstatus = date + " " + status;
							List<String> list = new ArrayList<String>();
							list.add(finalstatus);
							statusList.put(userid, list);
						}
						else {
							String finalstatus = date + " " + status;
							List<String> list = statusList.get(userid);
							list.add(finalstatus);
							statusList.put(userid, list);
						}	
						
						lineprocess = "";
					}
					else {
						
						lineprocess += line;
					}
				}	   
			}
			
			br.close();
		} catch(Exception e) {
			System.err.println("Read Status Error: " + e.getMessage() + " line: " + linecount);
		}
	}
	
	public void writeToFile() {
		
		Writer writer = null;
		
		Iterator<Entry<String, List<String>>> it = statusList.entrySet().iterator();
		while(it.hasNext()) {
			Map.Entry<String, List<String>> pairs = it.next();
			List<String> statuslist = pairs.getValue();
			
			try {
			    writer = new BufferedWriter(new OutputStreamWriter(
			          new FileOutputStream("data\\teststatusfiles\\user-" + pairs.getKey() + ".txt"), "utf-8"));
			    
			    String userid = pairs.getKey();
			    
			    writer.write(userid + "\n");
			    
			    int gender = -1;
			    if (user2gender.containsKey(userid)) {
			    	gender = user2gender.get(userid);
			    }
			    else {
			    	System.out.println("userid: " + userid + " does not exist in user2gender map");
			    }
			    
			    writer.write(gender + "\n");
			    
			    for(int i=0; i<statuslist.size(); ++i) {
			    	writer.write(statuslist.get(i).toLowerCase() + "\n");
			    }
			   
			} catch (IOException ex) {
				System.err.println("Write Error: " + ex.getMessage());
			} finally {
			   try {writer.close();} catch (Exception ex) {}
			}
			
		}
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final String fmeta = "data\\test_metadata.csv";
		final String fstatus = "data\\test_statuses.csv";
		TestDataSet tds = new TestDataSet(fmeta, fstatus);
		tds.readMetaFile();
		tds.readStatusFile();
		tds.writeToFile();
	}
	
	

}
