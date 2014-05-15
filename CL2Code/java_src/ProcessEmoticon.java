import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.File;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.*;

public class ProcessEmoticon {
	/*
	 * replaces emoticons with words 
	 */
	
	private static final int NUSERS = 700;
	private final String filename;
	private Map<String, String> emotimap;
	
	public ProcessEmoticon(String filename) {
		this.filename = filename;
		emotimap = new HashMap<String, String>();
	}
	
	public void readEmoticonFile() {
		BufferedReader br;
		try {
			br = new BufferedReader(new FileReader(filename));
			String line;
			int linecount = 0;
			String expr = "";
			while ((line = br.readLine()) != null) {
				++linecount;
				if (linecount%2 != 0) {
					//System.out.print(line);
					expr = line;
				}
				else {
					String[] token = line.split(" ");
					for (int i=0; i<token.length; i++) {
						//System.out.print(" %%%% " + token[i]);
						if(!emotimap.containsKey(token[i])) {
							emotimap.put(token[i].trim(), expr);
						}
						else {
							System.out.println("Duplicate token: " + token[i]);
						}
					}
					//System.out.println(" %%%%");
				}   
			}
			
			br.close();
		} catch(Exception e) {
			System.err.println(" Read Error: " + e.getMessage());
		}
		finally {
			//printMap(this.emotimap);
		}
	}
	
	public void processStatusFiles() {
		//final File folder = new File("data\\labeledstatusfiles");
		final File folder = new File("data\\teststatusfiles");
		File[] listOfFiles = folder.listFiles();
		//System.out.println("Number of Files: " + listOfFiles.length);
		
		for (File file : listOfFiles) {
		    if (file.isFile()) {
		        //System.out.println(folder + "\\" + file.getName());
		    	String sfilename = folder + "\\" + file.getName();
		    	try {
		    		BufferedReader br = new BufferedReader(new FileReader(sfilename));
					String line;
					int linecount = 0;
					
					List<String> fileLines = new ArrayList<String>();
					while ((line = br.readLine()) != null) {
						++linecount;
						if (linecount >= 3) {
							//System.out.println(line);
							String timeStamp = line.substring(0, 19);
							String newline = line.substring(20, line.length());
							
							Iterator<Map.Entry<String, String>> i = emotimap.entrySet().iterator(); 
							while(i.hasNext()){
							    String key = i.next().getKey();
							    newline = newline.replace(key.toLowerCase(), " " + emotimap.get(key) + " ");
							    newline = newline.replace(key.toUpperCase(), " " + emotimap.get(key) + " ");
							    //System.out.println(key + ", " + emotimap.get(key));
							    /*
							    if (key.equals(":)")) {
							    	System.out.println("%%%% " + newline);
							    }
							    */
							}
							//System.out.println("#### " + newline);
							
							String finalLine = timeStamp + " " + newline;
							fileLines.add(finalLine);
					
						}
						else {
							fileLines.add(line);
						}
					}
					
					fileWrite(fileLines, file.getName());
					
					br.close();
				} catch(Exception e) {
					System.err.println(" Read Error: " + e.getMessage());
				}
		    }
		}
		
	}
	
	public void processUnlabeledSatusFiles() {
		final File folder = new File("data\\unlabeledstatusfiles");
		File[] listOfFiles = folder.listFiles();
		//System.out.println("Number of Files: " + listOfFiles.length);
		
		for (File file : listOfFiles) {
		    if (file.isFile()) {
		        //System.out.println(folder + "\\" + file.getName());
		    	String sfilename = folder + "\\" + file.getName();
		    	try {
		    		BufferedReader br = new BufferedReader(new FileReader(sfilename));
					String line;
					int linecount = 0;
					
					List<String> fileLines = new ArrayList<String>();
					while ((line = br.readLine()) != null) {
						++linecount;
						if (linecount >= 2) {
							//System.out.println(line);
							String newline = line;
							Iterator<Map.Entry<String, String>> i = emotimap.entrySet().iterator(); 
							while(i.hasNext()){
							    String key = i.next().getKey();
							    newline = newline.replace(key.toLowerCase(), " " + emotimap.get(key) + " ");
							    newline = newline.replace(key.toUpperCase(), " " + emotimap.get(key) + " ");
							    //System.out.println(key + ", " + emotimap.get(key));
							    /*
							    if (key.equals(":)")) {
							    	System.out.println("%%%% " + newline);
							    }
							    */
							}
							//System.out.println("#### " + newline);
							fileLines.add(newline);
					
						}
						else {
							fileLines.add(line);
						}
					}
					
					fileWrite(fileLines, file.getName());
					
					br.close();
				} catch(Exception e) {
					System.err.println(" Read Error: " + e.getMessage());
				}
		    }
		}
		
	}
	
	public static void fileWrite(List<String> fileContent, String filename) {
		Writer writer = null;

		try {
			System.out.println("Writing file: " + filename);
			
			writer = new BufferedWriter(new OutputStreamWriter(
					new FileOutputStream("data\\newteststatusfiles\\" + filename), "utf-8"));
			
			//writer = new BufferedWriter(new OutputStreamWriter(
					//new FileOutputStream("data\\newlabeledstatusfiles\\" + filename), "utf-8"));
			
			//writer = new BufferedWriter(new OutputStreamWriter(
					//new FileOutputStream("data\\newunlabeledstatusfiles\\" + filename), "utf-8"));

			for(int i=0; i<fileContent.size(); ++i) {
				writer.write(fileContent.get(i) + "\n");
			}

		} catch (IOException ex) {
			System.err.println("Write Error: " + ex.getMessage());
		} finally {
			try {writer.close();} catch (Exception ex) {}
		}
		
	}
	
	public static void printMap(Map<String, String> map) {
		Iterator<Map.Entry<String, String>> i = map.entrySet().iterator(); 
		while(i.hasNext()){
		    String key = i.next().getKey();
		    System.out.println(key + ", " + map.get(key));
		}
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		final String fname = "data\\emoticon.txt";
		//final String fname = "data\\trainScores.csv";
		
		/*final File folder = new File("data");
		File[] listOfFiles = folder.listFiles();
		
		for (File file : listOfFiles) {
		    if (file.isFile()) {
		        System.out.println(file.getName());
		    }
		}*/
		
		ProcessEmoticon pd = new ProcessEmoticon(fname);
		pd.readEmoticonFile();
		pd.processStatusFiles();
		//pd.processUnlabeledSatusFiles();
		
		//String textstr = "I had a good :-| sleep :). The day was :-| good :D.";
		//String textstr = "thank you all for your kind birthday wishes. :)";
		//String newstr = textstr.replace(":)", " happy ");
		//System.out.println(newstr);
	}

}
