
import java.io.*;
import java.util.*;
import java.util.Map.Entry;

public class ProcessData {
	
	private static final int NUSERS = 700;
	private final String filename;
	private List<QuestionData> datalist;
	private Map<String, String[]> userid2uid;
	private StatusData[] userstatuslist;
	private Map<String, List<String>> unlabstatus; 
	
	public ProcessData(String fname){
		filename = fname;
		datalist = new ArrayList<QuestionData>();
		userid2uid = new HashMap<String, String[]>();
		userstatuslist = new StatusData[NUSERS];
		unlabstatus = new HashMap<String, List<String>>();
	}
	
	public void readFile() {
		try {
			BufferedReader br = new BufferedReader(new FileReader(filename));
			String line;
			int linecount = 0;
			while ((line = br.readLine()) != null) {
				++linecount;
				if(linecount > 1) {
					String[] token = line.split(",");
					int uid = linecount-1;
					String userid = token[0];
					String tstamp = token[1];
					
					int[] ques = new int[20];
					for(int i=0; i<20; ++i) {
						ques[i] = Integer.parseInt(token[i+2]);
					}
					
					int score = Integer.parseInt(token[token.length-2]);
					String sign = token[token.length-1];
					QuestionData d = new QuestionData(uid, userid, tstamp, ques, score, sign);
					datalist.add(d);
					String[] list = new String[]{String.valueOf(uid), String.valueOf(score), sign};
					userid2uid.put(userid, list);
				}	   
			}
			
			br.close();
		} catch(Exception e) {
			System.err.println(" Read Error: " + e.getMessage());
		}
		
	}
	
	public void readStatusFile(String filename) {
		try {
			BufferedReader br = new BufferedReader(new FileReader(filename));
			String line, lineprocess = "";
			int linecount = 0;
			
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
					
						if(userid2uid.containsKey(userid)) {
							String[] list = userid2uid.get(userid);
							int uid = Integer.parseInt(list[0]);
							int score = Integer.parseInt(list[1]);
							String sign = list[2];
							
							if(userstatuslist[uid-1] == null) {
								userstatuslist[uid-1] = new StatusData(userid, uid, score, sign);
								userstatuslist[uid-1].addStatus(status, date);
							}
							else {
								userstatuslist[uid-1].addStatus(status, date);
							}
						}
						else {
							System.out.println("userid " + userid + "doesn't exist");
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
			System.err.println("Read Status Error: " + e.getMessage());
		}
		
	}
	
	public void readUnlabStatusFile(String filename) {
		try {
			BufferedReader br = new BufferedReader(new FileReader(filename));
			String line, lineprocess = "";
			int linecount = 0;
			
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
						
						if(!unlabstatus.containsKey(userid)) {
							List<String> statusList = new ArrayList<String>();
							statusList.add(status);
							unlabstatus.put(userid, statusList);
						}
						else {
							List<String> statusList = unlabstatus.get(userid);
							statusList.add(status);
							unlabstatus.put(userid, statusList);
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
			System.err.println("Read Status Error: " + e.getMessage());
		}
	}
	
	public void writeUnlabStatusFiles() {
		Writer writer = null;
		
		Iterator<Entry<String, List<String>>> it = unlabstatus.entrySet().iterator();
		while(it.hasNext()) {
			Map.Entry<String, List<String>> pairs = it.next();
			List<String> statusList = pairs.getValue();
			
			try {
			    writer = new BufferedWriter(new OutputStreamWriter(
			          new FileOutputStream("data\\unlabeledstatusfiles\\user-" + pairs.getKey() + ".txt"), "utf-8"));
			    
			    writer.write(pairs.getKey() + "\n");
			    for(int i=0; i<statusList.size(); ++i) {
			    	writer.write(statusList.get(i).toLowerCase() + "\n");
			    }
			   
			} catch (IOException ex) {
				System.err.println("Write Error: " + ex.getMessage());
			} finally {
			   try {writer.close();} catch (Exception ex) {}
			}
			
		}
		
	}
	
	public void writeStatusFiles() {
		
		Writer writer = null;

		for(int fileid=0; fileid<userstatuslist.length; ++fileid) {
			if(userstatuslist[fileid] != null) {
				try {
					System.out.println("Writing file: " + fileid);
				    writer = new BufferedWriter(new OutputStreamWriter(
				          new FileOutputStream("data\\labeledstatusfiles\\user-" + userstatuslist[fileid].getUid() + ".txt"), "utf-8"));
				    
				    writer.write(userstatuslist[fileid].getUid() + "\n");
				    writer.write(userstatuslist[fileid].getUserid() + "\n");
				    writer.write(userstatuslist[fileid].getScore() + "\n");
				    writer.write(userstatuslist[fileid].getSign() + "\n");
				    for(int i=0; i<userstatuslist[fileid].getStatusCount(); ++i) {
				    	writer.write(userstatuslist[fileid].getStatus(i) + "\n");
				    }
				   
				} catch (IOException ex) {
					System.err.println("Write Error: " + ex.getMessage());
				} finally {
				   try {writer.close();} catch (Exception ex) {}
				}
			}
		}	
	}
	
	public void printDataList(int lim) {
		
		int len = lim < datalist.size() ? lim : datalist.size();
		
		for(int i=0; i<len; ++i) {
			System.out.println(datalist.get(i).getUid() + "\t" + 
								datalist.get(i).getUserid() + "\t" +
								datalist.get(i).getTstamp());
			for(int j=0; j<20; ++j) {
				System.out.print(datalist.get(i).getQuesNum(j) + " ");
			}
			System.out.println();
			
			System.out.println(datalist.get(i).getScore() + "\t" +
								datalist.get(i).getSign());
		}
	}
	
	public void SortList() {
		Collections.sort(datalist);
		
		/*
		for(QuestionData temp : datalist) {
			System.out.println(temp.getUid() + "\t" + temp.getScore());
		}
		*/
		
		// get the top-20 counts of 0, 1, 2, 3 for q1-q20
		
		Writer writer = null;

		try {
		    writer = new BufferedWriter(new OutputStreamWriter(
		          new FileOutputStream("data\\bot-25.txt"), "utf-8"));
		    
		    writer.write("QSNO" + "\t" + "C0" + "\t" + "C1"  + "\t" + "C2" 
		    			+ "\t" + "C3" + "\n");
		    for(int q=0; q<20; ++q) {
				int c0 = 0, c1 = 0, c2 = 0, c3 = 0;
				for(int i=0; i<25; ++i) {
					int val = datalist.get(i).getQuesNum(q);
					if(val == 0) {
						++c0;
					}
					else if(val == 1){
						++c1;
					}
					else if(val == 2) {
						++c2;
					}
					else {
						++c3;
					}
				}
				
				if((c0 + c1 + c2 + c3) != 25) {
					System.out.println("Error: count not equal!");
				}
				
				writer.write((q+1) + "\t" + c0 + "\t" + c1 + "\t" + c2 + "\t" + c3 + "\n");
			}
		   
		} catch (IOException ex) {
			System.err.println("Write Error: " + ex.getMessage());
		} finally {
		   try {writer.close();} catch (Exception ex) {}
		}
		
		
		
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final String fname = "data\\train_scores_item_level.csv";
		final String statusfname = "data\\train_statuses.csv";
		final String unlabstatusfname = "data\\extra_statuses.csv";
		ProcessData pd = new ProcessData(fname);
		pd.readFile();
		//pd.printDataList(3);
		//pd.SortList();
		//pd.readStatusFile(statusfname);
		//pd.writeStatusFiles();
		
		System.out.println("Reading file: " + unlabstatusfname);
		pd.readUnlabStatusFile(unlabstatusfname);
		System.out.println("Writing content of file: " + unlabstatusfname);
		pd.writeUnlabStatusFiles();
		System.out.println("Done !!!");
	}

}
